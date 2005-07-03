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


#include "pref.h"

#include <klocale.h>

#include <qlayout.h>
#include <qlabel.h>
#include <qlineedit.h>

//------------------------------------------------------------------------------------------------------------------------------------------

KopyRightPreferences::KopyRightPreferences() : KDialogBase(TreeList, i18n("KopyRight Preferences"), Help|Default|Ok|Apply|Cancel, Ok)
{
  // this is the base class for your preferences dialog.  it is now a Treelist dialog.. but
  // there are a number of other possibilities (including Tab, Swallow, and just Plain)
  QFrame *frame;
  frame = addPage(i18n("Paths"), i18n("Paths for DVD and work space directories"));
  m_pageOne = new KopyRightPrefPageOne(frame);

//  frame = addPage(i18n("Second Page"), i18n("Page Two Options"));
//  m_pageTwo = new KopyRightPrefPageTwo(frame);
}

void KopyRightPreferences::setDevice(QString device)
{
  m_pageOne->setDvd(device);
}

void KopyRightPreferences::setWorkSpace(QString workSpace)
{
  m_pageOne->setDir(workSpace);
}

QString KopyRightPreferences::device()
{
  return m_pageOne->dvd();
}

QString KopyRightPreferences::workSpace()
{
  return m_pageOne->dir();
}

//------------------------------------------------------------------------------------------------------------------------------------------

KopyRightPrefPageOne::KopyRightPrefPageOne(QWidget *parent) : QFrame(parent)
{
  QVBoxLayout *vertical = new QVBoxLayout(parent);

  vertical->addWidget(new QLabel(i18n("DVD Device"), parent));
  m_dvdInput = new QLineEdit(parent);
  vertical->addWidget(m_dvdInput);
  vertical->addItem( new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum ));

  vertical->addWidget(new QLabel(i18n("Output Directory"), parent));
  m_workSpace = new QLineEdit(parent);
  vertical->addWidget(m_workSpace);
  vertical->addItem( new QSpacerItem( 151, 20, QSizePolicy::Minimum, QSizePolicy::Expanding ));

  clearWState( WState_Polished );
}

void KopyRightPrefPageOne::setDvd(QString dvd)
{
  m_dvdInput->setText(dvd);
}

void KopyRightPrefPageOne::setDir(QString dir)
{
  m_workSpace->setText(dir);
}

QString KopyRightPrefPageOne::dvd()
{
  return m_dvdInput->text();
}

QString KopyRightPrefPageOne::dir()
{
  return m_workSpace->text();
}

//------------------------------------------------------------------------------------------------------------------------------------------
/*
KopyRightPrefPageTwo::KopyRightPrefPageTwo(QWidget *parent) : QFrame(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setAutoAdd(true);

    new QLabel(i18n("Add something here"), this);
}
*/
//------------------------------------------------------------------------------------------------------------------------------------------

#include "pref.moc"
