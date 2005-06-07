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
#include "dvdchapter.h"

DvdChapter::DvdChapter(DvdStream *parent, const char *name) : DvdCell(parent, name)
{
}

DvdChapter::DvdChapter(int id, int le, int sc, double by, bool se) : DvdCell(id, le, by, se)
{
  chapterStartCell = sc;
}

DvdChapter::~DvdChapter()
{
}

void DvdChapter::setStartCell(int start)
{
  chapterStartCell = start;
}

int DvdChapter::startCell()
{
  return chapterStartCell;
}
