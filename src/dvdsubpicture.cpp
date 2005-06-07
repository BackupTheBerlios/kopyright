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
#include "dvdsubpicture.h"

DvdSubpicture::DvdSubpicture(DvdStream *parent, const char *name) : DvdStream(parent, name)
{
}

DvdSubpicture::DvdSubpicture(int id, QString lc, QString la, QString co, double by, bool se) : DvdStream(id, by, se)
{
  langCode = lc;
  lang = la;
  cont = co;
}

DvdSubpicture::~DvdSubpicture()
{
}

// Input Methods

void DvdSubpicture::setLanguageCode(QString langCode)
{
  langCode = langCode;
}

void DvdSubpicture::setLanguage(QString lang)
{
  lang = lang;
}

void DvdSubpicture::setContent(QString cont)
{
  cont = cont;
}

// Output Methods

QString DvdSubpicture::languageCode()
{
  return langCode;
}

QString DvdSubpicture::language()
{
  return lang;
}

QString DvdSubpicture::content()
{
  return cont;
}

#include "dvdsubpicture.moc"
