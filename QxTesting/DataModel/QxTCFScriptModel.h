#ifndef _QX_TCF_SCRIPT_MODEL_H
#define _QX_TCF_SCRIPT_MODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QStringList>

class QxTCFScriptModel : public QAbstractListModel
{
	Q_OBJECT

public:
	QxTCFScriptModel(QStringList cmdList,QObject *parent = 0);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	void loadTCF(const QString &path);

	QStringList getStringList() const {	return commandList;		}


signals:
	void numberPopulated(int number);

private slots:


protected:
	bool canFetchMore(const QModelIndex &parent) const;
	void fetchMore(const QModelIndex &parent);

private:
	QStringList commandList;
	int commandCount;
};

#endif