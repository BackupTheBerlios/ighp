/*
 * Copyright (c) 2010 Stefan Cosma <stefan.cosma@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <QtGui>
#include <QKeySequence>

#include "GhKeyPicker.h"

GhKeyPicker::GhKeyPicker(QWidget *parent) : QDialog(parent), mKeySequence(0)
{
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(new QLabel(tr("Press the new keys combination")));
	
	setLayout(layout);
	setWindowTitle(tr("Select Hotkey"));
}

void GhKeyPicker::keyPressEvent(QKeyEvent *event)
{
	event->accept();

	switch (event->key())
	{
		case Qt::Key_unknown:
			return;
		
		case Qt::Key_Control:
			mKeySequence += Qt::CTRL;
			break;
		
		case Qt::Key_Alt:
			mKeySequence += Qt::ALT;
			break;

		case Qt::Key_Shift:
			mKeySequence += Qt::SHIFT;
			break;
		
		case Qt::Key_Meta:
			mKeySequence += Qt::META;
			break;
		
		default:
			mKeySequence += event->key();
			QDialog::accept();
			break;
	}
}

void GhKeyPicker::keyReleaseEvent(QKeyEvent *event)
{
	event->accept();

	switch (event->key())
	{
		case Qt::Key_unknown:
			return;
		
		case Qt::Key_Control:
			mKeySequence -= Qt::CTRL;
			break;
		
		case Qt::Key_Alt:
			mKeySequence -= Qt::ALT;
			break;

		case Qt::Key_Shift:
			mKeySequence -= Qt::SHIFT;
			break;
		
		case Qt::Key_Meta:
			mKeySequence -= Qt::META;
			break;
	}
}

void GhKeyPicker::showEvent(QShowEvent *event)
{
	QWidget::grabKeyboard();
	QWidget::showEvent(event);
}

void GhKeyPicker::hideEvent(QHideEvent *event)
{
	QWidget::releaseKeyboard();
	QWidget::hideEvent(event);
}