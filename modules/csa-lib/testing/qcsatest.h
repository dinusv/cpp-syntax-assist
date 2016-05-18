#ifndef QCSATEST_H
#define QCSATEST_H

#include "qcsaglobal.h"
#include <QObject>
#include <QJSValue>

namespace csa{

class QCSAEngine;
class QCSATestCase;
class Q_CSA_EXPORT QCSATest : public QObject{

    Q_OBJECT

public:
    typedef QList<QCSATestCase*>::iterator Iterator;
    typedef QList<QCSATestCase*>::const_iterator ConstIterator;

public:
    explicit QCSATest(QObject *parent = 0);
    ~QCSATest();

    Iterator begin();
    ConstIterator begin() const;
    Iterator end();
    ConstIterator end() const;

    int loadTestFile(const QString& path);
    int loadTests(const QString& path);

    static void registerTestingTypes();

public slots:
    void describe(const QString& str, QJSValue val);

private:
    QList<QCSATestCase*> m_testcases;
    QCSAEngine*    m_scriptEngine;

};

inline QCSATest::Iterator QCSATest::begin(){
    return m_testcases.begin();
}

inline QCSATest::ConstIterator QCSATest::begin() const{
    return m_testcases.begin();
}

inline QCSATest::Iterator QCSATest::end(){
    return m_testcases.end();
}

inline QCSATest::ConstIterator QCSATest::end() const{
    return m_testcases.end();
}

}// namespace

#endif // QCSATEST_H
