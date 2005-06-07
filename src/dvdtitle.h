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
#ifndef DVDTITLE_H
#define DVDTITLE_H

#include "dvdaudio.h"
#include "dvdchapter.h"
#include "dvdcell.h"
#include "dvdsubpicture.h"
#include "dvdvideo.h"

#include <qlistview.h>
#include <qstringlist.h>
#include <qstring.h>
#include <qptrlist.h>
//#include <qobjectlist.h>

/**
 * This class inherit QListViewItem and add the properties
 * of a Dvd Title for QListViewItem capabilities.
 *
 * @short Dvd Title Class
 * @author Javier Garrido Campoy
 */
class DvdTitle : public QListViewItem
{
public:
  DvdTitle(QListView *parent, int id);
  DvdTitle(QListView *parent, int id, QString label1);
  DvdTitle(QListView *parent, int id, QString label1, QString label2);
  DvdTitle(QListView *parent, int id, QString label1, QString label2, QString label3);

  DvdTitle(QListViewItem *parent, int id);
  DvdTitle(QListViewItem *parent, int id, QString label1);
  DvdTitle(QListViewItem *parent, int id, QString label1, QString label2);
  DvdTitle(QListViewItem *parent, int id, QString label1, QString label2, QString label3);

  DvdTitle(QListView *parent, DvdTitle *copy);
  DvdTitle(QListViewItem *parent, DvdTitle *copy);

  ~DvdTitle();

  // Input Methods

  /**
   * This method only insert the appropiate values for this
   */
  void setId(int id);

  /**
   * This method only insert the appropiate values for this
   */
  void setVideo(int id, int vts, int ttn, double fps, QString form, QString aR, QString wid, QString hei, QString df, int angles, int le, double by, bool se);

  /**
   * Create a new DvdAudio object and insert this in m_audio list
   */
  void addAudio(int id, QString lc, QString la, QString fo, QString fr, QString qu, int ch, int ap, QString co, double by, bool select);

  /**
   * Create a new DvdChapter object and insert this in m_chapter list
   */
  void addChapter(int id, int length, int startCell, double bytes, bool select);

  /**
   * Insert a new DvdChapter object in m_chapter list that receive as parameter
   */
  void addChapter(DvdChapter *chapter);

  /**
   * Create a new DvdCell object and insert this in m_cell list
   */
  void addCell(int id, int length, double bytes, bool select);

  /**
   * Create a new DvdSubpicture object and insert this in m_subpictures list
   */
  void addSubpicture(int id, QString langCode, QString lang, QString cont, double bytes, bool select);

  /**
   * Set Title size in bytes
   */
  void setBytes(double bytes);

  /**
   *
   */
  void setAudioBytes(double bytes);

  /**
   *
   */
  void setSubpicturesBytes(double bytes);

  /**
   *
   */
  void setRequantFactor(double bytes);


  // Output Methods

  int id();

  /**
   * Return the identifier of this class.
   * QListViewItem set this to 0 | QCheckListViewItem set this to 1 | DvdTitle set this to 2
   */
  int rtti() const;

  /**
   *
   */
  DvdVideo *video();

  /**
   *
   */
  QPtrList<DvdAudio> audio();

  /**
   *
   */
  QPtrList<DvdChapter> chapters();

  /**
   *
   */
  QPtrList<DvdCell> cells();

  /**
   *
   */
  QPtrList<DvdSubpicture> subPictures();

  /**
   *
   */
  double bytes();

  /**
   *
   */
  double audioBytes();

  /**
   *
   */
  double subpicturesBytes();

  /**
   *
   */
  double requantFactor();

private:
  int m_id;

  DvdVideo *m_video;

  QPtrList<DvdAudio> m_audio;
  QPtrList<DvdChapter> m_chapters;
  QPtrList<DvdCell> m_cells;
  QPtrList<DvdSubpicture> m_subPictures;

  double m_audioBytes;
  double m_subPicturesBytes;
  double m_bytes;    // size of title in bytes (audio + video + subpictures)
  double m_requantFactor;
};

#endif
