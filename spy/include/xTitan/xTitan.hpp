/**
 * @file xTitan.hpp
 */
#ifndef XTITAN_CLIENT_XTITAN_HPP
#define XTITAN_CLIENT_XTITAN_HPP

#include <QtCore/QObject>

#include "xTitan/Utility/SpyInput.hpp"
#include "xTitan/Utility/SpyCheck.hpp"
#include "xTitan/Utility/SpyAsyncCheck.hpp"


/**
 * @brief Records function call
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
#define spyInput xtitan::detail::SpyInput(spyGetToken(this).toUtf8(),__FUNCTION__)
/**
 * @brief Creates synchronized check point
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
#define spyCheck xtitan::detail::SpyCheck(__FILE__,__LINE__)
/**
 * @brief Creates asynchronized check point
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
#define spyAsyncCheck xtitan::detail::SpyAsyncCheck(__FILE__,__LINE__)

XTITAN_SPY_DLL void spyRegisterObject(QObject * object, const QString & id);

XTITAN_SPY_DLL QString spyGetToken(QObject * object);
XTITAN_SPY_DLL QObject * spyGetObject(const QString & token);

XTITAN_SPY_DLL void spyTryTestAutomation();

XTITAN_SPY_DLL bool spyIsTesting();

XTITAN_SPY_DLL QString spyMakeSafeName(const QString & objName);

#endif
