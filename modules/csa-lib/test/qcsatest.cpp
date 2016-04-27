#include "qcsatest.h"
#include "qcsatestcase.h"
#include "qcsaengine.h"
#include "qcsaconsole.h"
#include <QJSValue>
#include <QJSEngine>
#include <QDirIterator>
#include <QDebug>

namespace csa{

//TODO add assertException

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
        "function assert(expr){ if ( !expr ) throw new Error(\"Assertion failed\"); }",
        result
    ))
    {
        QCSAConsole::logError("Error loading assertion functions: " + result.toString());
    }
}

QCSATest::~QCSATest(){
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

void QCSATest::describe(const QString &str, QJSValue val){
    if ( !val.isCallable() )
        return; //TODO: Manage error

    QCSATestCase* description = new QCSATestCase(str, m_scriptEngine, this);
    QJSValue arg = m_scriptEngine->engine()->newQObject(description);

    val.call(QJSValueList() << arg);

    m_testcases.append(description);
}

}// namespace
