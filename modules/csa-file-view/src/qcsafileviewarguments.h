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


#ifndef QCSAFILEVIEWARGUMENTS_HPP
#define QCSAFILEVIEWARGUMENTS_HPP

#include <QStringList>

class QGuiApplication;
class QCommandLineParser;
class QCSAFileViewArguments{

public:
    QCSAFileViewArguments(const QGuiApplication& app, const QString& applicationDescription);
    ~QCSAFileViewArguments();

    const QStringList& files() const;
    const QStringList& fileErrors() const;
    bool  hasFileErrors() const;

    bool  isSelectedFunctionSet() const;
    const QString& selectedFunction() const;

    bool  isCursorOffsetSet() const;
    int   cursorOffset() const;
    bool  isCursorSet() const;

    const QString& cursorFile() const;
    int   cursorLineOrOffset() const;

    bool  isCursorLineColumnSet() const;
    int   cursorLine() const;
    int   cursorColumn() const;

    int   logLevel() const;

    const QString& projectDir() const;

private:
    QCSAFileViewArguments(const QCSAFileViewArguments& other);
    QCSAFileViewArguments& operator =(const QCSAFileViewArguments& other);

    void initialize(const QGuiApplication& app, const QString& applicationDescription);

private:
    QCommandLineParser* m_commandLineParser;

    QStringList m_files;
    QStringList m_fileErrors;

    QStringList m_headerSearchPatterns;
    QStringList m_sourceSearchPatterns;

    QString m_projectDir;

    bool    m_functionSet;
    QString m_function;

    int     m_cursorOffset;
    int     m_cursorLine;
    int     m_cursorColumn;
    QString m_cursorFile;

    int     m_logLevel;
};

inline const QStringList& QCSAFileViewArguments::files() const{
    return m_files;
}

inline const QStringList &QCSAFileViewArguments::fileErrors() const{
    return m_fileErrors;
}

inline bool QCSAFileViewArguments::hasFileErrors() const{
    return m_fileErrors.size() > 0;
}

inline bool QCSAFileViewArguments::isSelectedFunctionSet() const{
    return m_functionSet;
}

inline const QString& QCSAFileViewArguments::selectedFunction() const{
    return m_function;
}

inline bool QCSAFileViewArguments::isCursorOffsetSet() const{
    return m_cursorOffset != -1;
}

inline int QCSAFileViewArguments::cursorOffset() const{
    return m_cursorOffset;
}

inline bool QCSAFileViewArguments::isCursorSet() const{
    return isCursorLineColumnSet() || isCursorOffsetSet() || m_cursorFile != "";
}

inline const QString &QCSAFileViewArguments::cursorFile() const{
    return m_cursorFile;
}

inline int QCSAFileViewArguments::cursorLineOrOffset() const{
    return isCursorLineColumnSet() ? m_cursorLine : m_cursorOffset;
}

inline bool QCSAFileViewArguments::isCursorLineColumnSet() const{
    return (m_cursorLine != -1 && m_cursorColumn != -1);
}

inline int QCSAFileViewArguments::cursorLine() const{
    return m_cursorLine;
}

inline int QCSAFileViewArguments::cursorColumn() const{
    return m_cursorColumn;
}

inline const QString &QCSAFileViewArguments::projectDir() const{
    return m_projectDir;
}

inline int QCSAFileViewArguments::logLevel() const{
    return m_logLevel;
}

#endif // QCSAFILEVIEWARGUMENTS_HPP
