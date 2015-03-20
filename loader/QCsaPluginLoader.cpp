#include "QCsaPluginLoader.hpp"
#include "QASTNodeConvert.hpp"
#include "QAnnotatedTokenConvert.hpp"
#include "QCodeBase.hpp"

#include <QList>
#include <QScriptEngine>
#include <QFileInfo>

namespace csa{

QCsaPluginLoader::QCsaPluginLoader(const QString& path)
    : m_path(path)
{
    m_engine = new QScriptEngine;
    qScriptRegisterMetaType<ast::QASTNode*>(m_engine, &nodeToScriptValue, &nodeFromScriptValue);
    qScriptRegisterSequenceMetaType<QList<ast::QASTNode*> >(m_engine);

    qScriptRegisterMetaType<QAnnotatedToken*>(m_engine, &tokenToScriptValue, &tokenFromScriptValue);
    qScriptRegisterSequenceMetaType<QList<QAnnotatedToken*> >(m_engine);

    qScriptRegisterMetaType(m_engine, &tokenKindToScriptValue, &tokenKindFromScriptValue);

    QScriptValue metaObject = m_engine->newQMetaObject(
                &QAnnotatedToken::staticMetaObject, m_engine->newFunction(&tokenScriptConstructor) );
    m_engine->globalObject().setProperty( "Token", metaObject );
}

QCsaPluginLoader::~QCsaPluginLoader(){
    delete m_engine;
}

int QCsaPluginLoader::initializePlugins(){
    QDirIterator it(m_path, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        if ( it.fileInfo().suffix() == "js" ){
            int err = loadPlugin(it.filePath());
            if ( err != 0 )
                return err;
        }
    }
    return 0;
}

int QCsaPluginLoader::loadPlugin(const QString& path){

    QFile configScript(path);
    if ( !configScript.open(QIODevice::ReadOnly) ){
        qCritical(qPrintable(("Error opening js configuration file. Make sure the file is present in " + path + ".")));
        return 2;
    }

    QTextStream configStream(&configScript);
    m_engine->evaluate(configStream.readAll(), configScript.fileName());

    if ( m_engine->hasUncaughtException() ){
        int line = m_engine->uncaughtExceptionLineNumber();
        qCritical( (
            "Uncaught javascript exception at line " + QString::number(line) + ":" +
            m_engine->uncaughtException().toString()).toStdString().c_str());
        return 3;
    }

    return 0;
}

}// namespace
