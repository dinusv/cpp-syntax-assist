#include "QSourceLocationConvert.hpp"

namespace csa{

QScriptValue sourceLocationToScriptValue(QScriptEngine* engine, QSourceLocation* const & location){
    QScriptValue obj = engine->newQObject(location, QScriptEngine::QtOwnership);
    return obj;
}

void sourceLocationFromScriptValue(const QScriptValue& obj, QSourceLocation*& location){
    location = qobject_cast<QSourceLocation*>(obj.toQObject());
}

}// namespace
