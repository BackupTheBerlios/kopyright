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
#ifndef DVDCHAPTER_H
#define DVDCHAPTER_H

#include <dvdcell.h>

/**
 * This is a extension of DvdStream that only add the first cell of a title
 *
 * @short Dvd Chapter
 * @author Javier Garrido Campoy
 */

class DvdChapter : public DvdCell
{
public:
  /**
   * Default Constructor
   *
   * @param parent = parent object for QObject constructor class
   * @param name = name of class
   */
  DvdChapter(DvdStream *parent = 0, const char *name = 0);

  /**
   * Constructor for get all posibles properties in Chapter Stream
   *
   * @param id = identifier
   * @param le = length
   * @param sc = start cell
   * @param by = bytes
   * @param se = selected
   */
  DvdChapter(int id, int le, int sc, double by, bool se);

  /**
   * Default Destructor
   */
  ~DvdChapter();

  /**
   * Set the first cell of the chapter
   *
   * @param start = first cell
   */
  void setStartCell(int start);

  /**
   * Get the first cell for chapter
   */
  int startCell();

private:
  int chapterStartCell;
};

#endif
