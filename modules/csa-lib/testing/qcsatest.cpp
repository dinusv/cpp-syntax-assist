#include "qcsatest.h"
#include "qcsatestcase.h"
#include "qcsatestscenario.h"

#include "qcsaengine.h"
#include "qcsaconsole.h"

#include <qqml.h>
#include <QJSValue>
#include <QJSEngine>
#include <QDirIterator>
#include <QDebug>

namespace csa{

QCSATest::QCSATest(QObject *parent)
    : QObject(parent)
    , m_scriptEngine(new QCSAEngine(new QJSEngine))
{
    m_scriptEngine->loadNodeCollection();
    m_scriptEngine->loadNodesFunction();
    m_scriptEngine->loadFileFunctions();
    m_scriptEngine->setContextObject("CSATest", this);

    QJSValue result;
    if ( !m_scriptEngine->execute(
        "function assert(expr){ if ( !expr ) throw new Error(\"Assertion failed\"); }\n"
        "function assertThrows(block, errorType){\n"
            "var setErrorType = errorType || Error, assertion = false;\n"
            "try{ block() } catch ( error ){ if ( error instanceof setErrorType ) assertion = true; }\n"
            "if ( !assertion ) throw new Error(\"Throws assertion failed.\");\n"
        "}",
        result
    ))
    {
        QCSAConsole::logError("Error loading assertion functions: " + result.toString());
    }
}

QCSATest::~QCSATest(){
    for ( QCSATest::Iterator it = m_testcases.begin(); it != m_testcases.end(); ++it )
        delete *it;
    m_testcases.clear();
}

QCSATestCase *QCSATest::findTestCase(const QString &name){
    for ( QCSATest::Iterator it = m_testcases.begin(); it != m_testcases.end(); ++it )
        if ( (*it)->name() == name )
            return *it;
    return 0;
}

int QCSATest::loadTestFile(const QString &path){
    return m_scriptEngine->loadFile(path);
}

int QCSATest::loadTests(const QString &path){
    QFileInfo fInfo(path);
    if ( !fInfo.exists() ){
        QCSAConsole::logError("Path does not exist: " + path + ".");
        return -1;
    }

    if ( QFileInfo(path).isDir() ){
        QDirIterator it(path, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            it.next();
            if ( it.fileInfo().suffix() == "js" && it.fileInfo().baseName().endsWith("test") ){
                int err = loadTests(it.filePath());
                if ( err != 0 )
                    return err;
            }
        }
    } else {
        return loadTestFile(path);
    }

    return 0;
}

void QCSATest::registerTestingTypes(){
    qmlRegisterUncreatableType<csa::QCSATest>(
        "CSA", 1, 0, "Test", "CSATest is available only as a property.");
    qmlRegisterUncreatableType<csa::QCSATestCase>(
        "CSA", 1, 0, "TestCase", "CSATestCase is available only as a property.");
    qmlRegisterUncreatableType<csa::QCSATestScenario>(
        "CSA", 1, 0, "TestScenario", "CSATestScenario is available only as a property.");
}

void QCSATest::describe(const QString &str, QJSValue val){
    if ( !val.isCallable() ){
        QCSAConsole::logError("Description \'" + str + "\' is not a function.");
        return;
    }

    QCSATestCase* description = new QCSATestCase(str, m_scriptEngine, this);
    QJSValue arg = m_scriptEngine->engine()->newQObject(description);

    val.call(QJSValueList() << arg);

    m_testcases.append(description);
}

}// namespace
