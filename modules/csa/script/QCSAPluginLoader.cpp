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

#include "QCSAPluginLoader.hpp"
#include "QASTNodeConvert.hpp"
#include "QASTFileConvert.hpp"
#include "QSourceLocationConvert.hpp"
#include "QAnnotatedTokenConvert.hpp"
#include "QCodeBase.hpp"

#include <QList>
#include <QFileInfo>
#include <QJSEngine>
#include <QJSValue>
#include <QJSValueIterator>

namespace csa{

QCSAPluginDebugger::QCSAPluginDebugger(QObject* parent)
    : QObject(parent)
{
}

QCSAPluginDebugger::~QCSAPluginDebugger(){
}

void QCSAPluginDebugger::print(const QString& message){
    qDebug() << message;
}

void QCSAPluginDebugger::printError(const QString &message){
    qCritical("Script error: %s", qPrintable(message));
}


QCSAPluginLoader::QCSAPluginLoader(QJSEngine* engine, QObject* parent)
    : QObject(parent)
    , m_engine(engine)
    , m_pluginDebugger(new QCSAPluginDebugger)
{
    setContextObject("debug", m_pluginDebugger);
}

QCSAPluginLoader::~QCSAPluginLoader(){
    delete m_pluginDebugger;
}

bool QCSAPluginLoader::loadNodeCollection(){

    QJSValue evaluateResult = m_engine->evaluate(
        "function NodeCollection(nodes){this.nodes = nodes ? nodes : [];} \n"

        "NodeCollection.prototype.children = function(){ \n"
        "    var newCollection = new NodeCollection(); \n"
        "    for ( var i = 0; i < this.nodes.length; ++i ) \n"
        "        newCollection.nodes.push(nodes[i].children()); \n"
        "    return newCollection; \n"
        "} \n"

        "NodeCollection.prototype.toString = function(){ \n"
        "    var ret = 'NodeCollection['; \n"
        "    for ( var i = 0; i < this.nodes.length; ++i ){ \n"
        "        if ( i !== 0 ) \n"
        "            ret += ', '; \n"
        "        ret += '\'' + this.nodes[i].content() + '\''; \n"
        "    } \n"
        "    return ret + ']'; \n"
        "} \n"

        "NodeCollection.registerPlugin = function(properties){ \n"
        "    if( typeof plugins !== 'undefined' ) \n"
        "        plugins.registerPlugin(properties); \n"
        "    return NodeCollection; \n"
        "} \n"
    );


    if ( evaluateResult.isError() ){
        qCritical("Error loading node collection: %s", qPrintable(evaluateResult.toString()));
        return false;
    }
    return true;
}

bool QCSAPluginLoader::loadNodesFunction(){

    QJSValue evaluateResult = m_engine->evaluate(
        "function nodes(selector, type){ \n"
        "    return new NodeCollection(codeBase.find(selector, type ? type : '')); \n"
        "} \n"
    );

    if ( evaluateResult.isError() ){
        qCritical("Error loading nodes() function: %s", qPrintable(evaluateResult.toString()));
        return false;
    }
    return true;
}

int QCSAPluginLoader::loadPlugins(const QString& path){
    QJSEngine* engine = m_engine;

    QFileInfo fInfo(path);
    if ( !fInfo.exists() ){
        qCritical("%s", qPrintable("Path does not exist: " + path + "."));
        return -1;
    }

    if ( QFileInfo(path).isDir() ){
        QDirIterator it(path, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            it.next();
            if ( it.fileInfo().suffix() == "js" ){
                int err = loadPlugins(it.filePath());
                if ( err != 0 )
                    return err;
            }
        }
    } else {
        QFile configScript(path);
        if ( !configScript.open(QIODevice::ReadOnly) ){
            qCritical(
                "%s", qPrintable(("Error opening js configuration file. Make sure the file is present in " + path + ".")));
            return 2;
        }

        QTextStream configStream(&configScript);
        QJSValue evaluateResult = engine->evaluate(configStream.readAll(), configScript.fileName());

        if ( evaluateResult.isError() ){
            qCritical("Uncaught javascript exception: %s", qPrintable(evaluateResult.toString()));
            return 3;
        }
    }

    return 0;
}

bool QCSAPluginLoader::execute(const QString& jsCode, QJSValue& result){
    if ( !m_engine )
        return false;

    result = m_engine->evaluate(jsCode);
    if ( result.isError() ){
        qCritical("Uncaught javascript exception: %s", qPrintable(result.toString()));
        return false;
    }

    return true;
}

void QCSAPluginLoader::setContextObject(const QString& name, QObject* object){
    m_engine->globalObject().setProperty(name, m_engine->newQObject(object));
}

bool QCSAPluginLoader::execute(const QString& jsCode){
    QJSValue result;
    return execute(jsCode, result);
}

}// namespace
