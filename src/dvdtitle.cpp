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
#include "dvdtitle.h"

DvdTitle::DvdTitle(QListView *parent, int id) : QListViewItem(parent)
{
  m_id = id;
}

DvdTitle::DvdTitle(QListView *parent, int id, QString label1) : QListViewItem(parent, label1)
{
  m_id = id;
}

DvdTitle::DvdTitle(QListView *parent, int id, QString label1, QString label2) : QListViewItem(parent, label1, label2)
{
  m_id = id;
}

DvdTitle::DvdTitle(QListView *parent, int id, QString label1, QString label2, QString label3) : QListViewItem(parent, label1, label2, label3)
{
  m_id = id;
}

DvdTitle::DvdTitle(QListViewItem *parent, int id) : QListViewItem(parent)
{
  m_id = id;
}

DvdTitle::DvdTitle(QListViewItem *parent, int id, QString label1) : QListViewItem(parent, label1)
{
  m_id = id;
}

DvdTitle::DvdTitle(QListViewItem *parent, int id, QString label1, QString label2) : QListViewItem(parent, label1, label2)
{
  m_id = id;
}

DvdTitle::DvdTitle(QListViewItem *parent, int id, QString label1, QString label2, QString label3) : QListViewItem(parent, label1, label2, label3)
{
  m_id = id;
}

DvdTitle::DvdTitle(QListView *parent, DvdTitle *copy) : QListViewItem(parent)
{
  m_id = copy->id();
  m_audio = copy->audio();
  m_chapters = copy->chapters();
  m_cells = copy->cells();
  m_subPictures = copy->subPictures();
  m_video = new DvdVideo(copy->video());
  m_bytes = copy->bytes();
  m_audioBytes = copy->audioBytes();
  m_subPicturesBytes = copy->subpicturesBytes();
  m_requantFactor = copy->requantFactor();
}

DvdTitle::DvdTitle(QListViewItem *parent, DvdTitle *copy) : QListViewItem(parent)
{
  m_id = copy->id();
  m_audio = copy->audio();
  m_chapters = copy->chapters();
  m_cells = copy->cells();
  m_subPictures = copy->subPictures();
  m_video = new DvdVideo(copy->video());
  m_bytes = copy->bytes();
  m_audioBytes = copy->audioBytes();
  m_subPicturesBytes = copy->subpicturesBytes();
  m_requantFactor = copy->requantFactor();
}

DvdTitle::~DvdTitle()
{
}

// Input Methods

void DvdTitle::setId(int id)
{
  m_id = id;
}

void DvdTitle::setVideo(int id, int vts, int ttn, double fps, QString form, QString aR, QString wid, QString hei, QString df, int angles, int le, double by, bool se)
{
  m_video = new DvdVideo(id, vts, ttn, fps, form, aR, wid, hei, df, angles, le, by, se);
}

void DvdTitle::addAudio(int id, QString lc, QString la, QString fo, QString fr, QString qu, int ch, int ap, QString co, double by, bool se)
{
  m_audio.append(new DvdAudio(id, lc, la, fo, fr, qu, ch, ap, co, by, se));
}

void DvdTitle::addChapter(int id, int length, int startCell, double bytes, bool select)
{
  m_chapters.append(new DvdChapter(id, length, startCell, bytes, select));
}

void DvdTitle::addChapter(DvdChapter *chapter)
{
  m_chapters.append(chapter);
}

void DvdTitle::addCell(int id, int length, double bytes, bool select)
{
  m_cells.append(new DvdCell(id, length, bytes, select));
}

void DvdTitle::addSubpicture(int id, QString langCode, QString lang, QString cont, double bytes, bool select)
{
  m_subPictures.append(new DvdSubpicture(id, langCode, lang, cont, bytes, select));
}

void DvdTitle::setBytes(double bytes)
{
  m_bytes = bytes;
}

void DvdTitle::setAudioBytes(double bytes)
{
  m_audioBytes = bytes;
}

void DvdTitle::setSubpicturesBytes(double bytes)
{
  m_subPicturesBytes = bytes;
}

void DvdTitle::setRequantFactor(double requant)
{
  m_requantFactor = requant;
}

// Output Methods

int DvdTitle::id()
{
  return m_id;
}

int DvdTitle::rtti() const
{
  return 2;
}

DvdVideo * DvdTitle::video()
{
  return m_video;
}

QPtrList<DvdAudio> DvdTitle::audio()
{
  return m_audio;
}

QPtrList<DvdChapter> DvdTitle::chapters()
{
  return m_chapters;
}

QPtrList<DvdCell> DvdTitle::cells()
{
  return m_cells;
}

QPtrList<DvdSubpicture> DvdTitle::subPictures()
{
  return m_subPictures;
}

double DvdTitle::bytes()
{
  return m_bytes;
}

double DvdTitle::audioBytes()
{
  return m_audioBytes;
}

double DvdTitle::subpicturesBytes()
{
  return m_subPicturesBytes;
}

double DvdTitle::requantFactor()
{
  return m_requantFactor;
}
