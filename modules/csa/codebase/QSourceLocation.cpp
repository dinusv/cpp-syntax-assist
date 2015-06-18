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

#include "QSourceLocation.hpp"
#include <QFileInfo>

namespace csa{

QSourceLocation::QSourceLocation(
        const QString& file,
        unsigned int line,
        unsigned int column,
        unsigned int offset,
        QObject* parent)
    : QObject(parent)
    , m_filePath(file)
    , m_line(line)
    , m_column(column)
    , m_offset(offset){
}

QSourceLocation::QSourceLocation(
        const char *file,
        unsigned int line,
        unsigned int column,
        unsigned int offset,
        QObject* parent)
    : QObject(parent)
    , m_filePath(file)
    , m_line(line)
    , m_column(column)
    , m_offset(offset){
}

QSourceLocation::QSourceLocation(
        const CXFile &file,
        unsigned int line,
        unsigned int column,
        unsigned int offset,
        QObject* parent)
    : QObject(parent)
    , m_line(line)
    , m_column(column)
    , m_offset(offset){

    CXString fileName = clang_getFileName(file);
    m_filePath        = clang_getCString(fileName);
    clang_disposeString(fileName);
}

QSourceLocation::QSourceLocation(const CXSourceLocation &location, QObject* parent)
    : QObject(parent){
    assign(location);
}

QSourceLocation::QSourceLocation(const QSourceLocation& other, QObject* parent)
    : QObject(parent){
    assign(other);
}

QSourceLocation::~QSourceLocation(){
}

void QSourceLocation::assign(const CXSourceLocation &location){
    CXFile file;
    clang_getSpellingLocation(location, &file, &m_line, &m_column, &m_offset);

    CXString fileName = clang_getFileName(file);
    if (fileName.data != 0)
        m_filePath = clang_getCString(fileName);
    clang_disposeString(fileName);
}

void QSourceLocation::assign(const QSourceLocation &other){
    m_filePath   = other.m_filePath;
    m_line       = other.m_line;
    m_column     = other.m_column;
    m_offset     = other.m_offset;
}

void QSourceLocation::assign(unsigned int offset, const CXTranslationUnit &translationUnit){
    CXFile file = clang_getFile(translationUnit, m_filePath.toUtf8());
    if ( file != 0 ){
        assign(clang_getLocationForOffset(translationUnit, file, offset));
    }
}

void QSourceLocation::assign(unsigned int line, unsigned int column, const CXTranslationUnit& translationUnit){
    CXFile file = clang_getFile(translationUnit, m_filePath.toUtf8());
    if ( file != 0 )
        assign(clang_getLocation(translationUnit, file, line, column));
}

QString QSourceLocation::fileName() const{
    return QFileInfo(m_filePath).fileName();
}

}// namespace
