#include "QAnnotatedTokenConvert.hpp"

namespace csa{


QScriptValue tokenToScriptValue(QScriptEngine* engine, QAnnotatedToken* const & token){
    QScriptValue obj = engine->newQObject(token, QScriptEngine::QtOwnership);
    return obj;
}

void tokenFromScriptValue(const QScriptValue& obj, QAnnotatedToken*& token){
    token = qobject_cast<QAnnotatedToken*>(obj.toQObject());
}

QScriptValue tokenScriptConstructor(QScriptContext* context, QScriptEngine* engine){
    QAnnotatedToken* token = new QAnnotatedToken(context->argument(0).toQObject());
    return engine->newQObject(token, QScriptEngine::ScriptOwnership);
}

QScriptValue tokenKindToScriptValue(QScriptEngine* engine, const QAnnotatedToken::TokenKind& tokenKind){
    return engine->newVariant((int)tokenKind);
}

void tokenKindFromScriptValue(const QScriptValue& obj, QAnnotatedToken::TokenKind& tokenKind){
    tokenKind = (QAnnotatedToken::TokenKind)obj.toInt32();
}

}// namespace
