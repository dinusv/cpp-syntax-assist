#include "testscenario.h"
#include "qcsatestscenario.h"
#include <QtTest/QTest>

using namespace csa;

TestScenario::TestScenario(const QString& testcase, csa::QCSATestScenario *scenario, QObject *parent)
    : QObject(parent)
    , m_scenario(scenario)
    , m_testcase(testcase)
{
    qDebug("\nDescribing \'%s\' -> Scenario \'%s\'", qPrintable(testcase), qPrintable(scenario->name()));
    qDebug() << "_____________________________________________________\n";
}

TestScenario::~TestScenario(){
}

void TestScenario::init(){
    QJSValue val = m_scenario->init();
    if ( val.isError() ){

    }
}

void TestScenario::runTest(){
    QJSValue val = m_scenario->runTest();
    if ( val.isError() ){
        qDebug() << "ERror" << val.toString();
        //HERE: See stack trace details, etc, fail properly
        QFAIL("FAILED");
    }
}

void TestScenario::cleanup(){
    QJSValue val = m_scenario->cleanup();
    if ( val.isError() ){

    }
}
