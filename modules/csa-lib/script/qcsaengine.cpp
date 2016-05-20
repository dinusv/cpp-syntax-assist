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

#include "qcsaengine.h"
#include "qcsaconsole.h"
#include "qcodebase.h"
#include "qsourcelocation.h"
#include "qannotatedtoken.h"

#include <qqml.h>
#include <QList>
#include <QFileInfo>
#include <QJSEngine>
#include <QQmlEngine>
#include <QJSValue>
#include <QJSValueIterator>

namespace csa{

QCSAEngine::QCSAEngine(QJSEngine* engine, QObject* parent)
    : QObject(parent)
    , m_engine(engine)
{
    if ( !m_engine->globalObject().hasProperty("console") )
        setContextObject("console", &QCSAConsole::instance());
}

QCSAEngine::~QCSAEngine(){
}

bool QCSAEngine::loadNodeCollection(){

    QJSValue evaluateResult = m_engine->evaluate(
        "function NodeCollection(nodes){this.nodes = nodes ? nodes : [];} \n"

        "NodeCollection.prototype.size = function(){ \n"
        "    return this.nodes.length;\n"
        "}\n"

        "NodeCollection.prototype.each = function(eachcall){ \n"
        "    this.nodes.forEach(eachcall);\n"
        "}\n"

        "NodeCollection.prototype.append = function(node){ \n"
        "    if( node instanceof NodeCollection){ \n"
        "        for ( var i = 0; i < node.size(); ++i ) \n"
        "            this.nodes.push(node.nodes[i]); \n"
        "    } else {\n"
        "        this.nodes.push(node);\n"
        "    }\n"
        "}\n"

        "NodeCollection.prototype.children = function(){ \n"
        "    var newCollection = new NodeCollection(); \n"
        "    for ( var i = 0; i < this.nodes.length; ++i ){ \n"
        "        var nodeChildren = this.nodes[i].children(); \n"
        "        for ( var j = 0; j < nodeChildren.length; ++j ) \n"
        "            newCollection.nodes.push(nodeChildren[j]); \n"
        "    } \n"
        "    return newCollection; \n"
        "}\n"

        "NodeCollection.prototype.find = function(selector, typeName){ \n"
        "    if ( selector.indexOf('/', selector.length - 1) === -1 )\n"
        "       selector += '/';\n"
        "    var newCollection = new NodeCollection();\n"
        "    for ( var i = 0; i < this.nodes.length; ++i ){\n"
        "        var foundChildren = this.nodes[i].find(selector,(typeof typeName === 'undefined' ? '' : typeName)); \n"
        "        for ( var j = 0; j < foundChildren.length; ++j )\n"
        "            newCollection.nodes.push(foundChildren[j]);\n"
        "    }\n"
        "    return newCollection;\n"
        "}\n"

        "NodeCollection.prototype.remove = function(){ \n"
        "    for ( var i = 0; i < this.nodes.length; ++i ){\n"
        "        this.nodes[i].remove();\n"
        "    }\n"
        "}\n"

        "NodeCollection.prototype.toString = function(){ \n"
        "    var ret = 'NodeCollection['; \n"
        "    for ( var i = 0; i < this.nodes.length; ++i ){ \n"
        "        if ( i !== 0 ) \n"
        "            ret += ', '; \n"
        "        ret += '\\'' + this.nodes[i].declaration() + '\\''; \n"
        "    } \n"
        "    return ret + ']'; \n"
        "}\n"

        "NodeCollection.extend = function(options){\n"
        "    var target = NodeCollection.prototype;\n"
        "    for ( var name in options ){\n"
        "        if ( !options.hasOwnProperty(name) || options[name] === undefined )\n"
        "            continue;\n"
        "        var src = options[name];\n"
        "        var dst = target[name];\n"
        "        if ( target == src )\n"
        "            continue;\n"
        "        target[name] = src;\n"
        "    }\n"
        "    return NodeCollection;\n"
        "}\n"

        "NodeCollection.describe = function(options){\n"
        "    if( typeof plugins === 'undefined' ) \n"
        "        return;\n"
        "    var target = NodeCollection.prototype;\n"
        "    for ( var name in options ){\n"
        "        if ( !options.hasOwnProperty(name) || options[name] === undefined )\n"
        "            continue;\n"
        "        plugins.registerPlugin(name, options[name])\n"
        "    }\n"
        "    return NodeCollection;\n"
        "}\n"
    );

    if ( evaluateResult.isError() ){
        QCSAConsole::logError("Error loading node collection: " + evaluateResult.toString());
        return false;
    }
    return true;
}

bool QCSAEngine::loadNodesFunction(){

    QJSValue evaluateResult = m_engine->evaluate(
        "function nodes(selector, type){ \n"
        "    if ( selector instanceof String || typeof(selector) === 'string' ){\n"
        "       if ( selector.indexOf('/', selector.length - 1) === -1 )\n"
        "           selector += '/';\n"
        "       return new NodeCollection(codebase.find(selector, type || ''));\n"
        "    } else if ( selector instanceof Array ){\n"
        "       return new NodeCollection(selector)\n"
        "    } else {\n"
        "       return new NodeCollection([selector])\n"
        "    }\n"
        "}\n"

        "function selected(){"
        "   return nodes(NodeCollection.selectedPath || '')"
        "}"
    );

    if ( evaluateResult.isError() ){
        QCSAConsole::logError("Error loading nodes() function: " + evaluateResult.toString());
        return false;
    }
    return true;
}

bool QCSAEngine::loadFileFunctions(){

    QJSValue evaluateResult = m_engine->evaluate(
        "function createFile(file){ \n"
        "    return new NodeCollection(codebase.createFile(file)); \n"
        "}\n"

        "function parse(file){\n"
        "    var astFile = codebase.parseFile(file);\n"
        "    if (astFile)\n"
        "        return new NodeCollection(astFile);\n"
        "    return new NodeCollection();\n"
        "}\n"

        "function reparse(selector){ \n"
        "    if ( selector.indexOf('/', selector.length - 1) === -1 )\n"
        "       selector += '/';\n"
        "    var astFiles = codebase.find(selector, 'file');\n"
        "    var ncollect = new NodeCollection();\n"
        "    for (var i = 0; i < astFiles.length; ++i )\n"
        "        ncollect.nodes.push(codebase.reparseFile(astFiles[i]));\n"
        "    return ncollect; \n"
        "}\n"

        "function makePath(path){ \n"
        "    codebase.makePath(path); \n"
        "}\n"
    );

    if ( evaluateResult.isError() ){
        QCSAConsole::logError("Error loading file functions: " + evaluateResult.toString());
        return false;
    }
    return true;

}

int QCSAEngine::loadPlugins(const QString& path){
    QFileInfo fInfo(path);
    if ( !fInfo.exists() ){
        QCSAConsole::logError("Path does not exist: " + path + ".");
        return -1;
    }

    if ( fInfo.isDir() ){
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
        return loadFile(fInfo);
    }

    return 0;
}

int QCSAEngine::loadFile(const QString &path){
    return loadFile(QFileInfo(path));
}

int QCSAEngine::loadFile(const QFileInfo &file){
    QFile configScript(file.filePath());
    if ( !configScript.open(QIODevice::ReadOnly) ){
        QCSAConsole::logError(
            "Error opening js configuration file. Make sure the file is present in " + file.filePath() + ".");
        return 2;
    }

    QTextStream configStream(&configScript);

    QJSValue evaluateResult = m_engine->evaluate(configStream.readAll(), configScript.fileName());

    if ( evaluateResult.isError() ){
        QCSAConsole::logError("Uncaught javascript exception: " + evaluateResult.toString());
        return 3;
    }
    return 0;
}

bool QCSAEngine::execute(const QString& jsCode, QJSValue& result){
    if ( !m_engine )
        return false;

    result = m_engine->evaluate(jsCode);
    if ( result.isError() ){
        QCSAConsole::logError("Uncaught javascript exception: " + result.toString());
        return false;
    }

    return true;
}

void QCSAEngine::setContextObject(const QString& name, QObject* object){
    m_engine->globalObject().setProperty(name, m_engine->newQObject(object));
    QQmlEngine::setObjectOwnership(object, QQmlEngine::CppOwnership);
}

void QCSAEngine::setContextOwnedObject(const QString &name, QObject *object){
    m_engine->globalObject().setProperty(name, m_engine->newQObject(object));
}

void QCSAEngine::registerBaseTypes(){
    qmlRegisterUncreatableType<csa::QSourceLocation>(
        "CSA", 1, 0, "SourceLocation", "Source locations can be created from the codebase or ASTFiles.");
    qmlRegisterUncreatableType<csa::QAnnotatedToken>(
        "CSA", 1, 0, "Token", "Only access to Token properties of nodes is allowed.");
    qmlRegisterUncreatableType<csa::QCodebase>(
        "CSA", 1, 0, "Codebase", "Codebase is available only as a property.");
    qmlRegisterUncreatableType<csa::ast::QASTFile>(
        "CSA", 1, 0, "ASTFile", "ASTFile is available only as a property.");
    qmlRegisterUncreatableType<csa::ast::QASTNode>(
        "CSA", 1, 0, "ASTNode", "ASTNode is available only as a property.");
}

bool QCSAEngine::execute(const QString& jsCode){
    QJSValue result;
    return execute(jsCode, result);
}

}// namespace
