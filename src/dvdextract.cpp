/***************************************************************************
 *   Copyright (C) 2005 by Javier Garrido Campoy                           *
 *   Copyright (C) 2001 Billy Biggs <vektor@dumbterm.net>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "dvdextract.h"

/**
 * Returns true if the pack is a NAV pack.  This check is clearly insufficient,
 * and sometimes we incorrectly think that valid other packs are NAV packs.  I
 * need to make this stronger.
 */

int is_nav_pack(unsigned char *buffer)
{
  return (buffer[41] == 0xbf && buffer[1027] == 0xbf);
}

int extractTitleVob(QString dvdDevice, DvdTitle *dvdTitle, QString outputFile)
{
  int pgc_id, len, start_cell, cur_cell, ttn, pgn, next_cell, last_cell;
  unsigned int cur_pack;
  unsigned char data[1024 * DVD_VIDEO_LB_LEN];
  dvd_reader_t *dvd;          // Defines a DVD device
  dvd_file_t *title;
  ifo_handle_t *vmg_file;     // Defines an IFO file
  ifo_handle_t *vts_file;
  tt_srpt_t *tt_srpt;         // PartOfTitle Search Pointer Table.
  vts_ptt_srpt_t *vts_ptt_srpt;
  pgc_t *cur_pgc;

  FILE *file;
  file = fopen(outputFile, "w");
  if (file == NULL) printf("Can't open file.\n");

for (int j = 1; j <= dvdTitle->chapters().count(); j++) {

  int titleid = dvdTitle->id() - 1;
  int chapid = j - 1;
  int angle = dvdTitle->video()->angles() - 1;

  // Open DVD
  dvd = DVDOpen(dvdDevice);
  if(!dvd) {
//    QMessageBox::information("KopyRight - No DVD open", "Can't open disc " + dvdDevice);
    return -1;
  }

  // Load the video manager to find out the information about the titles on this disc.
  vmg_file = ifoOpen(dvd, 0);
  if(!vmg_file) {
//    QMessageBox::information(this, "KopyRight - No VMG info open", "Can't open VMG info.");
    DVDClose(dvd);
    return -1;
  }
  tt_srpt = vmg_file->tt_srpt;

  // Make sure our title number is valid.
  if(titleid < 0 || titleid >= tt_srpt->nr_of_srpts) {
//     QMessageBox::information(this, "KopyRight - Invalid Title", "Invalid title " + QString::number(titleid + 1));
    ifoClose(vmg_file);
    DVDClose(dvd);
    return -1;
  }

  // Make sure the number of chapters are valid for this title.
  if(chapid < 0 || chapid >= tt_srpt->title[titleid].nr_of_ptts ) {
//     QMessageBox::information(this, "KopyRight - Invalid Character", "Invalid number of chapters " + QString::number(chapid + 1));
    ifoClose(vmg_file);
    DVDClose(dvd);
    return -1;
  }

  // Make sure the angle number is valid for this title.
  if(angle < 0 || angle >= tt_srpt->title[titleid].nr_of_angles) {
//     QMessageBox::information(this, "KopyRight - Invalid Angle", "Invalid number of angles " + QString::number(angle + 1));
    ifoClose(vmg_file);
    DVDClose(dvd);
    return -1;
  }

  // Load the VTS information for the title set our title is in.
  vts_file = ifoOpen(dvd, tt_srpt->title[titleid].title_set_nr);
  if(!vts_file) {
//     QMessageBox::information(this, "KopyRight - Invalid IFO", "Can't open the title " + QString::number(tt_srpt->title[titleid].title_set_nr) + " info file.");
    ifoClose(vmg_file);
    DVDClose(dvd);
    return -1;
  }

  // Determine which program chain we want to watch. This is based on the chapter number.
  ttn = tt_srpt->title[titleid].vts_ttn;
  vts_ptt_srpt = vts_file->vts_ptt_srpt;
  pgc_id = vts_ptt_srpt->title[ttn - 1].ptt[chapid].pgcn;
  pgn = vts_ptt_srpt->title[ttn - 1].ptt[chapid].pgn;
  cur_pgc = vts_file->vts_pgcit->pgci_srp[pgc_id - 1].pgc;
  start_cell = cur_pgc->program_map[pgn - 1] - 1;

  //ThOe
  if (chapid + 1 == tt_srpt->title[titleid].nr_of_ptts) last_cell = cur_pgc->nr_of_cells;
  else last_cell = cur_pgc->program_map[(vts_ptt_srpt->title[ttn - 1].ptt[chapid + 1].pgn) - 1] - 1;

  // We've got enough info, time to open the title set data.
  title = DVDOpenFile( dvd, tt_srpt->title[ titleid ].title_set_nr, DVD_READ_TITLE_VOBS );
  if(!title) {
//     QMessageBox::information(this, "KopyRight - Invalid IFO", "Can't open title VOBS (VTS_" + QString::number(tt_srpt->title[titleid].title_set_nr) + "_1.VOB).");
    ifoClose(vts_file);
    ifoClose(vmg_file);
    DVDClose(dvd);
    return -1;
  }

  // Playback the cells for our chapter.
  next_cell = start_cell;
  for(cur_cell = start_cell; next_cell < last_cell;) {
    cur_cell = next_cell;

    // Check if we're entering an angle block.
    if(cur_pgc->cell_playback[cur_cell].block_type == BLOCK_TYPE_ANGLE_BLOCK) {
      int i;
      cur_cell += angle;
      for(i = 0;; ++i) {
        if(cur_pgc->cell_playback[ cur_cell + i ].block_mode == BLOCK_MODE_LAST_CELL) {
          next_cell = cur_cell + i + 1;
          break;
        }
      }
    } else next_cell = cur_cell + 1;

    // We loop until we're out of this cell.
    for(cur_pack = cur_pgc->cell_playback[cur_cell].first_sector; cur_pack < cur_pgc->cell_playback[cur_cell].last_sector;) {
      dsi_t dsi_pack;
      unsigned int next_vobu, next_ilvu_start, cur_output_size;

      // Read NAV packet.
      nav_retry:

      len = DVDReadBlocks(title, (int) cur_pack, 1, data);
      if( len != 1 ) {
//         QMessageBox::information(this, "KopyRight - Read Failed", "Read failed for block " + QString::number(cur_pack));
        ifoClose(vts_file);
        ifoClose(vmg_file);
        DVDCloseFile(title);
        DVDClose(dvd);
        return -1;
      }

      //assert( is_nav_pack( data ) );
      if(!is_nav_pack(data)) {
        cur_pack++;
        goto nav_retry;
      }

      #if 1
        // generate an MPEG2 program stream (including nav packs)
        if (fwrite( data, DVD_VIDEO_LB_LEN, 1, file ) != 1) {
          fprintf( stderr, "Write failed: %s\n", strerror (errno) );
          ifoClose( vts_file );
          ifoClose( vmg_file );
          DVDCloseFile( title );
          return -1;
        }
      #endif

      // Parse the contained dsi packet.
      navRead_DSI( &dsi_pack, &(data[ DSI_START_BYTE ]) );

      //assert( cur_pack == dsi_pack.dsi_gi.nv_pck_lbn );
      if(!( cur_pack == dsi_pack.dsi_gi.nv_pck_lbn)) {
        cur_output_size = 0;
        dsi_pack.vobu_sri.next_vobu = SRI_END_OF_CELL;
      }

      // Determine where we go next.  These values are the ones we mostly care about.
      next_ilvu_start = cur_pack + dsi_pack.sml_agli.data[ angle ].address;
      cur_output_size = dsi_pack.dsi_gi.vobu_ea;

      /**
       * If we're not at the end of this cell, we can determine the next
       * VOBU to display using the VOBU_SRI information section of the
       * DSI.  Using this value correctly follows the current angle,
       * avoiding the doubled scenes in The Matrix, and makes our life
       * really happy.
       *
       * Otherwise, we set our next address past the end of this cell to
       * force the code above to go to the next cell in the program.
       */
      if( dsi_pack.vobu_sri.next_vobu != SRI_END_OF_CELL ) {
        next_vobu = cur_pack + ( dsi_pack.vobu_sri.next_vobu & 0x7fffffff );
      } else {
        next_vobu = cur_pack + cur_output_size + 1;
      }

      assert( cur_output_size < 1024 );
      cur_pack++;

      // Read in and output cursize packs.
      len = DVDReadBlocks( title, (int)cur_pack, cur_output_size, data );
      if( len != (int) cur_output_size ) {
        fprintf( stderr, "Read failed for %d blocks at %d\n", cur_output_size, cur_pack );
        ifoClose( vts_file );
        ifoClose( vmg_file );
        DVDCloseFile( title );
        DVDClose( dvd );
        return -1;
      }

      if (fwrite( data, DVD_VIDEO_LB_LEN, cur_output_size, file ) != cur_output_size) {
        fprintf( stderr, "Write failed: %s\n", strerror (errno) );
        ifoClose( vts_file );
        ifoClose( vmg_file );
        DVDCloseFile( title );
        DVDClose( dvd );
        return -1;
      }

      cur_pack = next_vobu;
    }
  }
}
  ifoClose( vts_file );
  ifoClose( vmg_file );
  DVDCloseFile( title );
  DVDClose( dvd );
  return 0;

}
