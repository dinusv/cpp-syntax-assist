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
    qWarning("Script error: %s", qPrintable(message));
}



QCSAPluginLoader::QCSAPluginLoader(QJSEngine* engine, QObject* parent)
    : QObject(parent)
    , m_engine(engine)
    , m_pluginDebugger(new QCSAPluginDebugger)
{
    setContextObject("debug", m_pluginDebugger);
//    m_engine = new QScriptEngine;
//    qScriptRegisterMetaType<ast::QASTNode*>(m_engine, &nodeToScriptValue, &nodeFromScriptValue);
//    qScriptRegisterSequenceMetaType<QList<ast::QASTNode*> >(m_engine);

//    qScriptRegisterMetaType<ast::QASTFile*>(m_engine, &fileToScriptValue, &fileFromScriptValue);
//    qScriptRegisterSequenceMetaType<QList<ast::QASTFile*> >(m_engine);

//    qScriptRegisterMetaType<QSourceLocation*>(m_engine, &sourceLocationToScriptValue, &sourceLocationFromScriptValue);

//    qScriptRegisterMetaType<QAnnotatedToken*>(m_engine, &tokenToScriptValue, &tokenFromScriptValue);
//    qScriptRegisterSequenceMetaType<QList<QAnnotatedToken*> >(m_engine);

//    qScriptRegisterMetaType(m_engine, &tokenKindToScriptValue, &tokenKindFromScriptValue);

//    QScriptValue metaObject = m_engine->newQMetaObject(
//                &QAnnotatedToken::staticMetaObject, m_engine->newFunction(&tokenScriptConstructor) );
//    m_engine->globalObject().setProperty("Token", metaObject);
}

QCSAPluginLoader::~QCSAPluginLoader(){
    delete m_pluginDebugger;
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

        qDebug() << configScript.fileName();


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
