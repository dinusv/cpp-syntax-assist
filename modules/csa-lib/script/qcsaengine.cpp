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
#include "qcodebaseconfig.h"
#include "qsourcelocation.h"
#include "qannotatedtoken.h"
#include "qcsamodule.h"

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
        "    if ( !options.hasOwnProperty(\'name\') || options[\'name\'] === undefined )\n"
        "        return;\n"
        "    plugins.registerPlugin(options[\'name\'], options)\n"
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
        return loadFile(fInfo.absoluteFilePath());
    }

    return 0;
}

int QCSAEngine::loadFile(const QString &path, QCSAModule *from){
    QJSValue result = loadFile(QFileInfo(path), from);
    bool loadSuccess = result.isBool() ? result.toBool() : false;
    if ( !loadSuccess ){
        if ( result.isError() ){
            QCSAConsole::logError(
                "Failed to load module: \'" + path + "\'. Exception: " + result.property("message").toString()
            );
            return -1;
        } else if ( result.hasProperty("type") && result.property("type").toString() == "error"){
            QCSAConsole::logError(
                "Failed to load module: \'" + path + "\'. Exception: " + result.property("message").toString()
            );
            return result.property("number").toInt();
        }
    }
    return 0;
}

QJSValue QCSAEngine::loadFile(const QFileInfo &finfo, QCSAModule *){
    if ( !finfo.exists() || finfo.isDir() )
        return generateError("No such module found.", 1);

    QString filename = finfo.fileName();
    QString filepath = finfo.absolutePath();

    QCSAModule* module = 0;
    for ( QList<QCSAModule*>::ConstIterator it = m_loadedModules.begin(); it != m_loadedModules.end(); ++it ){
        if ( (*it)->filename() == filename && (*it)->dirname() == filepath )
            module = *it;
    }
    if( module )
        return module->exports();

    for ( QLinkedList<QCSAModule*>::iterator it = m_trace.begin(); it != m_trace.end(); ++it ){
        if ( filename == (*it)->filename() && filepath == (*it)->dirname() ){
            QString requireTrace;
            for ( QLinkedList<QCSAModule*>::iterator cit = m_trace.begin(); cit != m_trace.end(); ++cit )
                requireTrace += (*cit)->filename() + " -> ";
            return generateError("Loop detected on requiring: " + filename + ". Trace: " + requireTrace + filename, 2);
        }
    }

    QFile configScript(finfo.absoluteFilePath());
    if ( !configScript.open(QIODevice::ReadOnly) ){
        QCSAConsole::logError(
            "Error opening file. Make sure the file is present in " + finfo.filePath() + ".");
        return 2;
    }
    QTextStream configStream(&configScript);

    module = new QCSAModule(filename, filepath, this);
    QString moduleHeader =
        "(function(exports, module, __filename, __dirname){"
        "   var require = function(name){ var exports = module.load(name);"
        "       if ( exports.hasOwnProperty(\'type\') && exports.hasOwnProperty(\'message\') && exports['type'] === \'error\')"
        "           throw new Error(exports[\'message\']); return exports;"
        "   }\n";
    QString moduleFooter =
        "});";

    QJSValue evaluateResult =
        m_engine->evaluate(moduleHeader + configStream.readAll() + moduleFooter, configScript.fileName(), 0);
    if ( evaluateResult.isError() )
        return generateError(evaluateResult.property("message").toString(), 3);

    m_trace.append(module);

    QJSValue callResult = module->evaluateExports(evaluateResult);
    if ( callResult.isError() ){
        delete m_trace.takeLast();
        return generateError(callResult.property("message").toString(), 4);
    }

    m_loadedModules.append(module);
    m_trace.removeLast();
    return module->exports();
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

void QCSAEngine::selectNode(const QString &breadCrumbs){
    m_engine->globalObject().property("NodeCollection").setProperty("selectedPath", breadCrumbs);
}

void QCSAEngine::setContextObject(const QString& name, QObject* object){
    m_engine->globalObject().setProperty(name, m_engine->newQObject(object));
    QQmlEngine::setObjectOwnership(object, QQmlEngine::CppOwnership);
}

void QCSAEngine::setContextOwnedObject(const QString &name, QObject *object){
    m_engine->globalObject().setProperty(name, m_engine->newQObject(object));
}

void QCSAEngine::registerBaseTypes(){
    qmlRegisterUncreatableType<csa::QCSAModule>(
        "CSA", 1, 0, "Module", "Module constructor is private.");
}

void QCSAEngine::registerASTTypes(){
    qmlRegisterUncreatableType<csa::QSourceLocation>(
        "CSA", 1, 0, "SourceLocation", "Source locations can be created from the codebase or ASTFiles.");
    qmlRegisterUncreatableType<csa::QAnnotatedToken>(
        "CSA", 1, 0, "Token", "Only access to Token properties of nodes is allowed.");
    qmlRegisterUncreatableType<csa::QCodebase>(
        "CSA", 1, 0, "Codebase", "Codebase is available only as a property.");
    qmlRegisterUncreatableType<csa::QCodebaseConfig>(
        "CSA", 1, 0, "CodebaseConfig", "Codebase config is available only as a property");
    qmlRegisterUncreatableType<csa::ast::QASTFile>(
        "CSA", 1, 0, "ASTFile", "ASTFile is available only as a property.");
    qmlRegisterUncreatableType<csa::ast::QASTNode>(
        "CSA", 1, 0, "ASTNode", "ASTNode is available only as a property.");
}

bool QCSAEngine::execute(const QString& jsCode){
    QJSValue result;
    return execute(jsCode, result);
}

QJSValue QCSAEngine::generateError(const QString &message, int number){
    QJSValue error = m_engine->newObject();
    error.setProperty("number", number );
    error.setProperty("type", "error");
    error.setProperty("message", message);
    return error;
}

}// namespace
