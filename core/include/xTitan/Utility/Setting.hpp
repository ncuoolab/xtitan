#ifndef XTITAN_UTILITIES_SETTING_HPP
#define XTITAN_UTILITIES_SETTING_HPP

#include <memory>

#include <QtCore/QVariant>

#include "xTitan/Utility/CoreConfig.hpp"


namespace xtitan {

class XTITAN_CORE_DLL Setting {
public:
	static void initialize();
	static Setting & instance();

	QVariant get( const QString & key, const QVariant & defaultValue = QVariant() ) const;
	void set( const QString & key, const QVariant & value );

	void save() const;

	void setFilePath( const QString & path );

private:
	class Private;

	Setting();
	Setting( const Setting & );
	~Setting();
	Setting & operator =( const Setting & );

	std::shared_ptr< Private > p_;
};

}

#endif
