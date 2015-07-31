#ifndef QASTSEARCHTEST_HPP
#define QASTSEARCHTEST_HPP

#include <QObject>
#include "QTestRunner.hpp"

class QASTSearchTest : public QObject{

    Q_OBJECT
    Q_TEST_RUNNER_SUITE

public:
    explicit QASTSearchTest(QObject *parent = 0);
    virtual ~QASTSearchTest();

signals:

private slots:
    void initTestCase();
    void segmentMatchTest();
    void segmentPositionTest();
    void nodeSearchTest();

};

#endif // QASTSEARCHTEST_HPP
