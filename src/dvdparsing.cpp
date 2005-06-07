/***************************************************************************
 *   Copyright (C) 2005 by Javier Garrido Campoy                           *
 *   javi@level13                                                          *
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

#include "dvdparsing.h"

/*
char* aspect_ratio[4] = {"4/3", "16/9", "\"?:?\"", "16/9"};
char* quantization[4] = {"16bit", "20bit", "24bit", "drc"};
char* mpeg_version[2] = {"mpeg1", "mpeg2"};

char* video_format[2] = {"NTSC", "PAL"};
char* video_height[4] = {"480", "576", "???", "576"};
char* video_width[4]  = {"720", "704", "352", "352"};
double frames_per_s[4] = {-1.0, 25.00, -1.0, 29.97};

char* audio_format[7] = {"ac3", "?", "mpeg1", "mpeg2", "lpcm ", "sdds ", "dts"};
char* audio_type[5]   = {"Undefined", "Normal", "Impaired", "Comments1", "Comments2"};
char* sample_freq[2]  = {"48000", "48000"};


char* permitted_df[4] = {"P&S + Letter", "Pan&Scan", "Letterbox", "?"};
char* subp_type[16]   = {"Undefined", "Normal", "Large", "Children", "reserved", "Normal_CC", "Large_CC", "Children_CC",
  "reserved", "Forced", "reserved", "reserved", "reserved", "Director", "Large_Director", "Children_Director"};
*/
//------------------------------------------------------------------------------------------------------------------------------

// From StreamAnalyze
unsigned int mpa_bitrate_index [2][4][16] = {
  { // MPEG audio V2
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,32,64,96,128,160,192,224,256,288,320,352,384,416,448,0},
    {0,32,48,56,64 ,80 ,96 ,112,128,160,192,224,256,320,384,0},
    {0,8 ,16,24,32 ,64 ,80 ,56 ,64 ,128,160,112,128,256,320,0}
  },
  { // MPEG audio V1
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,32,64,96,128,160,192,224,256,288,320,352,384,416,448,0},
    {0,32,48,56,64 ,80 ,96 ,112,128,160,192,224,256,320,384,0},
    {0,32,40,48,56 ,64 ,80 ,96 ,112,128,160,192,224,256,320,0}
  }
};

double dvd_size_bytes = 4700000000.;
//char *stream_type_names[6] = { "MPEG Video", "MPEG Audio", "AC3 Audio", "DTS Audio", "LPCM-Audio", "Subpicture" };
QString stream_type_names[6] = { "MPEG Video", "MPEG Audio", "AC3 Audio", "DTS Audio", "LPCM-Audio", "Subpicture" };

// bitrate tables stolen from mplex
unsigned int ac3_bitrate_index[32] = {32,40,48,56,64,80,96,112,128,160,192,224,256,320,384,448,512,576,640,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned int dts_bitrate_index[32] = {32,56,64,96,112,128,192,224,256,320,384,448,512,576,640,768,960,1024,1152,1280,1344,1408,1411,1472,1536,1920,2048,3072,3840,0,0,0};

// From lsdvd
QString aspect_ratio[4] = {"4/3", "16/9", "\"?:?\"", "16/9"};
QString quantization[4] = {"16bit", "20bit", "24bit", "drc"};
QString mpeg_version[2] = {"mpeg1", "mpeg2"};

QString video_format[2] = {"NTSC", "PAL"};
QString video_height[4] = {"480", "576", "???", "576"};
QString video_width[4]  = {"720", "704", "352", "352"};
double frames_per_s[4] = {-1.0, 25.00, -1.0, 29.97};

QString audio_format[7] = {"ac3", "?", "mpeg1", "mpeg2", "lpcm ", "sdds ", "dts"};
QString audio_type[5]   = {"Undefined", "Normal", "Impaired", "Comments1", "Comments2"};
QString sample_freq[2]  = {"48000", "48000"};

QString permitted_df[4] = {"P&S + Letter", "Pan&Scan", "Letterbox", "?"};
QString subp_type[16]   = {"Undefined", "Normal", "Large", "Children", "reserved", "Normal_CC", "Large_CC", "Children_CC",
  "reserved", "Forced", "reserved", "reserved", "reserved", "Director", "Large_Director", "Children_Director"};

//-----------------------------------------------------------------------------------------------------------------------------------
int dvdtime2msec(dvd_time_t *dt)
{
  double fps = frames_per_s[(dt->frame_u & 0xc0) >> 6];

  long ms  = (((dt->hour & 0xf0) >> 3) * 5 + (dt->hour & 0x0f)) * 3600000;
  ms += (((dt->minute & 0xf0) >> 3) * 5 + (dt->minute & 0x0f)) * 60000;
  ms += (((dt->second & 0xf0) >> 3) * 5 + (dt->second & 0x0f)) * 1000;
  if(fps > 0) ms += ((dt->frame_u & 0x30) >> 3) * 5 + (dt->frame_u & 0x0f) * 1000.0 / fps;

  return ms;
}
//-----------------------------------------------------------------------------------------------------------------------------------
int get_title_name(const char *dvd_device, char *title)
{
  FILE *filehandle = 0;
  int i;

  if (!(filehandle = fopen(dvd_device, "r"))) {
    fprintf(stderr, "Couldn't open %s for title\n", dvd_device);
    strcpy(title, "unknown");
    return -1;
  }

  if (fseek(filehandle, 32808, SEEK_SET)) {
    fclose(filehandle);
    fprintf(stderr, "Couldn't seek in %s for title\n", dvd_device);
    strcpy(title, "unknown");
    return -1;
  }

  if (32 != (i = fread(title, 1, 32, filehandle))) {
    fclose(filehandle);
    fprintf(stderr, "Couldn't read enough bytes for title.\n");
    strcpy(title, "unknown");
    return -1;
  }

  fclose (filehandle);

  title[32] = '\0';
  while(i-- > 2) if(title[i] == ' ') title[i] = '\0';
  return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------
char *lang_name(char* code)
{
  int k = 0;
  while (memcmp(language[k].code, code, 2) && language[k].name[0] ) { k++; }
  return language[k].name;
}
//-----------------------------------------------------------------------------------------------------------------------------------
int get_start_stop_blocks(dvd_reader_t *dvd, int title, uint8_t *ts_nr, int *chapstart, int *chapstop, uint32_t *startblock, uint32_t *lastblock, int* runtime)
{
  int ok = 0, titleid = title -1, ttn, pgc_id, pgn, start_cell, end_cell, i;
  ifo_handle_t *ifo_title, *ifo_main;
  vts_ptt_srpt_t *vts_ptt_srpt;
  tt_srpt_t *tt_srpt;
  pgc_t *pgc, *full_pgc;

  // Don't rip Title 0
  if(title == 0) {
    fprintf(stderr, "Invalid title number %d\n", title);
    return 0;
  }

  // Open main Ifo file
  ifo_main = ifoOpen(dvd, 0);
  if(!ifo_main) {
    fprintf(stderr, "Error opening main ifo file\n");
    return(0);
  }

  // Title number < Max Number of Titles ?
  if(titleid > ifo_main->tt_srpt->nr_of_srpts) {
    fprintf(stderr, "Invalid title number %d\n", title);
    return(0);
  }

  // Save Title set Number
  tt_srpt = ifo_main->tt_srpt;
  *ts_nr = tt_srpt->title[titleid].title_set_nr;

  // Open title Ifo file
  ifo_title = ifoOpen(dvd, tt_srpt->title[titleid].title_set_nr);
  if(!ifo_title) {
    fprintf(stderr, "Error opening ifo file for title %d\n", title);
    ifoClose(ifo_main);
    return(0);
  }

  // Get start/stop block for selected chapters
  if(ifo_title->vtsi_mat) {
    if(*chapstart == 0) *chapstart = 1;                                 // set first chapter if unset
    if(*chapstop == 0) *chapstop = tt_srpt->title[titleid].nr_of_ptts;  // set last chapter if unset

    ttn = tt_srpt->title[titleid].vts_ttn;                              // mostly stolen from transcode
    vts_ptt_srpt = ifo_title->vts_ptt_srpt;

    pgc_id = vts_ptt_srpt->title[ttn - 1].ptt[*chapstart-1].pgcn;
    pgn = vts_ptt_srpt->title[ttn - 1].ptt[*chapstart-1].pgn;
    pgc = ifo_title->vts_pgcit->pgci_srp[pgc_id - 1].pgc;
    full_pgc = pgc;

    start_cell = pgc->program_map[pgn - 1] - 1;
    *startblock = pgc->cell_playback[start_cell].first_sector;          // first block of start cell

    if(*chapstop == tt_srpt->title[titleid].nr_of_ptts) {               // if stop chapter = last chapter
      end_cell = pgc->nr_of_cells -1;
      *lastblock = pgc->cell_playback[end_cell].last_sector;
    } else {
      pgc_id = vts_ptt_srpt->title[ttn - 1].ptt[*chapstop].pgcn;
      pgn = vts_ptt_srpt->title[ttn - 1].ptt[*chapstop].pgn;
      pgc = ifo_title->vts_pgcit->pgci_srp[pgc_id - 1].pgc;
      end_cell = pgc->program_map[pgn - 1] - 2;
      *lastblock = pgc->cell_playback[end_cell].last_sector;            // last block of end cell
    }

    if((*chapstart == 1) && (*chapstop == tt_srpt->title[ titleid ].nr_of_ptts)) {   // all chapters selected ?
         *runtime = dvdtime2msec(&full_pgc->playback_time) / 1000;                   // runtime = movie runtime
    } else {                                                                         // only some chapters selected ?
      *runtime = 0;
      for( i=start_cell ; i<=end_cell ; i++ ) { 
        *runtime += dvdtime2msec(&pgc->cell_playback[i].playback_time) / 1000;       // sum runtime of selected cells
      }
    }

    ok = 1;
  }

  ifoClose(ifo_title);
  ifoClose(ifo_main);

  return(ok);
}
//-----------------------------------------------------------------------------------------------------------------------------------
int probe_dvd(dvd_file_t *vob, struct streamdata **list, uint32_t start_block, uint32_t stop_block)
{
  int x, y, z, found, count = 0;
  unsigned char buffer[DVD_VIDEO_LB_LEN], *searchptr;
  uint8_t packet_type;
  uint32_t maxblocks = stop_block, i;
  struct streamdata *new_data;

  if((stop_block-start_block) > PROBEBLOCKS) maxblocks = start_block + PROBEBLOCKS;

  for(i = start_block; i < maxblocks; i++) {            // read 10mb from dvd
    if(!DVDReadBlocks(vob, i, 1, buffer)) {
      fprintf(stderr, "ERROR probing for streams\n");
      exit(1);
    }

    found = 0;
    // Get stream id
    packet_type = buffer[17];
    // if private stream get sub-id
    if(packet_type == 0xBD) packet_type = buffer[23 + buffer[22]];

    // Check for video, audio, subpicture
    if( ((packet_type >= 0xE0) && (packet_type <= 0xEF))
     || ((packet_type >= 0x20) && (packet_type <= 0x3F))
     || ((packet_type >= 0xC0) && (packet_type <= 0xCF))
     || ((packet_type >= 0xA0) && (packet_type <= 0xAF))
     || ((packet_type >= 0x80) && (packet_type <= 0x8F))) {

      // Scan if we already know this stream
      for(x = 0; x < count; x++) {
        // Makes sure, we only get headers at stream start
        if(list[x]->id == packet_type) {
          found = 1;
          break;
        }
      }
      // Unknown stream, add to list
      if(!found) {
        new_data = (streamdata *)malloc(sizeof(struct streamdata));
        if(new_data == NULL) {
          fprintf(stderr, "Out of memory\n");
          exit(1);
        }

        new_data->id = packet_type;

        if((packet_type >= 0xE0) && (packet_type <= 0xEF)) {
          new_data->type = MPEG_VIDEO;
          new_data->bitrate = 0;                                                    // dummy entry for video
        } else if((packet_type >= 0xC0) && (packet_type <= 0xCF)) {
          new_data->type = MPEG_AUDIO;
          new_data->bitrate = 0;
          searchptr = buffer +17;
          searchptr = (unsigned char*)memchr(searchptr, 0xFF, searchptr - buffer);  // search for 1st byte of MPA Syncword 
          while(searchptr != NULL) {
            if(searchptr <= (buffer + 2045)) {                                      // check for search hit at end of buffer
              if((searchptr[1] & 0xF0) == 0xF0) {                                   // if next 4 bits set, we found the sync word
                x = (searchptr[1] & 0x08) >> 3;                                     // version id
                y = (searchptr[1] & 0x06) >> 1;                                     // layer code
                z = (searchptr[2] & 0xF0) >> 4;                                     // bitrate code
                new_data->bitrate = mpa_bitrate_index[x][y][z] * 1024;
                break;
              }
            }
            searchptr++;
            searchptr = (unsigned char*)memchr(searchptr, 0xFF, searchptr-buffer);
          }
          if(new_data->bitrate == 0) {                                              // no syncword found, try next packet
            fprintf(stderr, "unable to get MPEG-Audio bitrate\n");
            continue;
          }
        } else if((packet_type >= 0x80) && (packet_type <= 0x87)) {
          y = buffer[22] + buffer[buffer[22] + 26] + (unsigned char)26;             // calc start offsets
          if((buffer[y] != 0x0B) || (buffer[y + 1] != 0x77)) continue;              // check for AC3 Syncword
          new_data->type = AC3_AUDIO;
          x = buffer[y + 4] & 0x3F;                                                 // 6 lowest bits
          new_data->bitrate = ac3_bitrate_index[ x>>1 ] * 1024;                     // shift index >> 1 (same as /2 )
        } else if((packet_type >= 0x88) && (packet_type <= 0x8F)) {
          new_data->type = DTS_AUDIO;
          x = (buffer[buffer[22] + 35] & 0x3) << 3;                                 // 3 lowest bits of byte
          x = x | ((buffer[buffer[22] + 36] & 0xE0) >> 5);                          // 2 highest bits of next byte
          new_data->bitrate = dts_bitrate_index[x] * 1000;                          // kbit * 1000 fits better to filesize, so what ...
        } else if((packet_type >= 0xA0) && (packet_type <= 0xAF)) {
          new_data->type = LPCM_AUDIO;
          new_data->bitrate = 2000000;                                              // FIXME - hard coded, no detection yet
        } else if((packet_type >= 0x20) && (packet_type <= 0x3F)) {
          new_data->type = SUBPICTURE;
          new_data->bitrate = 0;                                                    // dummy entry for subpicture
        }

        list[count] = new_data;
        count++;
      }
    }
  }

  return(count);
}
//-----------------------------------------------------------------------------------------------------------------------------------
int allDvdInfo(QString dvdDevice, QListView *list)
{
  QListViewItem *element, *currentTitleChapters, *menus, *mainMovie, *extras;
  DvdTitle *currentTitle;
  QPixmap icon;

  // LSDVD
  char lang_code[3];
  char title[33];
  int titles, cell, vts_ttn, title_set_nr;

  struct stat m_dvdStat;
  ifo_handle_t *m_ifoZero, **m_ifo;      // define an IFO file
  dvd_reader_t *m_dvd;
  vtsi_mat_t *vtsi_mat;                  // Video Title Set Information Management Table
  vmgi_mat_t *vmgi_mat;
  audio_attr_t *audio_attr;
  video_attr_t *video_attr;
  subp_attr_t *subp_attr;
  pgcit_t *vts_pgcit;                    // Video Title Set Program Chain Information Table
  pgc_t *pgc;

  // StreamAnalyze
  struct streamdata *streamdata_list[MAXSTREAMS];
  dvd_file_t *vob;
  int stream_count = 0;
  int stream_audio = 0;
  uint8_t ts_nr;
  double video_size, overhead_size, temp;//nonvideo_size = 0, temp; //selected_size = 0, temp;

  // Check DVD status
  int ret = stat(dvdDevice, &m_dvdStat);
  if (ret < 0) {
    list->clear();
    element = new QListViewItem(list, "Can't find device " + dvdDevice);
    return -1;
  }

  // Open DVD
  m_dvd = DVDOpen(dvdDevice);
  if(!m_dvd) {
    list->clear();
    element = new QListViewItem(list, "Can't open disc " + dvdDevice);
    return -1;
  }

  // Open main IFO
  m_ifoZero = ifoOpen(m_dvd, 0);
  if (!m_ifoZero) {
    list->clear();
    element = new QListViewItem(list, "Can't open main ifo!");
    return -1;
  }

  // Open the rest of ifo's files
  m_ifo = (ifo_handle_t **)malloc((m_ifoZero->vts_atrt->nr_of_vtss + 1) * sizeof(ifo_handle_t *));
  for (int i = 1; i <= m_ifoZero->vts_atrt->nr_of_vtss; i++) {
    m_ifo[i] = ifoOpen(m_dvd, i);
    if (!m_ifo[i]) {
      element = new QListViewItem(list, "Can't open ifo!" + QString::number(i));
      return -1;
    }
  }

  titles = m_ifoZero->tt_srpt->nr_of_srpts;
  get_title_name(dvdDevice, title);
  vmgi_mat = m_ifoZero->vmgi_mat;

  // First Node with title name
  icon.load("/home/javi/Desarrollo/maker/images/dvd_unmount.png");
  element = new QListViewItem(list, title);
  element->setPixmap(0, icon);

  // Main Nodes of DVD Structure
  icon.load("/home/javi/Desarrollo/maker/images/folder_blue.png");
  menus = new QListViewItem(list->firstChild(), "Menus");
  menus->setPixmap(0, icon);
  mainMovie = new QListViewItem(list->firstChild(), "Main Movie");
  mainMovie->setPixmap(0, icon);
  extras = new QListViewItem(list->firstChild(), "Extras");
  extras->setPixmap(0, icon);

  for (int j = 0; j < titles; j++) {
    if (m_ifo[m_ifoZero->tt_srpt->title[j].title_set_nr]->vtsi_mat) {
      vtsi_mat = m_ifo[m_ifoZero->tt_srpt->title[j].title_set_nr]->vtsi_mat;
      vts_pgcit = m_ifo[m_ifoZero->tt_srpt->title[j].title_set_nr]->vts_pgcit;
      video_attr = &vtsi_mat->vts_video_attr;
      vts_ttn = m_ifoZero->tt_srpt->title[j].vts_ttn;
      vmgi_mat = m_ifoZero->vmgi_mat;
      title_set_nr = m_ifoZero->tt_srpt->title[j].title_set_nr;
      pgc = vts_pgcit->pgci_srp[m_ifo[title_set_nr]->vts_ptt_srpt->title[vts_ttn - 1].ptt[0].pgcn - 1].pgc;

      /////////////////////////////////////////////////// CALCULATE STREAMS SIZES ///////////////////////////////////////////////////
      int opt_chapter_start = 0, opt_chapter_stop = 0, runtime;
      uint32_t start_block = 0, stop_block = 0;
      double nonvideo_size = 0;

      if(get_start_stop_blocks(m_dvd, j + 1, &ts_nr, &opt_chapter_start, &opt_chapter_stop, &start_block, &stop_block, &runtime) ) {
        vob = DVDOpenFile(m_dvd, ts_nr, DVD_READ_TITLE_VOBS);
        if(!vob) {
          element = new QListViewItem(list, "Can't open vobs for title " + QString::number(ts_nr));
          DVDClose(m_dvd);
          exit(1);
        }

        stream_count = probe_dvd(vob, streamdata_list, start_block, stop_block); // probe some blocks to find all streams
        DVDCloseFile(vob);

        for(int i = 0; i < stream_count; i++) {
          switch(streamdata_list[i]->type) {
            case MPEG_VIDEO : streamdata_list[i]->size = 0; break;
            case SUBPICTURE : streamdata_list[i]->size = (double)(stop_block-start_block) * 2048 / 1450; break;
            case LPCM_AUDIO :
            case MPEG_AUDIO :
            case AC3_AUDIO  :
            case DTS_AUDIO  : streamdata_list[i]->size = streamdata_list[i]->bitrate / 8 * runtime; break;
          }
          nonvideo_size += streamdata_list[i]->size;
        }

        overhead_size = (stop_block-start_block) * 2048 / 50;                    // rough guess about space wasted by padding, pack headers, ...
        nonvideo_size += overhead_size;                                          // sum of all non video stuff
        video_size = ((double)(stop_block-start_block) * 2048) - nonvideo_size;  // video size = vob size - non video stuff

        for(int i = 0; i < stream_count; i++) {
          if(streamdata_list[i]->type == MPEG_VIDEO) streamdata_list[i]->size = video_size;  // add video size
        }
      }

      /////////////////////////////////////////////////////////// TITLE ///////////////////////////////////////////////////////////
      icon.load("/home/javi/Desarrollo/maker/images/display.png");
      if ((pgc->nr_of_programs > 1) && (dvdtime2msec(&pgc->playback_time) > 500000)) currentTitle = new DvdTitle(mainMovie, j + 1);
      else currentTitle = new DvdTitle(extras, j + 1);

      currentTitle->setVideo(j + 1, m_ifoZero->tt_srpt->title[j].title_set_nr, m_ifoZero->tt_srpt->title[j].vts_ttn,
        frames_per_s[(pgc->playback_time.frame_u & 0xc0) >> 6], video_format[video_attr->video_format], aspect_ratio[video_attr->display_aspect_ratio],
        video_width[video_attr->picture_size], video_height[video_attr->video_format], permitted_df[video_attr->permitted_df],
        m_ifoZero->tt_srpt->title[j].nr_of_angles, dvdtime2msec(&pgc->playback_time), video_size, true);

      currentTitle->setBytes((double)(stop_block - start_block) * 2048);

      currentTitle->setPixmap(0, icon);
      currentTitle->setText(0, "Title: " + QString::number(j + 1));
      currentTitle->setText(1, ((DvdVideo*)currentTitle->video())->lengthFormated());
      currentTitle->setText(2, visualiceSize(currentTitle->bytes()));
      currentTitle->setAudioBytes(0);
      currentTitle->setSubpicturesBytes(0);

      stream_audio = 0;

      /////////////////////////////////////////////////////////// AUDIO ///////////////////////////////////////////////////////////
      for (int i = 0; i < vtsi_mat->nr_of_vts_audio_streams; i++) {
        audio_attr = &vtsi_mat->vts_audio_attr[i];
        sprintf(lang_code, "%c%c", audio_attr->lang_code>>8, audio_attr->lang_code & 0xff);
        if (!lang_code[0]) {
          lang_code[0] = 'x';
          lang_code[1] = 'x';
        }

        if((i + 1 < stream_count) && (streamdata_list[i + 1]->type != MPEG_VIDEO) && (streamdata_list[i + 1]->type != SUBPICTURE)) temp = streamdata_list[i + 1]->size;
        else temp = 0;

        // Use only "i" because "tcextract" index audio streams starting at 0
        currentTitle->addAudio(i, lang_code, lang_name(lang_code), audio_format[audio_attr->audio_format], sample_freq[audio_attr->sample_frequency],
          quantization[audio_attr->quantization], audio_attr->channels + 1, audio_attr->application_mode, audio_type[audio_attr->lang_extension], temp, true);

        currentTitle->setAudioBytes(currentTitle->audioBytes() + temp);
        stream_audio += 1;
      }

      /////////////////////////////////////////////////////////// CHAPTERS ///////////////////////////////////////////////////////////
      icon.load("/home/javi/Desarrollo/maker/images/kaboodle.png");
      currentTitleChapters = new QListViewItem(currentTitle, "Chapters");
      currentTitleChapters->setPixmap(0, icon);

      cell = 0;
      for (int i = 0; i < pgc->nr_of_programs; i++) {
        int second = 0, minute = 0, hour = 0, tmp, next = pgc->program_map[i + 1];
        char hex[2];
        if (i == pgc->nr_of_programs - 1) next = pgc->nr_of_cells + 1;
        while (cell < next - 1) {
          sprintf(hex, "%02x", pgc->cell_playback[cell].playback_time.second);
          tmp = second + atoi(hex);
          minute = minute + (tmp / 60);
          second = tmp % 60;
          sprintf(hex, "%02x", pgc->cell_playback[cell].playback_time.minute);
          tmp = minute + atoi(hex);
          hour = hour + (tmp / 60);
          minute = tmp % 60;
          cell++;
        }

        DvdChapter *chap = new DvdChapter(i + 1, (((hour * 120) + (minute * 60) + second) * 1000), pgc->program_map[i], 0, true);
        currentTitle->addChapter(chap);

        element = new QListViewItem(currentTitleChapters, "Chapter: " + QString::number(i + 1) + ", Start Cell: " + QString::number(pgc->program_map[i]));
        element->setText(1, chap->lengthFormated());
      }

      /////////////////////////////////////////////////////////// CELLS ///////////////////////////////////////////////////////////
      for (int i = 0; i < pgc->nr_of_cells; i++) {
//        currentTitle->addCell(i + 1, QString::number(pgc->cell_playback[i].playback_time.hour) + ":" +  QString::number(pgc->cell_playback[i].playback_time.minute)
//          + ":" +  QString::number(pgc->cell_playback[i].playback_time.second), 0, true);

        currentTitle->addCell(i + 1, dvdtime2msec(&pgc->cell_playback[i].playback_time), 0, true);
      }

      /////////////////////////////////////////////////////////// SUBPICTURES ///////////////////////////////////////////////////////////
      for (int i = 0; i < vtsi_mat->nr_of_vts_subp_streams; i++) {
        subp_attr = &vtsi_mat->vts_subp_attr[i];
        sprintf(lang_code, "%c%c", subp_attr->lang_code>>8, subp_attr->lang_code & 0xff);
        if (!lang_code[0]) {
          lang_code[0] = 'x';
          lang_code[1] = 'x';
        }

//        if((i + 1 < stream_count) && (streamdata_list[stream_count - i]->type == SUBPICTURE)) temp = streamdata_list[stream_count - (i + 1)]->size;
        if((stream_audio + i + 1 < stream_count) && (streamdata_list[stream_audio + i + 1]->type == SUBPICTURE)) temp = streamdata_list[stream_audio + i + 1]->size;
        else temp = 0;

        currentTitle->addSubpicture(i, lang_code, lang_name(lang_code), subp_type[subp_attr->lang_extension], temp, true);

        currentTitle->setSubpicturesBytes(currentTitle->subpicturesBytes() + temp);
      }
    }
  }

  // Close all used files
  for (int i = 1; i <= m_ifoZero->vts_atrt->nr_of_vtss; i++) {
    ifoClose(m_ifo[i]);
  }
  ifoClose(m_ifoZero);
  DVDClose(m_dvd);

  return 0;
}
