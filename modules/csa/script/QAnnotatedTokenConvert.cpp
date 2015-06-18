/****************************************************************************
**
** Copyright (C) 2014-2015 Dinu SV.
** (contact: mail@dinusv.com)
** This file is part of C++ Snippet Assist application.
**
** GNU General Public License Usage
** 
** This file may be used under the terms of the GNU General Public License 
** version 3.0 as published by the Free Software Foundation and appearing 
** in the file LICENSE.GPL included in the packaging of this file.  Please 
** review the following information to ensure the GNU General Public License 
** version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.
**
****************************************************************************/


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
