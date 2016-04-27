#include "qcsatestcase.h"
#include "qcsatestscenario.h"
#include "qcsaengine.h"

namespace csa{

QCSATestCase::QCSATestCase(const QString &name, QCSAEngine* scriptEngine, QObject *parent)
    : QObject(parent)
    , m_name(name)
    , m_scriptEngine(scriptEngine)
{
}

QCSATestCase::~QCSATestCase(){
}


void QCSATestCase::beforeScenario(const QJSValue& fn){
    if ( !fn.isCallable() )
        return; //TODO: Error
    m_beforeScenario = fn;
    for ( QList<QCSATestScenario*>::iterator it = m_scenarios.begin(); it != m_scenarios.end(); ++it ){
        (*it)->setBeforeCall(m_beforeScenario);
    }
}

void QCSATestCase::afterScenario(const QJSValue &fn){
    if ( !fn.isCallable() )
        return; // TOOD: Error
    m_afterScenario = fn;
    for ( QList<QCSATestScenario*>::iterator it = m_scenarios.begin(); it != m_scenarios.end(); ++it ){
        (*it)->setAfterCall(m_beforeScenario);
    }
}

void QCSATestCase::scenario(const QString &name, const QJSValue &fn){
    if ( !fn.isCallable() )
        return;

    QCSATestScenario* scn = new QCSATestScenario(name, fn, m_scriptEngine, this);
    scn->setBeforeCall(m_beforeScenario);
    scn->setAfterCall(m_afterScenario);
    m_scenarios.append(scn);
}

}// namespace
