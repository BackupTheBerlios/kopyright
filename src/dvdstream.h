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
#ifndef DVDSTREAM_H
#define DVDSTREAM_H

#include <qobject.h>

#include "dvdvisualice.h"

/**
 * This is only a Wrapper Class that provides basic properties for Dvd Streams
 * like a identifier, size in bytes and the state of stream in new Dvd compilation.
 *
 * @short Dvd Stream
 * @author Javier Garrido Campoy
 */

class DvdStream : public QObject
{
Q_OBJECT
public:
  /**
   * Default constructor
   *
   * @param parent = QObject parent ot this object
   * @param name = name for object
   */
  DvdStream(QObject *parent = 0, const char *name = 0);

  /**
   * Constructor for DvdStream whitout QObject parent, only data
   *
   * @param id = numeric identifier of stream
   * @param bytes = size of stream in bytes
   * @param selected = show if the stream selected for new structure
   */
  DvdStream(int id, double bytes, bool select);

  /**
   * Default Destructor
   */
  ~DvdStream();

  /**
   * Set Id for Stream
   *
   * @param num = identifier
   */
  void setId(int id);

  /**
   * Set size for Stream
   *
   * @param bytes = size in bytes
   */
  void setBytes(double bytes);

  /**
   * Set state for Stream in new Dvd
   *
   * @param selec = state
   */
  void setSelected(bool selec);

  /**
   * Get Id of Stream
   */
  int id();

  /**
   * Get Id of Stream as Text Object
   */
  QString idAsText();

  /**
   * Get size of Stream
   */
  double bytes();

  /**
   * Get state of Stream in new Dvd
   */
  bool isSelected();

private:
  // Numeric identifier for Stream
  int streamId;

  // Size of Stream in Bytes
  double streamBytes;

  // Are Stream present in new Dvd Struct?
  bool streamSelected;
};

#endif
