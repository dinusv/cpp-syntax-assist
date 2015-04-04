#include "QCsaScriptEngine.hpp"
#include "QASTNodeConvert.hpp"
#include "QSourceLocationConvert.hpp"
#include "QAnnotatedTokenConvert.hpp"
#include "QCodeBase.hpp"

#include <QList>
#include <QScriptEngine>
#include <QFileInfo>

namespace csa{

QCSAScriptEngine::QCSAScriptEngine(QObject* parent)
    : QObject(parent)
{
    m_engine = new QScriptEngine;
    qScriptRegisterMetaType<ast::QASTNode*>(m_engine, &nodeToScriptValue, &nodeFromScriptValue);
    qScriptRegisterSequenceMetaType<QList<ast::QASTNode*> >(m_engine);

    qScriptRegisterMetaType<QSourceLocation*>(m_engine, &sourceLocationToScriptValue, &sourceLocationFromScriptValue);

    qScriptRegisterMetaType<QAnnotatedToken*>(m_engine, &tokenToScriptValue, &tokenFromScriptValue);
    qScriptRegisterSequenceMetaType<QList<QAnnotatedToken*> >(m_engine);

    qScriptRegisterMetaType(m_engine, &tokenKindToScriptValue, &tokenKindFromScriptValue);

    QScriptValue metaObject = m_engine->newQMetaObject(
                &QAnnotatedToken::staticMetaObject, m_engine->newFunction(&tokenScriptConstructor) );
    m_engine->globalObject().setProperty( "Token", metaObject );
}

QCSAScriptEngine::~QCSAScriptEngine(){
    delete m_engine;
}

void QCSAScriptEngine::setCodeBase(QCodeBase* cBase){
    QScriptValue scriptCBase = m_engine->newQObject(cBase, QScriptEngine::QtOwnership);
    m_engine->globalObject().setProperty("codeBase", scriptCBase);
}

int QCSAScriptEngine::loadPlugins(const QString& path){
    QFileInfo fInfo(path);
    if ( !fInfo.exists() ){
        qCritical(qPrintable("Path does not exist: " + path + "."));
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
                qPrintable(("Error opening js configuration file. Make sure the file is present in " + path + ".")));
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
    }

    return 0;
}

bool QCSAScriptEngine::execute(const QString& jsCode, QScriptValue& result){
    if ( !m_engine )
        return false;

    result = m_engine->evaluate(jsCode);
    if ( m_engine->hasUncaughtException() ){
        qCritical( ("Uncaught javascript exception:" + result.toString()).toStdString().c_str() );
        return false;
    }

    return true;
}

bool QCSAScriptEngine::execute(const QString& jsCode){
    QScriptValue result;
    return execute(jsCode, result);
}

}// namespace
