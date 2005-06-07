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


#include "kopyrightview.h"
#include "dvdvisualice.h"

#include <qpainter.h>
#include <qlayout.h>
#include <qlistview.h>
#include <qlabel.h>

#include <klistview.h>
#include <ktrader.h>
#include <klibloader.h>
#include <kmessagebox.h>
#include <krun.h>
#include <klocale.h>
#include <kruler.h>

KopyRightView::KopyRightView(QWidget *parent) : KopyRightViewBase(parent), DCOPObject("KopyRightIface")
{
  // setup our layout manager to automatically add our widgets
  QHBoxLayout *top_layout = new QHBoxLayout(this);
  top_layout->setAutoAdd(true);
}

KopyRightView::~KopyRightView()
{
}

void KopyRightView::setBytes(double bytes)
{
  m_newStructSize = bytes;
}

double KopyRightView::bytes()
{
  return m_newStructSize;
}

void KopyRightView::updateSizes(QListViewItem *title)
{
  // Size like text
  if(title) {
    m_propVideoSize->setText(visualiceSize(((DvdTitle *)title)->video()->bytes()));
    m_propAudioSize->setText(visualiceSize(((DvdTitle *)title)->audioBytes()));
    m_propSubpictureSize->setText(visualiceSize(((DvdTitle *)title)->subpicturesBytes()));
  }

  // KRuler and size of new DVD only changes if the chages are in a edit title
//  if( ((QString)title->listView()->name() == "m_newStruct") || ((QString)title->listView()->name() == "m_propAudio")
//   || ((QString)title->listView()->name() == "m_propSubpictur")) {

    m_totalSize->setText(visualiceSize(m_newStructSize));

    m_fillRuler->setValue((int)m_newStructSize / 10000000);

    if(m_newStructSize > 4700000000.) m_fillRuler->setPaletteBackgroundColor(QColor(238, 0, 0));
    else m_fillRuler->setPaletteBackgroundColor(QColor(0, 238, 16));
//  }

//  m_correctSize->
//requant_factor = (video_size / (4700000000 - audio_size)) * 1.04;
//  if (m_newStructSize > 4700000000) m_shrinkBox;
}

void KopyRightView::slotSetStatus(const QString& status)
{
  emit signalChangeStatusbar(status);
}

void KopyRightView::slotSetTitle(const QString& title)
{
  emit signalChangeCaption(title);
}

void KopyRightView::editTitle(QListViewItem *stream)
{
  if(stream && stream->rtti() == 1) {
    bool ok;
    DvdTitle *title = (DvdTitle *)m_newStruct->currentItem();
    DvdSubpicture *current;

    if((QString)stream->listView()->name() == "m_propAudio") {
      current = (DvdAudio *)title->audio().at(stream->text(0).toInt(&ok, 10));

      if(((QCheckListItem *)stream)->isOn()) {
        current->setSelected(true);
        title->setAudioBytes(title->audioBytes() + current->bytes());
        this->setBytes(this->bytes() + current->bytes());
      } else {
        current->setSelected(false);
        title->setAudioBytes(title->audioBytes() - current->bytes());
        this->setBytes(this->bytes() - current->bytes());
      }

    } else if((QString)stream->listView()->name() == "m_propSubpictur") {
      current = (DvdSubpicture *)title->subPictures().at(stream->text(0).toInt(&ok, 10));

      if(((QCheckListItem *)stream)->isOn()) {
        current->setSelected(true);
        title->setSubpicturesBytes(title->subpicturesBytes() + current->bytes());
        this->setBytes(this->bytes() + current->bytes());
      } else {
        current->setSelected(false);
        title->setSubpicturesBytes(title->subpicturesBytes() - current->bytes());
        this->setBytes(this->bytes() - current->bytes());
      }
    }
    updateSizes(title);
  }
}

void KopyRightView::requantTitle(const QString &factor)
{
  if(m_newStruct->childCount() != 0) {
    bool ok;
    DvdTitle *title = (DvdTitle *)m_newStruct->currentItem();
    double fac = factor.toDouble(&ok);

    title->setRequantFactor(fac);
//  title->video()->set
    m_propVideoSize->setText(visualiceSize((fac / 1.04) * (4700000000. - title->audioBytes())));
  }
}

void KopyRightView::viewTitle(QListViewItem *title)
{
  if(title && title->rtti() == 2) {
    m_propAudio->clear();

    QCheckListItem *check;

    QPtrList<DvdAudio> list_audio = ((DvdTitle *)title)->audio();
    DvdAudio *audio;
    if (list_audio.isEmpty()) m_propAudio->insertItem (new QListViewItem (m_propAudio, "No Audio Streams"));
    else {
      if((QString)title->listView()->name() == "m_struct") {
        m_newStruct->clearSelection();
        for(audio = list_audio.first(); audio; audio = list_audio.next()) {
          m_propAudio->insertItem(
            new QListViewItem(m_propAudio, audio->idAsText(), audio->language(), audio->format(), audio->frequency(), visualiceSize(audio->bytes())));
        }
      } else if((QString)title->listView()->name() == "m_newStruct") {
        m_struct->clearSelection();
        for(audio = list_audio.first(); audio; audio = list_audio.next()) {
          check = new QCheckListItem(m_propAudio, audio->idAsText(), QCheckListItem::CheckBoxController);
          check->setText(1, audio->language());
          check->setText(2, audio->format());
          check->setText(3, audio->frequency());
          check->setText(4, visualiceSize(audio->bytes()));
          m_propAudio->insertItem(check);
          if(audio->isSelected()) check->setState(QCheckListItem::On);
          else check->setState(QCheckListItem::Off);
        }
      }
    }

    m_propSubpictur->clear();

    QPtrList<DvdSubpicture> list_subPictures = ((DvdTitle *)title)->subPictures();
    DvdSubpicture * Subpicture;
    if (list_subPictures.isEmpty()) m_propSubpictur->insertItem (new QListViewItem (m_propSubpictur, "No Subpiture Streams"));
    else {
      if((QString)title->listView()->name() == "m_struct") {
        m_newStruct->clearSelection();
        for(Subpicture = list_subPictures.first(); Subpicture; Subpicture = list_subPictures.next()) {
          m_propSubpictur->insertItem(new QListViewItem(m_propSubpictur, Subpicture->idAsText(), Subpicture->language(), visualiceSize(Subpicture->bytes())));
        }
      } else if((QString)title->listView()->name() == "m_newStruct") {
        m_struct->clearSelection();
        for(Subpicture = list_subPictures.first(); Subpicture; Subpicture = list_subPictures.next()) {
          check = new QCheckListItem(m_propSubpictur, Subpicture->idAsText(), QCheckListItem::CheckBoxController);
          check->setText(1, Subpicture->language());
          check->setText(2, visualiceSize(Subpicture->bytes()));
          m_propSubpictur->insertItem(check);
          if(Subpicture->isSelected()) check->setOn(true);
          else check->setOn(false);
        }
      }
    }
    updateSizes((DvdTitle *)title);
  }
}

#include "kopyrightview.moc"
