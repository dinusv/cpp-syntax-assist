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


#ifndef QASTDESTRUCTOR_HPP
#define QASTDESTRUCTOR_HPP

#include "QCSAGlobal.hpp"
#include "QASTNode.hpp"

namespace csa{ namespace ast{

class Q_CSA_EXPORT QASTDestructor : public QASTNode{

public:
    QASTDestructor(
            QAnnotatedTokenSet* tokenSet,
            QSourceLocation* cursorLocation,
            QSourceLocation* rangeStartLocation,
            QSourceLocation* rangeEndLocation,
            QASTNode* parent = 0);

    QString content() const;
};

inline QString QASTDestructor::content() const{
    return identifier();
}

}}// namespace

#endif // QASTDESTRUCTOR_HPP
