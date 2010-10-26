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
#include <QVariant>

#include "GhAction.h"
#include "GhActionPicker.h"

GhActionPicker::GhActionPicker(const QList<GhAction*> *actions, QWidget *parent) : QDialog(parent), id(-1)
{
	list = new QListWidget;
	list->setSelectionMode(QAbstractItemView::SingleSelection);
	for (int i = 0; i < actions->size(); ++i) {
		QListWidgetItem *item = new QListWidgetItem(actions->at(i)->name(), list);
		item->setData(Qt::UserRole, i);
	}
	if (list->count() > 0)
		list->setCurrentItem(list->item(0));

	dialogButtonBox = new QDialogButtonBox;
	dialogButtonBox->setStandardButtons(
		QDialogButtonBox::Ok |
		QDialogButtonBox::Cancel);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(list);
	layout->addWidget(dialogButtonBox);
	
	setLayout(layout);
	setWindowTitle(tr("Select Action"));

	connect(dialogButtonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(dialogButtonBox, SIGNAL(rejected()), this, SLOT(reject()));
	connect(list, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(listDoubleClicked(const QModelIndex&)));
}

void GhActionPicker::done(int result)
{
	id = -1;
	if (result == QDialog::Accepted) {
		QListWidgetItem *item = list->currentItem();
		if (item)
			id = item->data(Qt::UserRole).toInt();
	}
	QDialog::done(result);
}

void GhActionPicker::listDoubleClicked(const QModelIndex& /* index */)
{
	done(QDialog::Accepted);
}
