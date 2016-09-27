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

#ifndef QCSAENGINE_H
#define QCSAENGINE_H

#include "qcsaglobal.h"
#include <QString>
#include <QObject>
#include <QLinkedList>

class QJSValue;
class QJSEngine;
class QFileInfo;

namespace csa{

class QCodebase;
class QCSAModule;

class Q_CSA_EXPORT QCSAEngine : public QObject{

    Q_OBJECT

public:
    QCSAEngine(QJSEngine* engine, QObject* parent = 0);
    ~QCSAEngine();

    QJSEngine* engine();

    bool loadNodeCollection();
    bool loadNodesFunction();
    bool loadFileFunctions();

    int loadPlugins(const QString &path);
    int loadFile(const QString& path, QCSAModule* from = 0);
    QJSValue loadFile(const QFileInfo& file, QCSAModule* from = 0);

    bool execute(const QString &jsCode, QJSValue& result);

    void setContextObject(const QString& name, QObject* object);
    void setContextOwnedObject(const QString& name, QObject* object);

    static void registerBaseTypes();
    static void registerASTTypes();

public slots:
    bool execute(const QString& jsCode);
    void selectNode(const QString& breadCrumbs);

private:
    QJSValue generateError(const QString& message, int number);

    QLinkedList<QCSAModule*> m_trace;
    QList<QCSAModule*>       m_loadedModules;

    QJSEngine*  m_engine;
};

inline QJSEngine* QCSAEngine::engine(){
    return m_engine;
}

}// namespace

#endif // QCSAPLUGINLOADER_HPP
