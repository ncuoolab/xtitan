#include "xTitan/xTitan.hpp"

#include <QtCore/QRegExp>

#include "Spy/Spy.hpp"


using xtitan::Spy;


/**
 * @defgroup spy Spy Library
 */

/**
 * @ingroup spy
 * @def spyInput
 * @brief Records function call
 * @sa spyRegisterObject
 *
 * This macro records function call, so that test driver can replay this
 * function later.
 * The function being recording **MUST** match following rules:
 * 1. This function is a **Qt slot** or **Q_INVOKABLE** decorated.
 * 2. The instance is **registered** by spyRegisterObject.
 * 3. This function is a **public** method (i.e. not private or protected).
 * 4. All parameters either are **one of** these types or can **implicitly convert**
 *    to these types:
 *     * bool
 *     * int64_t
 *     * double
 *     * std::string
 *     * std::wstring
 *
 * To use this macro is quite simple, just put this macro in your function
 * (generally at first line), and concat all parameters with **operator %**.
 *
 * Following example explains how to use spyInput:
 *
 * ~~~{.cpp}
 * void Dialog::showResult(int code, const std::wstring & message) {
 *     spyInput % code % message;
 *     // regular code
 * }
 * ~~~
 */

/**
 * @ingroup spy
 * @def spyCheck
 * @brief Creates synchronized check point
 * @sa spyAsyncCheck
 *
 * This macro creates a synchronized check point, which can record variables or
 * function results while recording, and compares their values in each replay.
 * After replay, if check point sequence mismatched (either by reordering,
 * missing or duplicating), then this replay was failed.
 *
 * To use this macro record variables, simply put variables with
 * **operator %**, but variable type must be one of
 *
 * * bool
 * * int64_t
 * * double
 * * std::string
 * * std::wstring
 *
 * or any other types which can implicitly convert to these types.
 *
 * For example:
 *
 * ~~~{.cpp}
 * void Job::doSomething() {
 *     int i = 0;
 *     // after some logic, you want to check variable i and member variable name
 *     spyCheck % i % this->name;
 * }
 * ~~~
 */

/**
 * @ingroup spy
 * @def spyAsyncCheck
 * @brief Creates asynchronized check point
 * @sa spyCheck
 *
 * Similar to spyCheck, except that check points can reorder. Because execution
 * order may randomly interleave in multi-thread context, you must use this
 * macro in concurrent functions.
 *
 * To distinguish individual execution order in each threads, you must specify
 * ID and precondition to each check points. ID should be a string.
 * Use id() to specify ID, and pre() to specify precondition, for example:
 *
 * ~~~{.cpp}
 * void ConcurrentJob::run() {
 *     int i = 0;
 *     // some logic
 *     spyAsyncCheck.id("id1") % i;
 *     // some logic
 *     spyAsyncCheck.id("id2").pre("id1") % i;
 *     // some logic
 *     spyAsyncCheck.id("id3").pre("id2") % i;
 * }
 * ~~~
 */

/**
 * @ingroup spy
 * @brief Register object for record/replay
 * @param [in] object object being register
 * @param [in] id unique ID for this object
 * @sa spyInput
 *
 * If @p object is nullptr, or @p id is an empty string, this function does **nothing**.
 *
 * Objects must register by this method first before they can be recording by
 * spyInput. A common place for this function is inside a constructor.
 */
void spyRegisterObject( QObject * object, const QString & id ) {
	if( !object || id.isEmpty() ) {
		return;
	}

	// NOTE: never use direct call
	// because this method may call in thread which is different from Spy::instance
	QMetaObject::invokeMethod( &Spy::instance(), "registerObject", Q_ARG( QObject *, object ), Q_ARG( const QString &, id ) );
}

/**
 * @ingroup spy
 * @brief Get ID of a registered object
 * @param [in] object
 * @return ID of @p object
 * @sa spyGetObject spyRegisterObject
 *
 * This function can retrive ID which registered by spyRegisterObject.
 */
QString spyGetToken( QObject * object ) {
	return Spy::instance().getToken( object );
}

/**
 * @ingroup spy
 * @brief Get a registered object by ID
 * @param [in] token
 * @return object that ID is @p token
 * @sa spyGetToken spyRegisterObject
 *
 * This function can retrive object which registered by spyRegisterObject.
 */
QObject * spyGetObject( const QString & token ) {
	return Spy::instance().getObject( token );
}

/**
 * @ingroup spy
 * @brief Start record/replay
 * @sa spyIsTesting
 *
 * Without calling this function, record/replay won't start. Please call this
 * function as early as possible, e.g. in the main function.
 */
void spyTryTestAutomation() {
	Spy::instance().activate();
}

/**
 * @ingroup spy
 * @brief record/replay is start or not
 * @return true if started
 * @sa spyTryTestAutomation
 */
bool spyIsTesting() {
	return Spy::instance().isTesting();
}

QString spyMakeSafeName( const QString & objName ) {
	QString sName( objName );
	return sName.replace( QRegExp( "[^a-zA-Z0-9_]" ), "" );
}
