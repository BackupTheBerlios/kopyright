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
#ifndef DVDVIDEO_H
#define DVDVIDEO_H

#include <dvdcell.h>

/**
 * This is a Wrapper Class that provide audio properties,
 * size of this and set if is selected for new Dvd or not.
 *
 * @short Audio Properties
 * @author Javier Garrido Campoy
 */

class DvdVideo : public DvdCell
{
Q_OBJECT
public:
  /**
   * Default Constructor
   *
   * @param parent = parent object for QObject constructor class
   * @param name = name of object
   */
  DvdVideo(DvdCell *parent = 0, const char *name = 0);

  /**
   * Constructor for get all properties posibles in Audio Stream
   *
   * @param id = identifier
   * @param vts = number of VTS file
   * @param ttn = 
   * @param fps = frames per second
   * @param form = format
   * @param aR = aspect ratio
   * @param wid = width
   * @param hei = heigth
   * @param df = 
   * @param angles = number of angles
   * @param le = length
   * @param by = size in bytes
   * @param se = selected
   */
  DvdVideo(int id, int vts, int ttn, double fps, QString form, QString aR, QString wid, QString hei, QString df, int angles, int le, double by, bool se);

  /**
   * Constructor for get all properties posibles in Audio Stream
   *
   * @param copy = copy
   */
  DvdVideo(DvdVideo *copy);

  /**
   * Default Destructor
   */
  ~DvdVideo();

  // Output Methods

  int vts();
  int ttn();
  double fps();
  QString format();
  QString aspectRatio();
  QString width();
  QString height();
  QString df();
  int angles();

private:
  int videoVTS ;             // number of VTS
  int videoTTN;
  double videoFPS;           // frames per second
  QString videoFormat;       // video format
  QString videoAspectRatio;  // aspect ratio
  QString videoWidth;        // width of image
  QString videoHeight;       // height of image
  QString videoDF;
  int videoAngles;           // number of angles
};

#endif
