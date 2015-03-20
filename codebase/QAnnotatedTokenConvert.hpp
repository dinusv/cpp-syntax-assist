#ifndef QANNOTATEDTOKENCONVERT_HPP
#define QANNOTATEDTOKENCONVERT_HPP


#include <QScriptEngine>
#include <QScriptValue>
#include "QAnnotatedToken.hpp"

namespace csa{

QScriptValue tokenToScriptValue(QScriptEngine *engine, QAnnotatedToken* const & token);
void tokenFromScriptValue(const QScriptValue &obj, QAnnotatedToken* &token);
inline QScriptValue tokenScriptConstructor( QScriptContext* context, QScriptEngine* engine ){
    QAnnotatedToken* token = new QAnnotatedToken(context->argument(0).toQObject());
    return engine->newQObject(token, QScriptEngine::ScriptOwnership);
}

inline QScriptValue tokenKindToScriptValue(QScriptEngine *engine, const QAnnotatedToken::TokenKind &tokenKind){
    return engine->newVariant((int)tokenKind);
}

inline void tokenKindFromScriptValue(const QScriptValue &obj, QAnnotatedToken::TokenKind &tokenKind){
    tokenKind = (QAnnotatedToken::TokenKind)obj.toInt32();
}


}// namespaces

#endif // QANNOTATEDTOKENCONVERT_HPP
