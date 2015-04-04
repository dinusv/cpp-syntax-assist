#ifndef QTESTRUNNER_HPP
#define QTESTRUNNER_HPP

#include <QObject>
#include <QList>
#include <QSharedPointer>

class QTestRunner{

public:
    static int registerTest(QObject* test);
    static int runTests(int argc, char *argv[]);
    static int runTest(int index, int argc, char* argv[]);
    static int totalRegisteredTests();

private:
    static QList<QSharedPointer<QObject> > m_tests;
};

inline int QTestRunner::totalRegisteredTests(){
    return m_tests.size();
}

#define Q_TEST_RUNNER_SUITE \
    public:\
        static const int testIndex;

#define Q_TEST_RUNNER_REGISTER(className) \
    const int className::testIndex = QTestRunner::registerTest(new className)


#endif // QTESTRUNNER_HPP
