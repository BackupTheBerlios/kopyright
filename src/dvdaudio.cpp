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
#include "dvdaudio.h"

DvdAudio::DvdAudio(DvdSubpicture *parent, const char *name) : DvdSubpicture(parent, name)
{
}

DvdAudio::DvdAudio(int id, QString lc, QString la, QString fo, QString fr, QString qu, int ch, int ap, QString co, double by, bool se)
 : DvdSubpicture(id, lc, la, co, by, se)
{
  audioFormat = fo;
  audioFrequency = fr;
  audioQuantization = qu;
  audioChannels = ch;
  audioApplicationMode = ap;
}

DvdAudio::~DvdAudio()
{
}

// Input Methods

void DvdAudio::setFormat(QString format)
{
  this->audioFormat = format;
}

void DvdAudio::setFrequency(QString freq)
{
  this->audioFrequency = freq;
}

void DvdAudio::setQuantization(QString quant)
{
  this->audioQuantization = quant;
}

void DvdAudio::setChannels(int chan)
{
  this->audioChannels = chan;
}

void DvdAudio::setApplicationMode(int ap)
{
  this->audioApplicationMode = ap;
}

// Output Methods

QString DvdAudio::format()
{
  return this->audioFormat;
}

QString DvdAudio::frequency()
{
  return this->audioFrequency;
}

QString DvdAudio::quantization()
{
  return this->audioQuantization;
}

int DvdAudio::channels()
{
  return this->audioChannels;
}

int DvdAudio::applicationMode()
{
  return this->audioApplicationMode;
}

#include "dvdaudio.moc"
