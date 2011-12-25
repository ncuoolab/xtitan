#include "QxTesting/DataModel/QxTCFScriptModel.h"
#include <QApplication>
#include <QPalette>
#include <QBrush>
#include <QFile>
#include <QTextStream>
#include <QStringList>

QxTCFScriptModel::QxTCFScriptModel(QStringList cmdList, QObject *parent)
: commandList(cmdList),commandCount(0), QAbstractListModel(parent)
{}

int QxTCFScriptModel::rowCount(const QModelIndex &  parent ) const
{
	return commandCount;
}

QVariant QxTCFScriptModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (index.row() >= commandList.size() || index.row() < 0)
		return QVariant();

	if (role == Qt::DisplayRole)
		return commandList.at(index.row());
	else if (role == Qt::BackgroundRole) {
		int batch = (index.row() / 100) % 2;
		if (batch == 0)
			return qApp->palette().base();
		else
			return qApp->palette().alternateBase();
	}
	return QVariant();
}

bool QxTCFScriptModel::canFetchMore(const QModelIndex & index ) const
{
	if (commandCount < commandList.size())
		return true;
	else
		return false;
}

void QxTCFScriptModel::fetchMore(const QModelIndex &  index )
{
	int remainder = commandList.size() - commandCount;
	int itemsToFetch = qMin(100, remainder);

	beginInsertRows(QModelIndex(), commandCount, commandCount+itemsToFetch);

	commandCount += itemsToFetch;

	endInsertRows();

	emit numberPopulated(itemsToFetch);
}

void QxTCFScriptModel::loadTCF(const QString &path)
{
	QFile file( path ); 
	if ( file.open( QFile::ReadOnly) ) { 
		QTextStream stream( &file ); 
		QString line; 
		while ( !stream.atEnd() ) { 
			line = stream.readLine(); // line of text excluding '\n' 
			commandList += line; 
		} 
		file.close(); 
	} 
	commandCount = 0;
	reset();
}