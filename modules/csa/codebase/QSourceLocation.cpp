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

#include "clang-c/Index.h"
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
        const QSourceLocation& other,
        QObject* parent)
    : QObject(parent)
    , m_filePath(other.m_filePath)
    , m_line(other.m_line)
    , m_column(other.m_column)
    , m_offset(other.m_offset){
}

QSourceLocation::~QSourceLocation(){
}

void QSourceLocation::assign(const QSourceLocation &other){
    m_filePath   = other.m_filePath;
    m_line       = other.m_line;
    m_column     = other.m_column;
    m_offset     = other.m_offset;
}

QSourceLocation&QSourceLocation::operator =(const QSourceLocation& other){
    assign(other);
    return *this;
}

QString QSourceLocation::fileName() const{
    return QFileInfo(m_filePath).fileName();
}

QString QSourceLocation::toString() const{
    return QString("SourceLocation(offset:") + QString::number(m_offset) + ",line:" + QString::number(m_line) +
            ",column:" + QString::number(m_column) + ",file: " + fileName() + ")";
}

}// namespace
