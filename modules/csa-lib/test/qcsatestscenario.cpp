#include "qcsatestscenario.h"
#include "QCodeBase.hpp"
#include "QCSAPluginLoader.hpp"

namespace csa{

QCSATestScenario::QCSATestScenario(
        const QString& name,
        const QJSValue& fn,
        QCSAPluginLoader* scriptEngine,
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
    const char* args[] = {"-c", "-x", "c++"};
    m_codebase = new QCodebase(args, 3);
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
