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

class QJSValue;
class QJSEngine;

namespace csa{

class QCodeBase;

class Q_CSA_EXPORT QCSAPluginConsole : public QObject{

    Q_OBJECT

public:
    QCSAPluginConsole(QObject* parent = 0);
    ~QCSAPluginConsole();

public slots:
    void log(const QString& message);
    void warn(const QString& message);
    void error(const QString& message);
};

class Q_CSA_EXPORT QCSAPluginLoader : public QObject{

    Q_OBJECT

public:
    QCSAPluginLoader(QJSEngine* engine, QObject* parent = 0);
    ~QCSAPluginLoader();

    QJSEngine* engine();

    bool loadNodeCollection();
    bool loadNodesFunction();
    int loadPlugins(const QString &path);

    bool execute(const QString &jsCode, QJSValue& result);

    void setContextObject(const QString& name, QObject* object);
    void setContextOwnedObject(const QString& name, QObject* object);

public slots:
    bool execute(const QString& jsCode);

private:
    QJSEngine*          m_engine;
    QCSAPluginConsole* m_pluginDebugger;
};

inline QJSEngine* QCSAPluginLoader::engine(){
    return m_engine;
}

}// namespace

#endif // QCSAPLUGINLOADER_HPP
