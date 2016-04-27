/****************************************************************************
**
** Copyright (C) 2014-2015 Dinu SV.
** (contact: mail@dinusv.com)
** This file is part of C++ Snippet Assist application.
**
** GNU General Public License Usage
** 
** This file may be used under the terms of the GNU General Public License 
** version 3.0 as published by the Free Software Foundation and appearing 
** in the file LICENSE.GPL included in the packaging of this file.  Please 
** review the following information to ensure the GNU General Public License 
** version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.
**
****************************************************************************/


#include "QTestRunner.hpp"
#include <QTest>

QList<QSharedPointer<QObject> >* QTestRunner::m_tests = 0;

int QTestRunner::registerTest(QObject* test){
    if ( !m_tests ){
        m_tests = new QList<QSharedPointer<QObject> >;
        atexit(&cleanupTests);
    }
    m_tests->append(QSharedPointer<QObject>(test));
    return 0;
}

int QTestRunner::runTests(int argc, char *argv[]){
    int code = 0;
    for ( QList<QSharedPointer<QObject> >::iterator it = m_tests->begin(); it != m_tests->end(); ++it ){
        code += QTest::qExec(it->data(), argc, argv);
    }
    return code;
}

int QTestRunner::runTest(int index, int argc, char* argv[]){
    if ( index > m_tests->size() )
        return -1;
    return QTest::qExec((*m_tests)[index].data(), argc, argv);
}

void QTestRunner::cleanupTests(){
    delete m_tests;
}
