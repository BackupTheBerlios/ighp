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

#pragma once

#include <QDialog>
#include <QList>

class QWidget;
class QTabWidget;
class QDialogButtonBox;
class QTableView;
class QPushButton;
class QItemSelection;
class QModelIndex;

class GhAction;
class GhKey;
class GhKeyModel;

class GhDialog : public QDialog
{
	Q_OBJECT

public:
	GhDialog(QWidget *parent = 0);
	~GhDialog();

public slots:
	void addHkButtonClicked();
	void changeHkButtonClicked();
	void removeHkButtonClicked();
	void applyButtonClicked();

	void hotkeysTableSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
	void keyModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

private:
	GhKeyModel *keyModel;
	QTabWidget *tabWidget;
	
	QWidget *hotkeysPage;
	QWidget *aboutPage;

	QDialogButtonBox *dialogButtonBox;

	QTableView *hotkeysTable;

	QPushButton *addHkButton;
	QPushButton *changeHkButton;
	QPushButton *removeHkButton;
	QPushButton *applyButton;

	void initializeGui();
};
