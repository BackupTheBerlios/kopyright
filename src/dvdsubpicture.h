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
#ifndef DVDSUBPICTURE_H
#define DVDSUBPICTURE_H

#include "dvdstream.h"

/**
 * Extension of DvdStream that add all language and content of subpictures
 *
 * @short Dvd Subpicture
 * @author Javier Garrido Campoy
 */

class DvdSubpicture : public DvdStream
{
Q_OBJECT
public:
  /**
   * Default Constructor
   *
   * @param parent = parent object for QObject constructor class
   * @param name = name of object
   */
  DvdSubpicture(DvdStream *parent = 0, const char *name = 0);

  /**
   * Constructor for all posibles properties in Subpicture Stream
   *
   * @param id = identifier
   * @param lc = language code
   * @param la = language
   * @param co = content
   * @param by = bytes
   * @param se = selected
   */
  DvdSubpicture(int id, QString lc, QString la, QString co, double by, bool se);

  /**
   * Default Destructor
   */
  ~DvdSubpicture();

  // Input Methods

  /**
   * Set language code as text
   *
   * @param langCode = languge code as text
   */
  void setLanguageCode(QString langCode);

  /**
   * Set language name
   *
   * @param lang = language name
   */
  void setLanguage(QString lang);

  /**
   * Set content of subpicture stream
   *
   * @param cont = conten of subpicture
   */
  void setContent(QString cont);

  // Output Methods

  /**
   * Get language code
   */
  QString languageCode();

  /**
   * Get language name
   */
  QString language();

  /**
   * Get content of Subpicture
   */
  QString content();

private:
  QString langCode;
  QString lang;
  QString cont;
};

#endif
