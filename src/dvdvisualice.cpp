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

#include "dvdvisualice.h"

QString visualiceSize(double bytes)
{
  QString siz;
  double parcial = bytes;
  int scale = 0;

  while (parcial > 1024) {
    parcial /= 1024;
    scale++;
  }

  switch(scale) {
    case 1: return siz.sprintf("%2.2f", parcial) + " Kb";
    case 2: return siz.sprintf("%2.2f", parcial) + " Mb";
    case 3: return siz.sprintf("%2.2f", parcial) + " Gb";
  }

  return siz.sprintf("%2.2f", bytes) + " b";
}