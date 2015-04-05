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
