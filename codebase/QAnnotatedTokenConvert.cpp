#include "QAnnotatedTokenConvert.hpp"

namespace csa{


QScriptValue tokenToScriptValue(QScriptEngine* engine, QAnnotatedToken* const & token){
    QScriptValue obj = engine->newQObject(token, QScriptEngine::QtOwnership);
    return obj;
}

void tokenFromScriptValue(const QScriptValue& obj, QAnnotatedToken*& token){
    token = qobject_cast<QAnnotatedToken*>(obj.toQObject());
}

}// namespace
