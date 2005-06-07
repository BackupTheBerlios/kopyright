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
#ifndef DVDCELL_H
#define DVDCELL_H

#include <dvdstream.h>

/**
 * This is a extension of DvdStream that only add lenght atribute
 * and methods for manipulate.
 *
 * @short Dvd Cell
 * @author Javier Garrido Campoy
 */

class DvdCell : public DvdStream
{
Q_OBJECT
public:
  /**
   * Default Constructor
   *
   * @param parent = parent object for QObject constructor class
   * @param name = name of class
   */
  DvdCell(DvdStream *parent = 0, const char *name = 0);

  /**
   * Constructor for get all posibles properties in Cell Stream
   *
   * @param id = identifier
   * @param le = length
   * @param by = bytes
   * @param se = selected
   */
  DvdCell(int id, int le, double by, bool se);

  /**
   * Default destructor
   */
  ~DvdCell();

  /**
   * Specify length for cell like a integer
   *
   * @param le = length
   */
  void setLength(int le);

  /**
   * Specify length for cell like QString
   *
   * @param le = length
   */
  void setLength(QString le);

  /**
   * Get length of cell
   */
  int length();

  /**
   * Get length of cell formated as text
   */
  QString lengthFormated();

private:
  int cellLength;  // in miliseconds
};

#endif
