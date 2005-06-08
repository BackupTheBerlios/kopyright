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


#ifndef _KOPYRIGHTVIEW_H_
#define _KOPYRIGHTVIEW_H_

#include "kopyrightviewbase.h"
#include "dvdtitle.h"

#include <qwidget.h>
#include <kparts/part.h>
#include <kopyrightiface.h>

class QPainter;
class KURL;
class QFillStatus;

/**
 * This is the main view class for KopyRight.  Most of the non-menu,
 * non-toolbar, and non-statusbar (e.g., non frame) GUI code should go here.
 *
 * @short Main view
 * @author Javier Garrido Campoy <javiergarcam@gmail.com>
 * @version 0.1
 */
class KopyRightView : public KopyRightViewBase, public KopyRightIface
{
    Q_OBJECT
public:
  /**
   * Default constructor
   */
  KopyRightView(QWidget *parent);

  /**
   * Destructor
   */
  virtual ~KopyRightView();

  /**
   *
   */
  void setBytes(double bytes);

  /**
   *
   */
  double bytes();

  /**
   *
   */
//  double requant(DvdTitle *title);

  /**
   *
   */
  void updateSizes(QListViewItem *title);

signals:
  /**
   * Use this signal to change the content of the statusbar
   */
  void signalChangeStatusbar(const QString& text);

  /**
   * Use this signal to change the content of the caption
   */
  void signalChangeCaption(const QString& text);

public slots:
  virtual void editTitle(QListViewItem *stream);
  virtual void requantTitle(const QString &factor);
  virtual void viewTitle(QListViewItem *title);

private slots:
  void slotSetStatus(const QString& status);
  void slotSetTitle(const QString& title);

private:
//  KParts::ReadOnlyPart *m_html;
  double m_newStructSize;
  QFillStatus *m_fillStatus;
  
};

#endif // _KOPYRIGHTVIEW_H_
