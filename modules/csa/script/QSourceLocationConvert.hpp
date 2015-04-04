#ifndef QSOURCELOCATIONCONVERT_HPP
#define QSOURCELOCATIONCONVERT_HPP

#include <QScriptEngine>
#include <QScriptValue>
#include "QSourceLocation.hpp"

namespace csa{

// JS Conversion
// -------------

QScriptValue Q_CSA_EXPORT sourceLocationToScriptValue(QScriptEngine *engine, QSourceLocation* const & location);
void Q_CSA_EXPORT sourceLocationFromScriptValue(const QScriptValue &obj, QSourceLocation* &location);

}// namespace


#endif // QSOURCELOCATIONCONVERT_HPP
