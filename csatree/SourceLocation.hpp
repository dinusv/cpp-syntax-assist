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


#ifndef SOURCELOCATION_HPP
#define SOURCELOCATION_HPP

#include "clang-c/Index.h"
#include <string>

namespace csa{

class SourceLocation{

public:
    SourceLocation(const std::string& file, unsigned int line, unsigned int column, unsigned int offset );
    SourceLocation(const char* file, unsigned int line, unsigned int column, unsigned int offset );
    SourceLocation(const CXFile& file, unsigned int line, unsigned int column, unsigned int offset );
    SourceLocation(const CXSourceLocation& location);
    ~SourceLocation();

    void assign(const CXSourceLocation& location);
    void assign(const SourceLocation& other);
    void assign(unsigned int offset, const CXTranslationUnit& translationUnit);

    unsigned int line() const;
    unsigned int column() const;
    unsigned int offset() const;
    const std::string &filePath() const;

private:
    std::string  m_file;
    unsigned int m_line;
    unsigned int m_column;
    unsigned int m_offset;

};

inline unsigned int SourceLocation::line() const{
    return m_line;
}

inline unsigned int SourceLocation::column() const{
    return m_column;
}

inline unsigned int SourceLocation::offset() const{
    return m_offset;
}


}// namespace

#endif // SOURCELOCATION_HPP
