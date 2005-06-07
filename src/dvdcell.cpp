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
#include "dvdcell.h"

DvdCell::DvdCell(DvdStream *parent, const char *name) : DvdStream(parent, name)
{
}

DvdCell::DvdCell(int id, int le, double by, bool se) : DvdStream(id, by, se)
{
  cellLength = le;
}

DvdCell::~DvdCell()
{
}

void DvdCell::setLength(int le)
{
  cellLength = le;
}

void DvdCell::setLength(QString le)
{
  int hours, minutes, seconds;
  bool ok;

  QString analyze = le.mid(0, 2);
  hours = analyze.toInt(&ok, 10);

  analyze = le.mid(3, 2);
  minutes = analyze.toInt(&ok, 10);

  analyze = le.mid(6, 2);
  seconds = analyze.toInt(&ok, 10);

  seconds += (minutes * 60) + (hours * 120);

  cellLength = seconds * 1000;
}

int DvdCell::length()
{
  return cellLength;
}

QString DvdCell::lengthFormated()
{
  int hours, minutes, seconds = cellLength / 1000;
  QString len;

  minutes = abs(seconds / 60);
  seconds = seconds % 60;
  hours = abs(minutes / 60);
  minutes = minutes % 60;

  return len.sprintf("%02d:%02d:%02d", hours, minutes, seconds);
}
