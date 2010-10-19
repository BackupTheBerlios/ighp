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

#include "GhPlugin.h"

#include "GhDialog.h"
#include "GhActionPicker.h"
#include "GhKeyPicker.h"

#include "GhKeyModel.h"

#include "GhKey.h"
#include "actions.h"

extern GhPlugin *plugin;

GhDialog::GhDialog(QWidget *parent) : QDialog(parent)
{
	keyModel = new GhKeyModel;
	keyModel->setGhKeyList(plugin->hotkeys());

	initializeGui();

	connect(dialogButtonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(dialogButtonBox, SIGNAL(rejected()), this, SLOT(reject()));

	connect(addHkButton, SIGNAL(clicked()), this, SLOT(addHkButtonClicked()));
	connect(changeHkButton, SIGNAL(clicked()), this, SLOT(changeHkButtonClicked()));
	connect(removeHkButton, SIGNAL(clicked()), this, SLOT(removeHkButtonClicked()));
	connect(applyButton, SIGNAL(clicked()), this, SLOT(applyButtonClicked()));

	connect(hotkeysTable->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
		this, SLOT(hotkeysTableSelectionChanged(const QItemSelection &, const QItemSelection &)));

	connect(keyModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
		this, SLOT(keyModelDataChanged(const QModelIndex &, const QModelIndex &)));
}

GhDialog::~GhDialog()
{
	delete keyModel;
}

void GhDialog::hotkeysTableSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
	QModelIndexList indexList = hotkeysTable->selectionModel()->selectedRows();
	const int rowCount = indexList.count();

	changeHkButton->setEnabled(rowCount == 1);
	removeHkButton->setEnabled(rowCount > 0);
}

void GhDialog::addHkButtonClicked()
{
	GhActionPicker actionPicker = GhActionPicker(&GhAction::Actions, this);
	if (actionPicker.exec() != QDialog::Accepted)
		return;

	int actionId = actionPicker.selectedId();
	if (actionId < 0) {
		QMessageBox msgBox;
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setWindowTitle(tr("Invalid Action"));
		msgBox.setText(tr("Invalid action index recieved."));
		msgBox.exec();
		return;
	}

	GhKeyPicker keyPicker = GhKeyPicker(this);
	if (keyPicker.exec() != QDialog::Accepted)
		return;

	int keySequenceId = keyPicker.keySequence();
	if (keySequenceId < 0) {
		QMessageBox msgBox;
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setWindowTitle(tr("Invalid Hotkey"));
		msgBox.setText(tr("Invalid key sequence recieved."));
		msgBox.exec();
		return;
	}

	//TODO: Check for duplicate keys;

	int rowIndex = keyModel->hotkeys().count();
	keyModel->insertRows(rowIndex, 1, QModelIndex());

	QModelIndex index = keyModel->index(rowIndex, GhKeyModel::Column_Action, QModelIndex());
	keyModel->setData(index, actionId, Qt::EditRole);
	index = keyModel->index(rowIndex, GhKeyModel::Column_Hotkey, QModelIndex());
	keyModel->setData(index, keySequenceId, Qt::EditRole);
}

void GhDialog::changeHkButtonClicked()
{
	QModelIndexList indexList = hotkeysTable->selectionModel()->selectedRows();

	GhKeyPicker keyPicker = GhKeyPicker(this);
	if (keyPicker.exec() != QDialog::Accepted)
		return;

	int keySequenceId = keyPicker.keySequence();
	if (keySequenceId < 0) {
		QMessageBox msgBox;
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setWindowTitle(tr("Invalid Hotkey"));
		msgBox.setText(tr("Invalid key sequence recieved."));
		msgBox.exec();
		return;
	}

	QModelIndex index = keyModel->index(indexList[0].row(), GhKeyModel::Column_Hotkey, QModelIndex());
	keyModel->setData(index, keySequenceId, Qt::EditRole);
}

void GhDialog::removeHkButtonClicked()
{
	QModelIndexList indexList = hotkeysTable->selectionModel()->selectedRows();
	QModelIndex index;

	foreach(index, indexList) {
		keyModel->removeRows(index.row(), 1, QModelIndex());
	}
}

void GhDialog::keyModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
	applyButton->setEnabled(true);
}

void GhDialog::applyButtonClicked()
{
	plugin->resetKeys(keyModel->hotkeys());
	applyButton->setEnabled(false);
}

#pragma region Gui
void GhDialog::initializeGui()
{
	hotkeysTable = new QTableView;
	hotkeysTable->setModel(keyModel);
	hotkeysTable->verticalHeader()->hide();
	hotkeysTable->verticalHeader()->setDefaultSectionSize(20);
	hotkeysTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	hotkeysTable->setShowGrid(false);
	hotkeysTable->setCornerButtonEnabled(false);
	hotkeysTable->setAlternatingRowColors(true);
	hotkeysTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	hotkeysTable->setSelectionMode(QAbstractItemView::ExtendedSelection);

	addHkButton = new QPushButton(tr("&Add"));
	changeHkButton = new QPushButton(tr("&Change"));
	changeHkButton->setEnabled(false);
	removeHkButton = new QPushButton(tr("&Remove"));
	removeHkButton->setEnabled(false);

	QHBoxLayout *hpButtonLayout = new QHBoxLayout;
	hpButtonLayout->addWidget(addHkButton);
	hpButtonLayout->addWidget(changeHkButton);
	hpButtonLayout->addWidget(removeHkButton);

	QVBoxLayout *hpLayout = new QVBoxLayout;
	hpLayout->addWidget(hotkeysTable);
	hpLayout->addLayout(hpButtonLayout);

	QVBoxLayout *apLayout = new QVBoxLayout;
	apLayout->addWidget(new QLabel("<a href='http://ighp.berlios.de/'>http://ighp.berlios.de/</a>"));

	hotkeysPage = new QWidget;
	hotkeysPage->setLayout(hpLayout);
	aboutPage = new QWidget;
	aboutPage->setLayout(apLayout);

	tabWidget = new QTabWidget;
	tabWidget->addTab(hotkeysPage, tr("Hotkeys"));
	tabWidget->addTab(aboutPage, tr("About"));

	dialogButtonBox = new QDialogButtonBox;
	dialogButtonBox->setStandardButtons(
		QDialogButtonBox::Ok |
		QDialogButtonBox::Apply |
		QDialogButtonBox::Cancel);

	applyButton = dialogButtonBox->button(QDialogButtonBox::Apply);
	applyButton->setEnabled(false);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(tabWidget);
	layout->addWidget(dialogButtonBox);
	
	setLayout(layout);
	setWindowTitle(tr("Global Hotkeys"));
}
#pragma endregion