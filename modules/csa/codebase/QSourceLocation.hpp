/****************************************************************************
**
** Copyright (C) 2014 Dinu SV.
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


#ifndef QSOURCELOCATION_HPP
#define QSOURCELOCATION_HPP

#include "clang-c/Index.h"
#include "QCSAGlobal.hpp"
#include <QString>
#include <QObject>

namespace csa{

class QSourceLocation : public QObject{

    Q_OBJECT

public:
    QSourceLocation(
            const QString& file,
            unsigned int line,
            unsigned int column,
            unsigned int offset,
            QObject* parent = 0 );
    QSourceLocation(
            const char* file,
            unsigned int line,
            unsigned int column,
            unsigned int offset,
            QObject* parent = 0);
    QSourceLocation(
            const CXFile& file,
            unsigned int line,
            unsigned int column,
            unsigned int offset,
            QObject* parent = 0);
    QSourceLocation(const CXSourceLocation& location, QObject* parent = 0);
    QSourceLocation(const QSourceLocation& other, QObject* parent = 0);
    ~QSourceLocation();

    void assign(const CXSourceLocation& location);
    void assign(const QSourceLocation& other);
    void assign(unsigned int offset, const CXTranslationUnit& translationUnit);
    void assign(unsigned int line, unsigned int column, const CXTranslationUnit& translationUnit);

public slots:
    unsigned int line() const;
    unsigned int column() const;
    unsigned int offset() const;
    QString filePath() const;

private:
    QString      m_filePath;
    unsigned int m_line;
    unsigned int m_column;
    unsigned int m_offset;

};

inline unsigned int QSourceLocation::line() const{
    return m_line;
}

inline unsigned int QSourceLocation::column() const{
    return m_column;
}

inline unsigned int QSourceLocation::offset() const{
    return m_offset;
}

inline QString QSourceLocation::filePath() const{
    return m_filePath;
}

}// namespace

#endif // QSOURCELOCATION_HPP
