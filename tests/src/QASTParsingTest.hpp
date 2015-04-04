#ifndef QASTPARSINGTEST_HPP
#define QASTPARSINGTEST_HPP

#include <QObject>
#include "QTestRunner.hpp"

namespace csa{
class QCSAScriptEngine;
}

class QASTParsingTest : public QObject{

    Q_OBJECT
    Q_TEST_RUNNER_SUITE

public:
    explicit QASTParsingTest(QObject *parent = 0);
    virtual ~QASTParsingTest();

private slots:
    void unknownTypeDeduction();

private:
    void init();

    csa::QCSAScriptEngine* m_engine;

    int     m_parserEngineCode;
    QString m_parserTestPath;

};

#endif // QASTPARSINGTEST_HPP
