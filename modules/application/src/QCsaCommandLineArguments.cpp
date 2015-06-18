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


#include "QCsaCommandLineArguments.hpp"
#include <QGuiApplication>
#include <QCommandLineParser>

QCsaCommandLineArguments::QCsaCommandLineArguments(
        const QGuiApplication& app,
        const QString& applicationDescription)
    : m_commandLineParser(new QCommandLineParser)
    , m_cursorOffset(-1)
    , m_cursorLine(-1)
    , m_cursorColumn(-1)
    , m_executeAndQuitFlag(false)
{
    initialize(app, applicationDescription);
}

QCsaCommandLineArguments::~QCsaCommandLineArguments(){
    delete m_commandLineParser;
}

void QCsaCommandLineArguments::initialize(const QGuiApplication& app, const QString& applicationDescription){

    // Setup parsing
    // -------------

    m_commandLineParser->setApplicationDescription(applicationDescription);
    m_commandLineParser->addHelpOption();
    m_commandLineParser->addVersionOption();
    m_commandLineParser->addPositionalArgument("file", QGuiApplication::translate("main", "File to parse."));

    QCommandLineOption cursorOffset("c",
        QCoreApplication::translate("main", "User cursor offset within the file."),
        QCoreApplication::translate("main", "offset")
    );
    m_commandLineParser->addOption(cursorOffset);

    QCommandLineOption cursorLineColumnOffset("lc",
        QCoreApplication::translate("main", "Line:Column of the user cursor within the file."),
        QCoreApplication::translate("main", "line:column")
    );
    m_commandLineParser->addOption(cursorLineColumnOffset);

    QCommandLineOption selectedFunction(QStringList() << "e" << "function",
        QCoreApplication::translate("main", "Selected javascript function from the loaded plugins."),
        QCoreApplication::translate("main", "function")
    );
    m_commandLineParser->addOption(selectedFunction);

    QCommandLineOption executeAndQuit("f",
        QCoreApplication::translate("main", "Execute function and quit.")
    );
    m_commandLineParser->addOption(executeAndQuit);

    // Process arguments
    // -----------------

    m_commandLineParser->process(app);

    // Get Values
    // ----------

    QStringList positionalArgs = m_commandLineParser->positionalArguments();
    if ( positionalArgs.size() == 0 )
        m_commandLineParser->showHelp(1);

    m_file = positionalArgs.at(0);

    m_functionSet = m_commandLineParser->isSet(selectedFunction);
    if ( m_functionSet )
        m_function = m_commandLineParser->value(selectedFunction);

    m_cursorOffset = m_commandLineParser->isSet(cursorOffset) ? m_commandLineParser->value(cursorOffset).toInt() : -1;
    if ( m_cursorOffset == -1 ){
        bool isLineColumnSet = m_commandLineParser->isSet(cursorLineColumnOffset);
        if ( isLineColumnSet ){
            QStringList lineColSplit = m_commandLineParser->value(cursorLineColumnOffset).split(":");
            if ( lineColSplit.size() != 2 )
                m_commandLineParser->showHelp(2);

            bool lineConvertOk, colConvertOk;
            m_cursorLine   = lineColSplit[0].toInt(&lineConvertOk);
            m_cursorColumn = lineColSplit[1].toInt(&colConvertOk);
            if ( lineConvertOk || colConvertOk )
                m_commandLineParser->showHelp(3);

        } else {
            m_cursorColumn = -1;
            m_cursorLine   = -1;
        }

    }

    m_executeAndQuitFlag = m_commandLineParser->isSet(executeAndQuit);
}

