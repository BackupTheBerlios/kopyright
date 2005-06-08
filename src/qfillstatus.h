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
#ifndef QFILLSTATUS_H
#define QFILLSTATUS_H

#include <qwidget.h>

/**
@author Matías Costa
*/
class QFillStatus : public QWidget
{
Q_OBJECT
public:
	QFillStatus(int fill=0, int maxFill=4700000, QWidget* parent=0, const char* name="unnamed", WFlags f=0);
	~QFillStatus();

	int fill();
	void setFill(int fill);
	
	int maxFill();
	void setMaxFill(int maxFill);
protected:
	void QFillStatus::paintEvent(QPaintEvent*);

private:
	int	m_fill;
	int	m_maxFill;
};

#endif
