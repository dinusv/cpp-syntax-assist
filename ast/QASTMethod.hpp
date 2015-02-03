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


#ifndef QASTMETHOD_HPP
#define QASTMETHOD_HPP

#include "QASTNode.hpp"

namespace csa{

class QTokenClassifier;

namespace ast{

class QASTMethod : public QASTNode{

public:
    QASTMethod(
            QAnnotatedTokenSet* tokenSet,
            QTokenClassifier* classifier,
            QSourceLocation* cursorLocation,
            QSourceLocation* rangeStartLocation,
            QSourceLocation* rangeEndLocation,
            QASTNode* parent = 0);

    virtual QString content() const;
};

inline QString QASTMethod::content() const{
    return identifier();
}

}}// namespace

#endif // QASTMETHOD_HPP
