#include "QTestRunner.hpp"
#include <QTest>

QList<QSharedPointer<QObject> > QTestRunner::m_tests;

int QTestRunner::registerTest(QObject* test){
    m_tests.append(QSharedPointer<QObject>(test));
    return m_tests.size() - 1;
}

int QTestRunner::runTests(int argc, char *argv[]){
    int code = 0;
    for ( QList<QSharedPointer<QObject> >::iterator it = m_tests.begin(); it != m_tests.end(); ++it ){
        code += QTest::qExec(it->data(), argc, argv);
    }
    return code;
}

int QTestRunner::runTest(int index, int argc, char* argv[]){
    if ( index > m_tests.size() )
        return -1;
    return QTest::qExec(m_tests[index].data(), argc, argv);
}
