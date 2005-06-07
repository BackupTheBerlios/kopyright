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
#ifndef DVDAUDIO_H
#define DVDAUDIO_H

#include "dvdsubpicture.h"

/**
 * This is a Wrapper Class that provide audio properties,
 * size of this and set if is selected for new Dvd or not.
 *
 * @short Audio Properties
 * @author Javier Garrido Campoy
 */

class DvdAudio : public DvdSubpicture
{
Q_OBJECT
public:
  /**
   * Default Constructor
   *
   * @param parent = parent object for QObject constructor class
   * @param name = name of object
   */
  DvdAudio(DvdSubpicture *parent = 0, const char *name = 0);

  /**
   * Constructor for get all properties posibles in Audio Stream
   *
   * @param id = identifier
   * @param lc = language code
   * @param la = language
   * @param fo = format
   * @param fr = frequency
   * @param qu = quantization
   * @param ch = channels
   * @param ap = application mode
   * @param co = content
   * @param by = bytes
   * @param se = selected
   */
  DvdAudio(int id, QString lc, QString la, QString fo, QString fr, QString qu, int ch, int ap, QString co, double by, bool se);

  /**
   * Default Destructor
   */
  ~DvdAudio();

  // Input Methods

  /**
   * Set audio format as text
   *
   * @param format = format
   */
  void setFormat(QString format);

  /**
   * Set audio frequency
   *
   * @param freq = frequency
   */
  void setFrequency(QString freq);

  /**
   * Set quantization factor
   *
   * @param quant = quantization factor
   */
  void setQuantization(QString quant);

  /**
   * Set number of audio channels
   *
   * @param chan = audio channels
   */
  void setChannels(int chan);

  /**
   * Set application mode
   *
   * @param ap = application mode
   */
  void setApplicationMode(int ap);

  // Output Methods

  /**
   * Get audio format
   */
  QString format();

  /**
   * Get audio frequency
   */
  QString frequency();

  /**
   * Get quantization factor
   */
  QString quantization();

  /**
   * Get number of channels
   */
  int channels();

  /**
   * Get application mode
   */
  int applicationMode();

private:
  QString audioFormat;
  QString audioFrequency;
  QString audioQuantization;
  int audioChannels;
  int audioApplicationMode;
};

#endif
