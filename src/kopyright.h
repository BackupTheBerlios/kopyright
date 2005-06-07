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


#ifndef _KOPYRIGHT_H_
#define _KOPYRIGHT_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kapplication.h>
#include <kmainwindow.h>
#include <kaction.h>

#include "kopyrightview.h"

#include <qprocess.h>
#include <qfile.h>
#include <qdatastream.h>
#include <qprocess.h>

/**
 * This class serves as the main window for KopyRight.
 * It handles the menus, toolbars, and status bars.
 *
 * @short Main window class
 * @author Javier Garrido Campoy <javiergarcam@gmail.com>
 * @version 0.1
 */
class KopyRight : public KMainWindow
{
    Q_OBJECT
public:
  /**
   * Default Constructor
   */
  KopyRight();

  /**
   * Default Destructor
   */
  virtual ~KopyRight();

  /**
   * Get a list with file names that compose the title
   */
  QStringList titleFiles(QString dvdDir, int title);

  void prepareProcess(QProcess *process, QString command);

protected:
  /**
   * Overridden virtuals for Qt drag 'n drop (XDND)
   */
  virtual void dragEnterEvent(QDragEnterEvent *event);
  virtual void dropEvent(QDropEvent *event);

protected:
  /**
   * This function is called when it is time for the app to save its
   * properties for session management purposes.
   */
  void saveProperties(KConfig *);

  /**
   * This function is called when this app is restored.  The KConfig
   * object points to the session management config file that was saved
   * with @ref saveProperties
   */
  void readProperties(KConfig *);

private slots:
  void fileNew();
  void fileOpen();
  void fileSave();
  void fileSaveAs();
  void optionsPreferences();

  void changeStatusbar(const QString& text);
  void changeCaption(const QString& text);

  void shrinkTitle();
  void concatenateFiles();
  void extractVideo();
  void extractAudio();
  void extractSubpicture();
  void requant();
  void multiplex();
  void dvdDirGen();
  void dvdAuthor();

  void readFromStdout();
  void readFromStdout2File();
  void readFromStderr();

private:
  void setupAccel();
  void setupActions();

private:
  KopyRightView *m_view;
  KActionMenu *dvdMenu;

  QProcess *m_proc;
  QFile *file;
  QDataStream stream;
  QStringList m_fileList;

  int m_currentTitle;
};

#endif // _KOPYRIGHT_H_
