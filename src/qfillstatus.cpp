/***************************************************************************
 *   Copyright (C) 2005 by Matías Costa   *
 *   mcc3@alu.um.es   *
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
#include "qfillstatus.h"

#include <qpainter.h>

QFillStatus::QFillStatus(int fill, int maxFill, QWidget* parent, const char* name, WFlags f): QWidget(parent, name, f)
	,m_fill(fill), m_maxFill(maxFill)
{
	setSizePolicy(QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)5, 0, 0, false ));
	setMinimumSize(QSize(32, 24)); 
	setBackgroundMode(NoBackground);
}


QFillStatus::~QFillStatus()
{
}

int QFillStatus::fill() { return m_fill; }
void QFillStatus::setFill(int fill) { m_fill = fill; }
	
int QFillStatus::maxFill() { return m_maxFill; }
void QFillStatus::setMaxFill(int maxFill) { m_maxFill = maxFill; }

void QFillStatus::paintEvent(QPaintEvent* /*e*/)
{
	int w=width(), h=height();
	int green;
	QPainter p(this);
	if (m_fill <= m_maxFill){
		green = (m_fill * w) / m_maxFill;
		p.fillRect(0, 0, green, h, Qt::green);
		p.fillRect(green, 0, w, h, backgroundColor());
	} else {
		green = (m_maxFill * w) / m_fill;
		p.fillRect(0, 0, green, h, Qt::green);
		p.fillRect(green, 0, w, h, Qt::red);
	}
	;
	const int fillMB = m_fill / 1024;
	const int maxFillMB = m_maxFill / 1024;
	p.drawText (rect(), Qt::AlignHCenter | Qt::AlignCenter,
		    QString("%1 of %2 MB used").arg(fillMB).arg(maxFillMB));

}

#include "qfillstatus.moc"
