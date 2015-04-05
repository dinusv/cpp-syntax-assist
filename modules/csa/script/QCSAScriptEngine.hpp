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


#ifndef QCSAPLUGINLOADER_HPP
#define QCSAPLUGINLOADER_HPP

#include "QCSAGlobal.hpp"
#include <QString>
#include <QObject>

class QScriptEngine;
class QScriptValue;

namespace csa{

class QCodeBase;
class Q_CSA_EXPORT QCSAScriptEngine : public QObject{

    Q_OBJECT

public:
    QCSAScriptEngine(QObject* parent = 0);
    ~QCSAScriptEngine();

    void setCodeBase(QCodeBase* base);

    QScriptEngine* engine();
    int loadPlugins(const QString& path);

    bool execute(const QString &jsCode, QScriptValue& result);

public slots:
    bool execute(const QString& jsCode);

private:
    QScriptEngine* m_engine;
};

inline QScriptEngine* QCSAScriptEngine::engine(){
    return m_engine;
}

}// namespace

#endif // QCSAPLUGINLOADER_HPP
