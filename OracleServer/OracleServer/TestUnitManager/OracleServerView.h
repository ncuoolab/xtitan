#ifndef  ORACLESERVER_H
#define ORACLESERVER_H

#include <QtGui/QMainWindow>
#include <QString>
#include <QTcpServer>

#include <boost/tr1/memory.hpp>

namespace Ui {
	class OracleServerClass;
}
class QxSelectTestDialog;
class QxSaveDialog;
class QxPreferenceDialog;
class TestUnitManager;
class QxTestCase;

class OracleServerView : public QMainWindow
{
	Q_OBJECT
public:
	OracleServerView();

	void addView(QWidget* widget,QString name);
	void removeView(QWidget* widget);

	void setSingleShot( bool singleShot );

public slots:
	void log( const QString & type, const QString & message );
	void error( const QString & type, const QString & errorString );
	void slot_startRegression();
	void slot_startSmokeTest();

private slots:
	void onStartRecord_();
	void onStopRecord_();
	void onStopReplay_();
	void onTestSpecific_();
	void onRequestSave_( const QxTestCase & );
	void onUpdate_();
	void onFinished_();

private:
	void lockReplayUI_( bool );
	void lockRecordUI_( bool );
	void showSaveDialog_( QxTestCase );

	std::tr1::shared_ptr< Ui::OracleServerClass > ui_;
	QxSelectTestDialog * selectDialog_;
	QxSaveDialog * saveDialog_;
	QxPreferenceDialog * preference_;
	TestUnitManager * manager_;
};

#endif // ORACLESERVER_H
