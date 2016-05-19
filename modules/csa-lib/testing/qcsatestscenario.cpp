#include "qcsatestscenario.h"
#include "qcodebase.h"
#include "qcodebaseconfig.h"
#include "qcsaengine.h"
#include "qcsaconsole.h"
#include <QJSEngine>

namespace csa{

QCSATestScenario::QCSATestScenario(
        const QString& name,
        const QJSValue& fn,
        QCSAEngine* scriptEngine,
        QObject *parent)
    : QObject(parent)
    , m_name(name)
    , m_test(fn)
    , m_scriptEngine(scriptEngine)
    , m_codebase(0)
{
}

QCSATestScenario::~QCSATestScenario(){
    delete m_codebase;
}

void QCSATestScenario::setBeforeCall(const QJSValue &fn){
    m_before = fn;
}

void QCSATestScenario::setAfterCall(const QJSValue &fn){
    m_after = fn;
}

QJSValue QCSATestScenario::init(){
    if ( !m_before.isCallable() && !m_before.isUndefined() ){
        m_lastError = m_scriptEngine->engine()->evaluate(
            "throw new Error(\"Scenario \'" + name() + "\' init parameter is not a function.\");"
        );
    }
    if ( !m_test.isCallable() ){
        m_lastError = m_scriptEngine->engine()->evaluate(
            "throw new Error(\"Scenario \'" + name() + "\' test parameter is not a function.\");"
        );
    }
    if ( !m_after.isCallable() && !m_after.isUndefined() ){
        m_lastError = m_scriptEngine->engine()->evaluate(
            "throw new Error(\"Scenario \'" + name() + "\' cleanup parameter is not a function.\");"
        );
    }

    if ( hasError() )
        return m_lastError;

    if ( m_codebase ){
        delete m_codebase;
        m_codebase = 0;
    }

    QCodebaseConfig* cbconfig = new QCodebaseConfig(m_scriptEngine->engine(), QDir::currentPath());
    QJSValue configRoot;

    QJSValue configClangArgs = m_scriptEngine->engine()->newArray(3);
    configClangArgs.setProperty(0, "-c");
    configClangArgs.setProperty(1, "-x");
    configClangArgs.setProperty(2, "c++");

    QJSValue configHeaderPatterns = m_scriptEngine->engine()->newArray(2);
    configHeaderPatterns.setProperty(0, "*.h");
    configHeaderPatterns.setProperty(1, "*.hpp");
    QJSValue configSourcePatterns = m_scriptEngine->engine()->newArray(2);
    configSourcePatterns.setProperty(0, "*.c");
    configSourcePatterns.setProperty(1, "*.cpp");

    configRoot.setProperty("clangArgs", configClangArgs);
    configRoot.setProperty("headerFilePatterns", configHeaderPatterns);
    configRoot.setProperty("sourceFilePatterns", configSourcePatterns);

    cbconfig->update(configRoot);
    m_codebase = new QCodebase(cbconfig);
    m_scriptEngine->setContextObject("codebase", m_codebase);
    QJSValue result = m_before.call();
    if ( result.isError() )
        m_lastError = result;
    return result;
}

QJSValue QCSATestScenario::runTest(){
    if ( hasError() )
        return lastError();
    return m_test.call();
}

QJSValue QCSATestScenario::cleanup(){
    if ( hasError() )
        return lastError();

    QJSValue result = m_after.call();
    delete m_codebase;
    m_codebase = 0;
    return result;
}

void QCSATestScenario::stripError(const QJSValue &error, QString &message, QString &file, int &line, QString &lineText){
    message  = error.property("message").toString();
    file     = "";
    line     = 1;
    lineText = "";

    QString stackTrace = error.property("stack").toString();
    QStringList stackTraceList = stackTrace.split("@");
    if ( stackTraceList.length() > 0 ){
        QString lastStackTrace = stackTraceList.last();
        int splitIndex = lastStackTrace.lastIndexOf(":");
        if ( splitIndex != -1 ){
            file = lastStackTrace.mid(0, splitIndex);
            line = lastStackTrace.mid(splitIndex + 1).toInt();
            QFile inputFile(file);
            if ( inputFile.open(QIODevice::ReadOnly) ){
                QTextStream in(&inputFile);
                int currentLine = 1;
                while ( !in.atEnd() ){
                    if ( currentLine == line ){
                        lineText = in.readLine().trimmed();
                        return;
                    } else {
                        in.readLine();
                    }
                    ++currentLine;
                }
            }
        }
    }
}

}// namespace
