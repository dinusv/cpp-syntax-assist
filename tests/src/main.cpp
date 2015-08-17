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


#include <QCoreApplication>
#include <QTest>
#include "QTestRunner.hpp"
#include "QASTParsingTest.hpp"
#include "QASTInsertionTest.hpp"
#include "QASTSearchTest.hpp"

#include <qqml.h>
#include "QCodeBase.hpp"
#include "QASTFile.hpp"
#include "QASTNode.hpp"
#include "QSourceLocation.hpp"

int main(int argc, char *argv[])
{
    qmlRegisterUncreatableType<csa::QSourceLocation>(
        "CSA", 1, 0, "SourceLocation", "SourceLocation is available only as a property.");

    qmlRegisterUncreatableType<csa::QCodeBase>(
        "CSA", 1, 0, "CodeBase", "Codebase is available only as a property.");

    qmlRegisterUncreatableType<csa::ast::QASTFile>(
        "CSA", 1, 0, "ASTFile", "ASTFile is available only as a property.");

    qmlRegisterUncreatableType<csa::ast::QASTNode>(
        "CSA", 1, 0, "ASTNode", "ASTNode is available only as a property.");

    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    return QTestRunner::runTests(argc, argv);
}
