/****************************************************************************
**
** Copyright (C) 2014 Dinu SV.
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


#include "QConfiguredEngine.hpp"
#include "QCodeBase.hpp"
#include "QASTNode.hpp"
#include <QtScript>
#include <QDebug>

namespace csa{

QConfiguredEngine::QConfiguredEngine(QQuickItem *parent)
    : QQuickItem(parent)
    , m_engine(0){

}

QConfiguredEngine::QConfiguredEngine(QScriptEngine* internal, QQuickItem *parent)
    : QQuickItem(parent)
    , m_engine(internal)
{
}

bool QConfiguredEngine::execute(const QString &jsCode){
    if ( !m_engine )
        return false;

    QScriptValue result = m_engine->evaluate(jsCode);
    if ( m_engine->hasUncaughtException() ){
        qCritical( ("Uncaught javascript exception:" + result.toString()).toStdString().c_str() );
        return false;
    }

    return true;
}

}// namespace
