/***************************************************************************
 *   Copyright (C) 2005 by Javier Garrido Campoy                           *
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
#include "dvdstream.h"

DvdStream::DvdStream(QObject *parent, const char *name) : QObject(parent, name)
{
}

DvdStream::DvdStream(int id, double bytes, bool select)
{
  streamId = id;
  streamBytes = bytes;
  streamSelected = select;
}

DvdStream::~DvdStream()
{
}

// Input Methods

void DvdStream::setId(int id)
{
  streamId = id;
}

void DvdStream::setBytes(double bytes)
{
  streamBytes = bytes;
}

void DvdStream::setSelected(bool selec)
{
  streamSelected = selec;
}

// Output Methods

int DvdStream::id()
{
  return streamId;
}

QString DvdStream::idAsText()
{
  return QString::number(streamId);
}

double DvdStream::bytes()
{
  return streamBytes;
}

bool DvdStream::isSelected()
{
  return streamSelected;
}

#include "dvdstream.moc"
