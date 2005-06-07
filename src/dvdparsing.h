/***************************************************************************
 *   Copyright (C) 2005 by Javier Garrido Campoy                           *
 *   javi@level13                                                          *
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

#ifndef _DVDPARSING_H_
#define _DVDPARSING_H_

#include "dvdtitle.h"

#include <qstring.h>
#include <qstringlist.h>
#include <qlistview.h>
#include <qpixmap.h>

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/stat.h>
#include <dvdread/ifo_read.h>
#include <dvdread/ifo_types.h>
#include <dvdread/dvd_reader.h>

#define MAXSTREAMS 50
#define PROBEBLOCKS 30720

#define MPEG_VIDEO  0
#define MPEG_AUDIO  1
#define AC3_AUDIO   2
#define DTS_AUDIO   3
#define LPCM_AUDIO  4
#define SUBPICTURE  5

struct streamdata {            // data for 1 stream
   unsigned char id;              // stream id
   long bitrate;                  // bitrate
   int type;                      // type of stream
   double size;                   // size of stream in bytes
};

static struct {
  char code[3];
  char name[20];
} language[] = {
  { "  ", "Not Specified" }, { "aa", "Afar" }, { "ab", "Abkhazian" }, { "af", "Afrikaans" }, { "am", "Amharic" },
  { "ar", "Arabic" }, { "as", "Assamese" }, { "ay", "Aymara" }, { "az", "Azerbaijani" }, { "ba", "Bashkir" },
  { "be", "Byelorussian" }, { "bg", "Bulgarian" }, { "bh", "Bihari" }, { "bi", "Bislama" }, { "bn", "Bengali; Bangla" },
  { "bo", "Tibetan" }, { "br", "Breton" }, { "ca", "Catalan" }, { "co", "Corsican" }, { "cs", "Czech" },
  { "cy", "Welsh" }, { "da", "Dansk" }, { "de", "Deutsch" }, { "dz", "Bhutani" }, { "el", "Greek" }, { "en", "English" },
  { "eo", "Esperanto" }, { "es", "Espanol" }, { "et", "Estonian" }, { "eu", "Basque" }, { "fa", "Persian" },
  { "fi", "Suomi" }, { "fj", "Fiji" }, { "fo", "Faroese" }, { "fr", "Francais" }, { "fy", "Frisian" }, { "ga", "Gaelic" },
  { "gd", "Scots Gaelic" }, { "gl", "Galician" }, { "gn", "Guarani" }, { "gu", "Gujarati" }, { "ha", "Hausa" },
  { "he", "Hebrew" }, { "hi", "Hindi" }, { "hr", "Hrvatski" }, { "hu", "Magyar" }, { "hy", "Armenian" },
  { "ia", "Interlingua" }, { "id", "Indonesian" }, { "ie", "Interlingue" }, { "ik", "Inupiak" }, { "in", "Indonesian" },
  { "is", "Islenska" }, { "it", "Italiano" }, { "iu", "Inuktitut" }, { "iw", "Hebrew" }, { "ja", "Japanese" },
  { "ji", "Yiddish" }, { "jw", "Javanese" }, { "ka", "Georgian" }, { "kk", "Kazakh" }, { "kl", "Greenlandic" },
  { "km", "Cambodian" }, { "kn", "Kannada" }, { "ko", "Korean" }, { "ks", "Kashmiri" }, { "ku", "Kurdish" },
  { "ky", "Kirghiz" }, { "la", "Latin" }, { "ln", "Lingala" }, { "lo", "Laothian" }, { "lt", "Lithuanian" },
  { "lv", "Latvian, Lettish" }, { "mg", "Malagasy" }, { "mi", "Maori" }, { "mk", "Macedonian" }, { "ml", "Malayalam" },
  { "mn", "Mongolian" }, { "mo", "Moldavian" }, { "mr", "Marathi" }, { "ms", "Malay" }, { "mt", "Maltese" },
  { "my", "Burmese" }, { "na", "Nauru" }, { "ne", "Nepali" }, { "nl", "Nederlands" }, { "no", "Norsk" }, { "oc", "Occitan" },
  { "om", "Oromo" }, { "or", "Oriya" }, { "pa", "Punjabi" }, { "pl", "Polish" }, { "ps", "Pashto, Pushto" },
  { "pt", "Portugues" }, { "qu", "Quechua" }, { "rm", "Rhaeto-Romance" }, { "rn", "Kirundi" }, { "ro", "Romanian"  },
  { "ru", "Russian" }, { "rw", "Kinyarwanda" }, { "sa", "Sanskrit" }, { "sd", "Sindhi" }, { "sg", "Sangho" },
  { "sh", "Serbo-Croatian" }, { "si", "Sinhalese" }, { "sk", "Slovak" }, { "sl", "Slovenian" }, { "sm", "Samoan" },
  { "sn", "Shona"  }, { "so", "Somali" }, { "sq", "Albanian" }, { "sr", "Serbian" }, { "ss", "Siswati" },
  { "st", "Sesotho" }, { "su", "Sundanese" }, { "sv", "Svenska" }, { "sw", "Swahili" }, { "ta", "Tamil" },
  { "te", "Telugu" }, { "tg", "Tajik" }, { "th", "Thai" }, { "ti", "Tigrinya" }, { "tk", "Turkmen" }, { "tl", "Tagalog" },
  { "tn", "Setswana" }, { "to", "Tonga" }, { "tr", "Turkish" }, { "ts", "Tsonga" }, { "tt", "Tatar" }, { "tw", "Twi" },
  { "ug", "Uighur" }, { "uk", "Ukrainian" }, { "ur", "Urdu" }, { "uz", "Uzbek" }, { "vi", "Vietnamese" },
  { "vo", "Volapuk" }, { "wo", "Wolof" }, { "xh", "Xhosa" }, { "yi", "Yiddish" }, { "yo", "Yoruba" }, { "za", "Zhuang" },
  { "zh", "Chinese" }, { "zu", "Zulu" }, { "xx", "Unknown" }, { "\0", "Unknown" }
};

  /**
   *  Extract the real start and stop blocks to read from the ifo structure
   *  If start and stop chapters are 0, they'll be set to the right values (start=1, stop=lastchapter)
   */
  int get_start_stop_blocks(dvd_reader_t *dvd, int title, uint8_t *ts_nr, int *chapstart, int *chapstop, uint32_t *startblock, uint32_t *lastblock, int* runtime);

  /**
   *
   */
  int probe_dvd(dvd_file_t *vob, struct streamdata **list, uint32_t start_block, uint32_t stop_block);

  /**
   * Get lenght in miliseconds
   */
  int dvdtime2msec(dvd_time_t *dt);

  /**
   * Get DVD Main Title name
   */
  int get_title_name(const char* dvd_device, char* title);

  /**
   * Get language name to a especific code
   */
  char* lang_name(char* code);

  /**
   * Get DVD info and put this in a QListView
   */
  int allDvdInfo(QString dvdDevice, QListView *list);

#endif // _DVDPARSING_H_
