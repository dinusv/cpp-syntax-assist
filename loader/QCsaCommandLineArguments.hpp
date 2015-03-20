#ifndef QCSACOMMANDLINEARGUMENTS_HPP
#define QCSACOMMANDLINEARGUMENTS_HPP

#include <QString>

class QGuiApplication;
class QCommandLineParser;
class QCsaCommandLineArguments{

public:
    QCsaCommandLineArguments(const QGuiApplication& app, const QString& applicationDescription);
    ~QCsaCommandLineArguments();

    const QString file() const;

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

    QString m_file;

    bool    m_functionSet;
    QString m_function;

    int     m_cursorOffset;
    int     m_cursorLine;
    int     m_cursorColumn;

    bool    m_executeAndQuitFlag;
};

inline const QString QCsaCommandLineArguments::file() const{
    return m_file;
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
