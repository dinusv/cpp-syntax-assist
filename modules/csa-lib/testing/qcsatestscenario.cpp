#include "qcsatestscenario.h"
#include "qcodebase.h"
#include "qcodebaseconfig.h"
#include "qcsaengine.h"
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
}

void QCSATestScenario::setBeforeCall(const QJSValue &fn){
    m_before = fn;
}

void QCSATestScenario::setAfterCall(const QJSValue &fn){
    m_after = fn;
}

QJSValue QCSATestScenario::init(){
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
    return m_before.call();
}

QJSValue QCSATestScenario::runTest(){
    return m_test.call();

}

QJSValue QCSATestScenario::cleanup(){
    QJSValue result = m_after.call();
    delete m_codebase;
    m_codebase = 0;
    return result;
}

}// namespace
