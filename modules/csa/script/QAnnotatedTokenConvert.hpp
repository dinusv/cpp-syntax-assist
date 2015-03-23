#ifndef QANNOTATEDTOKENCONVERT_HPP
#define QANNOTATEDTOKENCONVERT_HPP


#include <QScriptEngine>
#include <QScriptValue>
#include "QAnnotatedToken.hpp"

namespace csa{

QScriptValue Q_CSA_EXPORT tokenToScriptValue(QScriptEngine *engine, QAnnotatedToken* const & token);
void Q_CSA_EXPORT tokenFromScriptValue(const QScriptValue &obj, QAnnotatedToken* &token);
QScriptValue Q_CSA_EXPORT tokenScriptConstructor( QScriptContext* context, QScriptEngine* engine );

QScriptValue Q_CSA_EXPORT tokenKindToScriptValue(QScriptEngine *engine, const QAnnotatedToken::TokenKind &tokenKind);
void Q_CSA_EXPORT tokenKindFromScriptValue(const QScriptValue &obj, QAnnotatedToken::TokenKind &tokenKind);

}// namespaces

#endif // QANNOTATEDTOKENCONVERT_HPP
