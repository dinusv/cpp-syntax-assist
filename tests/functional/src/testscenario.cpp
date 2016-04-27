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

void TestScenario::stripError(const QJSValue &error, QString &message, QString &file, int &line, QString& lineText){
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

QString TestScenario::createErrorMessage(const QJSValue &error){
    QString message, file, lineText;
    int line;

    stripError(error, message, file, line, lineText);
    return message + ": " + lineText + (lineText.isEmpty() ? "" : "\n") + file + ":" + QString::number(line);
}
