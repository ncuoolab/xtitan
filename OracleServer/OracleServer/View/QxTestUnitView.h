#ifndef QXTESTUNITVIEW_H
#define QXTESTUNITVIEW_H

#include <QtGui/QWidget>

#include <boost/tr1/memory.hpp>

namespace Ui {
	class QxTestUnitView;
}
class QxInputPoint;
class QxCheckPoint;

class QxTestUnitView : public QWidget {
public:
	QxTestUnitView();

	void appendLog( const QString & msg );
	void appendInput( const QxInputPoint & input );
	void setCurrentLine( int line );

private:
	std::tr1::shared_ptr< Ui::QxTestUnitView > ui_;
};

#endif
