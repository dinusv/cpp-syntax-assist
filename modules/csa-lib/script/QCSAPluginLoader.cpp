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
#include "QCSAConsole.hpp"
#include "QASTNodeConvert.hpp"
#include "QASTFileConvert.hpp"
#include "QSourceLocationConvert.hpp"
#include "QAnnotatedTokenConvert.hpp"
#include "QCodeBase.hpp"

#include <QList>
#include <QFileInfo>
#include <QJSEngine>
#include <QQmlEngine>
#include <QJSValue>
#include <QJSValueIterator>

namespace csa{

QCSAPluginLoader::QCSAPluginLoader(QJSEngine* engine, QObject* parent)
    : QObject(parent)
    , m_engine(engine)
{
    if ( !m_engine->globalObject().hasProperty("console") )
        setContextObject("console", &QCSAConsole::instance());
}

QCSAPluginLoader::~QCSAPluginLoader(){
}

bool QCSAPluginLoader::loadNodeCollection(){

    QJSValue evaluateResult = m_engine->evaluate(
        "function NodeCollection(nodes){this.nodes = nodes ? nodes : [];} \n"

        "NodeCollection.prototype.children = function(){ \n"
        "    var newCollection = new NodeCollection(); \n"
        "    for ( var i = 0; i < this.nodes.length; ++i ){ \n"
        "        var nodeChildren = this.nodes[i].children(); \n"
        "        for ( var j = 0; j < nodeChildren.length; ++j ) \n"
        "            newCollection.nodes.push(nodeChildren[j]); \n"
        "    } \n"
        "    return newCollection; \n"
        "} \n"

        "NodeCollection.prototype.find = function(selector, typeName){ \n"
        "    if ( selector.indexOf('/', selector.length - 1) === -1 )\n"
        "       selector += '/';\n"
        "    var newCollection = new NodeCollection(); \n"
        "    for ( var i = 0; i < this.nodes.length; ++i ){ \n"
        "        var foundChildren = this.nodes[i].find(selector,(typeof typeName === 'undefined' ? '' : typeName)); \n"
        "        for ( var j = 0; j < foundChildren.length; ++j ) \n"
        "            newCollection.nodes.push(foundChildren[j]); \n"
        "    } \n"
        "    return newCollection; \n"
        "} \n"

        "NodeCollection.prototype.remove = function(){ \n"
        "    for ( var i = 0; i < this.nodes.length; ++i ){\n"
        "        this.nodes[i].remove();\n"
        "    }\n"
        "    codeBase.save()\n"
        "} \n"

        "NodeCollection.prototype.toString = function(){ \n"
        "    var ret = 'NodeCollection['; \n"
        "    for ( var i = 0; i < this.nodes.length; ++i ){ \n"
        "        if ( i !== 0 ) \n"
        "            ret += ', '; \n"
        "        ret += '\\'' + this.nodes[i].content() + '\\''; \n"
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
        QCSAConsole::logError("Error loading node collection: " + evaluateResult.toString());
        return false;
    }
    return true;
}

bool QCSAPluginLoader::loadNodesFunction(){

    QJSValue evaluateResult = m_engine->evaluate(
        "function nodes(selector, type){ \n"
        "    if ( selector.indexOf('/', selector.length - 1) === -1 )\n"
        "       selector += '/';\n"
        "    return new NodeCollection(codeBase.find(selector, type ? type : '')); \n"
        "} \n"
    );

    if ( evaluateResult.isError() ){
        QCSAConsole::logError("Error loading nodes() function: " + evaluateResult.toString());
        return false;
    }
    return true;
}

bool QCSAPluginLoader::loadFileFunctions(){

    QJSValue evaluateResult = m_engine->evaluate(
        "function createFile(file){ \n"
        "    return new NodeCollection(codeBase.createFile(file)); \n"
        "}\n"

        "function parse(file){\n"
        "    var astFile = codeBase.parseFile(file);\n"
        "    if (astFile)\n"
        "        return new NodeCollection(astFile);\n"
        "    return new NodeCollection();\n"
        "}\n"

        "function reparse(selector){ \n"
        "    if ( selector.indexOf('/', selector.length - 1) === -1 )\n"
        "       selector += '/';\n"
        "    var astFiles = codeBase.find(selector, 'file');\n"
        "    var ncollect = new NodeCollection();\n"
        "    for (var i = 0; i < astFiles.length; ++i )\n"
        "        ncollect.nodes.push(codeBase.reparseFile(astFiles[i]));\n"
        "    return ncollect; \n"
        "}\n"

        "function makePath(path){ \n"
        "    codeBase.makePath(path); \n"
        "}\n"
    );

    if ( evaluateResult.isError() ){
        QCSAConsole::logError("Error loading file functions: " + evaluateResult.toString());
        return false;
    }
    return true;

}

int QCSAPluginLoader::loadPlugins(const QString& path){
    QJSEngine* engine = m_engine;

    QFileInfo fInfo(path);
    if ( !fInfo.exists() ){
        QCSAConsole::logError("Path does not exist: " + path + ".");
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
            QCSAConsole::logError(
                "Error opening js configuration file. Make sure the file is present in " + path + ".");
            return 2;
        }

        QTextStream configStream(&configScript);
        QJSValue evaluateResult = engine->evaluate(configStream.readAll(), configScript.fileName());

        if ( evaluateResult.isError() ){
            QCSAConsole::logError("Uncaught javascript exception: " + evaluateResult.toString());
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
        QCSAConsole::logError("Uncaught javascript exception: " + result.toString());
        return false;
    }

    return true;
}

void QCSAPluginLoader::setContextObject(const QString& name, QObject* object){
    m_engine->globalObject().setProperty(name, m_engine->newQObject(object));
    QQmlEngine::setObjectOwnership(object, QQmlEngine::CppOwnership);
}

void QCSAPluginLoader::setContextOwnedObject(const QString &name, QObject *object){
    m_engine->globalObject().setProperty(name, m_engine->newQObject(object));
}

bool QCSAPluginLoader::execute(const QString& jsCode){
    QJSValue result;
    return execute(jsCode, result);
}

}// namespace
