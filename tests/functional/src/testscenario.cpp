#include "testscenario.h"
#include "qcsatestscenario.h"
#include "qcsaconsole.h"
#include <QtTest/QTest>

using namespace csa;

TestScenario::TestScenario(const QString& testcase, csa::QCSATestScenario *scenario, QObject *parent)
    : QObject(parent)
    , m_scenario(scenario)
    , m_testcase(testcase)
{
    QString message   = "Describing \'" + testcase + "\' -> Scenario \'" + scenario->name() + "\'";
    QString separator = "________________________________________________________________________";
    QCSAConsole::instance().log("\n" + message);
    QCSAConsole::instance().log(separator + "\n");
}

TestScenario::~TestScenario(){
}

void TestScenario::init(){
    QJSValue val = m_scenario->init();
    if ( val.isError() ){
        QFAIL(qPrintable(createErrorMessage(val)));
    }
}

void TestScenario::runTest(){
    QJSValue val = m_scenario->runTest();
    if ( val.isError() ){
        QFAIL(qPrintable(createErrorMessage(val)));
    }
}

void TestScenario::cleanup(){
    QJSValue val = m_scenario->cleanup();
    if ( val.isError() ){
        QFAIL(qPrintable(createErrorMessage(val)));
    }
}

QString TestScenario::createErrorMessage(const QJSValue &error){
    QString message, file, lineText;
    int line;

    QCSATestScenario::stripError(error, message, file, line, lineText);
    return message + ": " + lineText + (lineText.isEmpty() ? "" : "\n") + file + ":" + QString::number(line);
}
