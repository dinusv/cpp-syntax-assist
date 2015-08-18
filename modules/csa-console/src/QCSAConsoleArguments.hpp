#ifndef QCSACONSOLEARGUMENTS_H
#define QCSACONSOLEARGUMENTS_H

#include <QStringList>

class QCommandLineParser;
class QCSAConsoleArguments{

public:
    explicit QCSAConsoleArguments();
    ~QCSAConsoleArguments();

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
    QCSAConsoleArguments(const QCSAConsoleArguments& other);
    QCSAConsoleArguments& operator =(const QCSAConsoleArguments& other);

//    void initialize(const QGuiApplication& app, const QString& applicationDescription);

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

inline const QStringList& QCSAConsoleArguments::files() const{
    return m_files;
}

inline const QStringList &QCSAConsoleArguments::fileErrors() const{
    return m_fileErrors;
}

inline bool QCSAConsoleArguments::hasFileErrors() const{
    return m_fileErrors.size() > 0;
}

inline bool QCSAConsoleArguments::isSelectedFunctionSet() const{
    return m_functionSet;
}

inline const QString& QCSAConsoleArguments::selectedFunction() const{
    return m_function;
}

inline bool QCSAConsoleArguments::isCursorOffsetSet() const{
    return m_cursorOffset != -1;
}

inline int QCSAConsoleArguments::cursorOffset() const{
    return m_cursorOffset;
}

inline bool QCSAConsoleArguments::isCursorLineColumnSet() const{
    return (m_cursorLine != -1 && m_cursorColumn != -1);
}

inline int QCSAConsoleArguments::cursorLine() const{
    return m_cursorLine;
}

inline int QCSAConsoleArguments::cursorColumn() const{
    return m_cursorColumn;
}

inline bool QCSAConsoleArguments::isExecuteAndQuitSet() const{
    return m_executeAndQuitFlag;
}

#endif // QCSACONSOLEARGUMENTS_H
