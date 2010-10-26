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

#include "GhKeyModel.h"

#include <QKeySequence>

#include "GhKey.h"
#include "GhAction.h"

GhKeyModel::GhKeyModel(QObject *parent) : QAbstractTableModel(parent) { /* nothing to do */ }

int GhKeyModel::rowCount(const QModelIndex &parent) const
{
	return keys.count();
}

bool GhKeyModel::insertRows(int position, int rows, const QModelIndex &index)
{
	beginInsertRows(QModelIndex(), position, position + rows - 1);
	
	for (int row = 0; row < rows; row++) {
		keys.insert(position, GhKey(0, QKeySequence()));
	}
	
	endInsertRows();
	return true;
}

bool GhKeyModel::removeRows(int position, int rows, const QModelIndex &index)
{
	beginRemoveRows(QModelIndex(), position, position + rows - 1);
	
	for (int row = 0; row < rows; row++) {
		keys.removeAt(position);
	}
	
	endRemoveRows();
	return true;
}

bool GhKeyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid() || role != Qt::EditRole)
		return false;

	int row = index.row();

	if (index.column() == Column_Action) {
		keys.replace(row, GhKey(GhAction::Actions[value.toInt()], QKeySequence()));
	} else if (index.column() == Column_Hotkey) {
		keys[row].setKeySequence(QKeySequence(value.toInt()));
	} else return false;

	emit(dataChanged(index, index));
	return true;
}

QVariant GhKeyModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role == Qt::TextAlignmentRole)
		return int(Qt::AlignLeft | Qt::AlignVCenter);

	if (role == Qt::DisplayRole) {
		if (index.column() == Column_Action) {
			const GhAction *action = keys[index.row()].action();
			
			if (action == 0)
				return QVariant();

			return QString(keys[index.row()].action()->name());
		} else if (index.column() == Column_Hotkey)
			return QString(keys[index.row()].keySequence().toString());
	}

	return QVariant();
}

QVariant GhKeyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole)
		return QVariant();

	if (orientation != Qt::Horizontal)
		return QVariant();

	if (section == Column_Action)
		return QString(tr("Action"));

	if (section == Column_Hotkey)
		return QString(tr("Hotkey"));

	return QVariant();
}

bool GhKeyModel::containsKeySequence(const int keySequenceId) const
{
	for (int i = 0; i < keys.size(); ++i) {
		if(keys[i].keySequence()[0] == keySequenceId)
			return true;
	}

	return false;
}