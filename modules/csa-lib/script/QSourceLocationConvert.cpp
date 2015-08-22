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
