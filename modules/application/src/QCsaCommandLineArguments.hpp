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


#ifndef QCSACOMMANDLINEARGUMENTS_HPP
#define QCSACOMMANDLINEARGUMENTS_HPP

#include <QStringList>

class QGuiApplication;
class QCommandLineParser;
class QCsaCommandLineArguments{

public:
    QCsaCommandLineArguments(const QGuiApplication& app, const QString& applicationDescription);
    ~QCsaCommandLineArguments();

    const QStringList& files() const;
    const QStringList& fileErrors() const;
    bool  hasFileErrors() const;

    bool  isSelectedFunctionSet() const;
    const QString& selectedFunction() const;

    bool  isCursorOffsetSet() const;
    int   cursorOffset() const;

    bool  isCursorLineColumnSet() const;
    int   cursorLine() const;
    int   cursorColumn() const;

    bool  isExecuteAndQuitSet() const;

private:
    void initialize(const QGuiApplication& app, const QString& applicationDescription);

private:
    QCommandLineParser* m_commandLineParser;

    QStringList m_files;
    QStringList m_fileErrors;

    QStringList m_headerSearchPatterns;
    QStringList m_sourceSearchPatterns;

    QString m_searchDir;

    bool    m_functionSet;
    QString m_function;

    int     m_cursorOffset;
    int     m_cursorLine;
    int     m_cursorColumn;

    bool    m_executeAndQuitFlag;
};

inline const QStringList& QCsaCommandLineArguments::files() const{
    return m_files;
}

inline const QStringList &QCsaCommandLineArguments::fileErrors() const{
    return m_fileErrors;
}

inline bool QCsaCommandLineArguments::hasFileErrors() const{
    return m_fileErrors.size() > 0;
}

inline bool QCsaCommandLineArguments::isSelectedFunctionSet() const{
    return m_functionSet;
}

inline const QString& QCsaCommandLineArguments::selectedFunction() const{
    return m_function;
}

inline bool QCsaCommandLineArguments::isCursorOffsetSet() const{
    return m_cursorOffset != -1;
}

inline int QCsaCommandLineArguments::cursorOffset() const{
    return m_cursorOffset;
}

inline bool QCsaCommandLineArguments::isCursorLineColumnSet() const{
    return (m_cursorLine != -1 && m_cursorColumn != -1);
}

inline int QCsaCommandLineArguments::cursorLine() const{
    return m_cursorLine;
}

inline int QCsaCommandLineArguments::cursorColumn() const{
    return m_cursorColumn;
}

inline bool QCsaCommandLineArguments::isExecuteAndQuitSet() const{
    return m_executeAndQuitFlag;
}

#endif // QCSACOMMANDLINEARGUMENTS_HPP
