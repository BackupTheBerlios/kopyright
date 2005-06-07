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


#ifndef _KOPYRIGHTPREF_H_
#define _KOPYRIGHTPREF_H_

#include <kdialogbase.h>
#include <qframe.h>

class KopyRightPrefPageOne;
class KopyRightPrefPageTwo;

class KopyRightPreferences : public KDialogBase
{
    Q_OBJECT
public:
    KopyRightPreferences();

private:
    KopyRightPrefPageOne *m_pageOne;
    KopyRightPrefPageTwo *m_pageTwo;
};

class KopyRightPrefPageOne : public QFrame
{
    Q_OBJECT
public:
    KopyRightPrefPageOne(QWidget *parent = 0);
};

class KopyRightPrefPageTwo : public QFrame
{
    Q_OBJECT
public:
    KopyRightPrefPageTwo(QWidget *parent = 0);
};

#endif // _KOPYRIGHTPREF_H_
