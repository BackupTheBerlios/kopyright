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


#include "kopyright.h"
#include "dvdparsing.h"
#include "pref.h"

#include <qdragobject.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>
#include <qlistview.h>
#include <qmessagebox.h>
#include <qdir.h>

#include <kglobal.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kdeversion.h>
#include <kstatusbar.h>
#include <kaccel.h>
#include <kio/netaccess.h>
#include <kfiledialog.h>
#include <kconfig.h>
#include <kurl.h>
#include <kurldrag.h>
#include <kurlrequesterdlg.h>
#include <kstdaccel.h>
#include <kaction.h>
#include <kstdaction.h>
#include <klistview.h>
#include <kruler.h>

KopyRight::KopyRight() : KMainWindow( 0, "KopyRight" ), m_view(new KopyRightView(this))
{
  // accept dnd
  setAcceptDrops(true);

  // tell the KMainWindow that this is indeed the main widget
  setCentralWidget(m_view);

  // then, setup our actions
  setupActions();

  // and a status bar
  statusBar()->show();

  // Apply the create the main window and ask the mainwindow to automatically save settings if changed: window size, toolbar
  // position, icon size, etc.  Also to add actions for the statusbar toolbar, and keybindings if necessary.
  setupGUI();

  // allow the view to change the statusbar and caption
  connect(m_view, SIGNAL(signalChangeStatusbar(const QString&)), this, SLOT(changeStatusbar(const QString&)));
  connect(m_view, SIGNAL(signalChangeCaption(const QString&)), this, SLOT(changeCaption(const QString&)));
}

KopyRight::~KopyRight()
{
}

void KopyRight::setupActions()
{
  KStdAction::openNew(this, SLOT(fileNew()), actionCollection());
  KStdAction::open(this, SLOT(fileOpen()), actionCollection());
  KStdAction::save(this, SLOT(fileSave()), actionCollection());
  KStdAction::saveAs(this, SLOT(fileSaveAs()), actionCollection());
  KStdAction::find(this, SLOT(shrinkTitle()), actionCollection()); // Change for custom KAction
  KStdAction::quit(kapp, SLOT(quit()), actionCollection());

  KStdAction::preferences(this, SLOT(optionsPreferences()), actionCollection());

  // this doesn't do anything useful.  it's just here to illustrate how to insert a custom menu and menu item
//  dvdMenu = new KActionMenu(i18n("D&vd"), "dvd", actionCollection(), "dvd");
//  KAction *shrinking = new KAction(i18n("&Shrink Dvd"), 0, this, SLOT(shrink()), actionCollection(), "shrinking");
}

QStringList KopyRight::titleFiles(QString dvdDir, int title)
{
  QDir path(dvdDir + "/VIDEO_TS/");
  if(!path.exists()) QMessageBox::information(this, "maKer", "Cannot find the example directory");

  QStringList vts = path.entryList(QDir::Files, QDir::Name);
  vts = vts.grep(".VOB");

  // Select the current title VOB files
  if(title < 10) vts = vts.grep("VTS_0" + QString::number(title));
  else vts = vts.grep("VTS_" + QString::number(title));

  return vts;
}

void KopyRight::saveProperties(KConfig *config)
{
/*
  // the 'config' object points to the session managed config file.  
  //anything you write here will be available later when this app is restored
  if (!m_view->currentURL().isEmpty()) {
    #if KDE_IS_VERSION(3,1,3)
      config->writePathEntry("lastURL", m_view->currentURL());
    #else
      config->writeEntry("lastURL", m_view->currentURL());
    #endif
  }
*/
/*
    // this brings up the generic open dialog
    KURL url = KURLRequesterDlg::getURL(QString::null, this, i18n("Open Location") );

    // standard filedialog
    KURL url = KFileDialog::getOpenURL(QString::null, QString::null, this, i18n("Open Location"));
    if (!url.isEmpty()) m_view->openURL(url);
*/
}

void KopyRight::readProperties(KConfig *config)
{
/*
  // the 'config' object points to the session managed
  // config file.  this function is automatically called whenever
  // the app is being restored.  read in here whatever you wrote in 'saveProperties'
  QString url = config->readPathEntry("lastURL");
  if (!url.isEmpty()) m_view->openURL(KURL(url));
*/
}

void KopyRight::dragEnterEvent(QDragEnterEvent *event)
{
  // accept QListViewItem drags only
  // must be like event->accept(KURLDrag::canDecode(event)); but we have some like
  if (event->provides("application/x-qlistviewitem")) event->accept();
}

void KopyRight::dropEvent(QDropEvent *event)
{
  DvdTitle *copy;
  QListViewItem *tit;
  QPixmap icon;

  // this is a very simplistic implementation of a drop event.
  if(m_view->m_struct->currentItem()->rtti() == 2) {
    // If first Node with title name not exist will create this
    if (!m_view->m_newStruct->firstChild()) {
      icon.load("/home/javi/Desarrollo/maker/images/dvd_unmount.png");
      tit = new QListViewItem(m_view->m_newStruct, m_view->m_struct->firstChild()->text(0));
      tit->setPixmap(0, icon);
    }
    icon.load("/home/javi/Desarrollo/maker/images/display.png");
    copy = new DvdTitle(m_view->m_newStruct->firstChild(), (DvdTitle *)(m_view->m_struct->currentItem()));
    copy->setPixmap(0, icon);
    copy->setText(0, "Title: " + QString::number(copy->id()));
    copy->setText(1, copy->video()->lengthFormated());
    copy->setText(2, visualiceSize(copy->bytes()));

    m_view->setBytes(m_view->bytes() + copy->bytes());
    m_view->updateSizes(NULL);
  }
}

void KopyRight::fileNew()
{
  // this slot is called whenever the File->New menu is selected,
  // the New shortcut is pressed (usually CTRL+N) or the New toolbar button is clicked
  m_view->m_newStruct->clear();
  m_view->m_struct->clear();
  m_view->m_propAudio->clear();
  m_view->m_propSubpictur->clear();
  m_view->m_propVideoSize->setText("00.00 b");
  m_view->m_propAudioSize->setText("00.00 b");
  m_view->m_propSubpictureSize->setText("00.00 b");
}

void KopyRight::fileOpen()
{
  // this slot is called whenever the File->Open menu is selected,
  // the Open shortcut is pressed (usually CTRL+O) or the Open toolbar button is clicked
  m_view->m_newStruct->clear();
  m_view->m_struct->clear();

  m_view->m_newStruct->setRootIsDecorated(true);
  m_view->m_struct->setRootIsDecorated(true);

  allDvdInfo("/dev/dvd", m_view->m_struct);
}

void KopyRight::fileSave()
{
  // this slot is called whenever the File->Save menu is selected,
  // the Save shortcut is pressed (usually CTRL+S) or the Save toolbar
  // button is clicked

  // save the current file
}

void KopyRight::fileSaveAs()
{
/*
  // this slot is called whenever the File->Save As menu is selected,
  KURL file_url = KFileDialog::getSaveURL();
  if (!file_url.isEmpty() && file_url.isValid())
  {
    // save your info, here
  }
*/
}

void KopyRight::shrinkTitle()
{
  // Get first title in new Dvd Structure
//  DvdTitle *current = (DvdTitle*) (m_view->m_newStruct->firstChild())->itemBelow();

  // Get names of vob files for all titles in Dvd
//  QStringList vts = titleFiles("/dvd", current->id());
//  QStringList vts = titleFiles("/dvd", 3);

  m_currentTitle = 3;
  concatenateFiles();


  ////////////////////////////////////////// Concatenate VOB files of each title //////////////////////////////////////////
/*
  m_proc = new QProcess(this);

  m_proc->addArgument("cat");
  for (QStringList::Iterator it = vts.begin(); it != vts.end(); ++it) {
    m_proc->addArgument("/dvd/VIDEO_TS/" + *it);
  }

  // Create a temp file that concatenate the current title VOB's files
  file = new QFile("/home/javi/shrink/tmp/title1.vob");
  file->open(IO_WriteOnly);
  stream.setDevice(file);

  connect(m_proc, SIGNAL(readyReadStdout()), this, SLOT(readFromStdout2File()));
  connect(m_proc, SIGNAL(readyReadStderr()), this, SLOT(readFromStderr()));
  connect(m_proc, SIGNAL(processExited()), this, SLOT(extractVideo()));

  if (!m_proc->start()) QMessageBox::information(this, "maKer", "Unable to find the Application.");
*/
  // Slots call for:
  //   Get video for selected title VOB file
  //   Get only selected audio streams
  //   Requantice video file for each title
  //   Multiplex all files
  //   Create DVD structure
  //   Authoring DVD

/*
  for(int i = 0; i < (m_view->m_newStruct->firstChild())->childCount(); i++) {
     comprueba si hemos modificado algo del titulo actual, quitando algo o comprimiendolo
     si es que si
       saca los nombres de los VTS del titulo
       junta todos en un fichero
       saca el video del fichero
       saca el audio del fichero
       saca los subtitulos del fichero de video
       si es mas grande que un DVD calcula el indice de recompresion y se lo aplica
       remultiplexa los ficheros de video y audio
  }
  crea la estructura de ficheros del dvd
  hace un dvdauthor
  hace otro dvdauthor que crea los ficheros ifo

  quemar el dvd con k3b
*/
}

void KopyRight::optionsPreferences()
{
  // popup some sort of preference dialog, here
  KopyRightPreferences dlg;
  if (dlg.exec()) {
    // redo your settings
  }
}

void KopyRight::changeStatusbar(const QString& text)
{
  // display the text on the statusbar
  statusBar()->message(text);
}

void KopyRight::changeCaption(const QString& text)
{
  // display the text on the caption
  setCaption(text);
}

void KopyRight::readFromStdout()
{
  // Read and process the data.
  // Bear in mind that the data might be output in chunks.
  QMessageBox::information( this, "maKer", m_proc->readStdout() );
}

void KopyRight::readFromStdout2File()
{
  // We will serialize the data into the file
  stream << m_proc->readStdout();
}

void KopyRight::readFromStderr()
{
  // Read and process the data.
  // Bear in mind that the data might be output in chunks.
  QMessageBox::information( this, "maKer", m_proc->readStderr() );
}

void KopyRight::concatenateFiles()
{
  QStringList vts = titleFiles("/dvd", m_currentTitle);

  m_proc = new QProcess(this);
  m_proc->addArgument("cat");
  for (QStringList::Iterator it = vts.begin(); it != vts.end(); ++it) {
    m_proc->addArgument("/dvd/VIDEO_TS/" + *it);
  }
  connect(m_proc, SIGNAL(readyReadStdout()), this, SLOT(readFromStdout2File()));
  connect(m_proc, SIGNAL(readyReadStderr()), this, SLOT(readFromStderr()));
  connect(m_proc, SIGNAL(processExited()), this, SLOT(extractVideo()));

  // Create a tmp file that concatenate the current title VOB's files
  file = new QFile("/home/javi/shrink/tmp/title" + QString::number(m_currentTitle) + ".vob");
  file->open(IO_WriteOnly);
  stream.setDevice(file);

  if (!m_proc->start()) QMessageBox::information(this, "maKer", "Unable to find the Application.");
}

void KopyRight::extractVideo()
{
  m_proc = new QProcess(this);
  prepareProcess(m_proc, "tcextract -i /home/javi/shrink/tmp/title" + QString::number(m_currentTitle) + ".vob -t vob -x mpeg2");
  connect(m_proc, SIGNAL(readyReadStdout()), this, SLOT(readFromStdout2File()));
  connect(m_proc, SIGNAL(processExited()), this, SLOT(extractAudio()));

  file = new QFile("/home/javi/shrink/tmp/title" + QString::number(m_currentTitle) + ".m2v");
  file->open(IO_WriteOnly);
  stream.setDevice(file);

  if (!m_proc->start()) QMessageBox::information(this, "maKer", "Unable to find the Application.");
}

void KopyRight::extractAudio()
{
  m_proc = new QProcess(this);
  prepareProcess(m_proc, "tcextract -i /home/javi/shrink/tmp/title" + QString::number(m_currentTitle) + ".vob -t vob -x ac3 -a 0");
  connect(m_proc, SIGNAL(readyReadStdout()), this, SLOT(readFromStdout2File()));
  connect(m_proc, SIGNAL(processExited()), this, SLOT(requant()));

  file = new QFile("/home/javi/shrink/tmp/title" + QString::number(m_currentTitle) + ".ac3");
  file->open(IO_WriteOnly);
  stream.setDevice(file);

  if (!m_proc->start()) QMessageBox::information(this, "maKer", "Unable to find the Application.");
}

void KopyRight::extractSubpicture()
{
  m_proc = new QProcess(this);
  prepareProcess(m_proc, "tcextract -i /home/javi/shrink/tmp/title" + QString::number(m_currentTitle) + ".vob -t vob -x ps1 -a 0");
  connect(m_proc, SIGNAL(readyReadStdout()), this, SLOT(readFromStdout2File()));
//  connect(m_proc, SIGNAL(processExited()), this, SLOT(finished()));

  file = new QFile("/home/javi/shrink/tmp/title" + QString::number(m_currentTitle) + ".sub");
  file->open(IO_WriteOnly);
  stream.setDevice(file);

  if (!m_proc->start()) QMessageBox::information(this, "maKer", "Unable to find the Application.");
}

void KopyRight::requant()
{
  m_proc = new QProcess(this);
  prepareProcess(m_proc, "tcrequant -i /home/javi/shrink/tmp/title" + QString::number(m_currentTitle) 
                         + ".m2v -o /home/javi/shrink/tmp/title_shr" + QString::number(m_currentTitle) + ".m2v -f 1.5");
//  double requant_factor = (video_size / (4700000000 - audio_size)) * 1.04;
  connect(m_proc, SIGNAL(processExited()), this, SLOT(multiplex()));

  if (!m_proc->start()) QMessageBox::information(this, "maKer", "Unable to find the Application.");
}

void KopyRight::multiplex()
{
  m_proc = new QProcess(this);
  prepareProcess(m_proc, "mplex -f 8 -o /home/javi/shrink/tmp/final" + QString::number(m_currentTitle)
                         + ".mpg /home/javi/shrink/tmp/title_shr" + QString::number(m_currentTitle)
                         + ".m2v /home/javi/shrink/tmp/title" + QString::number(m_currentTitle) + ".ac3");
  // connect(m_proc, SIGNAL(readyReadStdout()), this, SLOT(readFromStdout2File()));
  // connect(m_proc, SIGNAL(readyReadStderr()), this, SLOT(readFromStderr()));
  // connect(m_proc, SIGNAL(processExited()), this, SLOT(extractSubpicture()));

  if (!m_proc->start()) QMessageBox::information(this, "maKer", "Unable to find the Application.");
}

void KopyRight::dvdDirGen()
{
  m_proc = new QProcess(this);
  prepareProcess(m_proc, "mkdir /home/javi/shrink/VIDEO_TS /home/javi/shrink/AUDIO_TS");
  // connect(m_proc, SIGNAL(readyReadStdout()), this, SLOT(readFromStdout2File()));
  // connect(m_proc, SIGNAL(readyReadStderr()), this, SLOT(readFromStderr()));
  // connect(m_proc, SIGNAL(processExited()), this, SLOT(extractSubpicture()));

  if (!m_proc->start()) QMessageBox::information(this, "maKer", "Unable to find the Application.");
}

void KopyRight::dvdAuthor()
{
//dvdauthor -o newdvd final.mpg
  m_proc = new QProcess(this);

  m_proc->addArgument("dvdauthor");
  m_proc->addArgument("-o");
  m_proc->addArgument("/home/javi/shr/AUDIO_TS");
  m_proc->addArgument("/home/javi/shr/AUDIO_TS");

//  connect(m_proc, SIGNAL(readyReadStdout()), this, SLOT(readFromStdout2File()));
//  connect(m_proc, SIGNAL(readyReadStderr()), this, SLOT(readFromStderr()));
//  connect(m_proc, SIGNAL(processExited()), this, SLOT(extractSubpicture()));

  if (!m_proc->start()) QMessageBox::information(this, "maKer", "Unable to find the Application.");
}

void KopyRight::prepareProcess(QProcess *process, QString command)
{
  QStringList args;
  args = args.split(" ", command);
  process->setArguments(args);
}

#include "kopyright.moc"
