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
