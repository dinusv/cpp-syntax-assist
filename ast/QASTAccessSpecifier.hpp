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


#ifndef CSAACCESSSPECIFIER_HPP
#define CSAACCESSSPECIFIER_HPP

#include "QASTNode.hpp"

namespace csa{ namespace ast{

class QASTAccessSpecifier : public QASTNode{

public:
    QASTAccessSpecifier(
            AnnotatedTokenSet* tokenSet,
            SourceLocation* cursorLocation,
            SourceLocation* rangeStartLocation,
            SourceLocation* rangeEndLocation,
            QASTNode* parent = 0);

    virtual std::string content() const;
    virtual std::string identifier() const;

private:
    std::string m_typeValueName;
};

inline std::string QASTAccessSpecifier::content() const{
    return m_typeValueName + " :";
}

inline std::string QASTAccessSpecifier::identifier() const{
    return m_typeValueName;
}

}}// namespace

#endif // CSAACCESSSPECIFIER_HPP
