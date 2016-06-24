#ifndef QASTSEARCHTEST_HPP
#define QASTSEARCHTEST_HPP

#include <QObject>
#include "qtestrunner.h"

class QASTSearchTest : public QObject{

    Q_OBJECT
    Q_TEST_RUNNER_SUITE

public:
    explicit QASTSearchTest(QObject *parent = 0);
    virtual ~QASTSearchTest();

private slots:
    void initTestCase();
    void segmentMatchTest();
    void nodeSearchTest();

};

#endif // QASTSEARCHTEST_HPP
