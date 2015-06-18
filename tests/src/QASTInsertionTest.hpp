#ifndef QASTINSERTIONTEST_HPP
#define QASTINSERTIONTEST_HPP

#include <QObject>
#include "QTestRunner.hpp"

namespace csa{
class QCSAScriptEngine;
}

class QASTInsertionTest : public QObject{

    Q_OBJECT
    Q_TEST_RUNNER_SUITE

public:
    explicit QASTInsertionTest(QObject *parent = 0);
    virtual ~QASTInsertionTest();

private slots:
    void initTestCase();
    void definedLocationInsertionTest();

private:
    csa::QCSAScriptEngine* m_engine;
    QString                m_parserTestPath;

};

#endif // QASTINSERTIONTEST_HPP
