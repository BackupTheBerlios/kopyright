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
#include "dvdvideo.h"

DvdVideo::DvdVideo(DvdCell *parent, const char *name) : DvdCell(parent, name)
{
}

DvdVideo::DvdVideo(int id, int vts, int ttn, double fps, QString form, QString aR, QString wid, QString hei, QString df, int angles, int le, double by, bool se)
 : DvdCell(id, le, by, se)
{
  videoVTS = vts;
  videoTTN = ttn;
  videoFPS = fps;
  videoFormat = form;
  videoAspectRatio = aR;
  videoWidth = wid;
  videoHeight = hei;
  videoDF = df;
  videoAngles = angles;
}

DvdVideo::DvdVideo(DvdVideo *copy)
{
  this->setId(copy->id());
  videoVTS = copy->vts();
  videoTTN = copy->ttn();
  videoFPS = copy->fps();
  videoFormat = copy->format();
  videoAspectRatio = copy->aspectRatio();
  videoWidth = copy->width();
  videoHeight = copy->height();
  videoDF = copy->df();
  videoAngles = copy->angles();
  this->setLength(copy->length());
  this->setBytes(copy->bytes());
  this->setSelected(copy->isSelected());
}

DvdVideo::~DvdVideo()
{
}

// Output methods

int DvdVideo::vts()
{
  return videoVTS;
}

int DvdVideo::ttn()
{
  return videoTTN;
}

double DvdVideo::fps()
{
  return videoFPS;
}

QString DvdVideo::format()
{
  return videoFormat;
}

QString DvdVideo::aspectRatio()
{
  return videoAspectRatio;
}

QString DvdVideo::width()
{
  return videoWidth;
}

QString DvdVideo::height()
{
  return videoHeight;
}

QString DvdVideo::df()
{
  return videoDF;
}

int DvdVideo::angles()
{
  return videoAngles;
}
