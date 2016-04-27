#ifndef QASTINSERTIONTEST_HPP
#define QASTINSERTIONTEST_HPP

#include <QObject>
#include "QTestRunner.hpp"

class QJSEngine;

namespace csa{
class QCSAPluginLoader;
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
    QJSEngine*             m_engine;
    csa::QCSAPluginLoader* m_pluginLoader;
    QString                m_parserTestPath;

};

#endif // QASTINSERTIONTEST_HPP
