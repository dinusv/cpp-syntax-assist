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


#ifndef QASTNAMESPACE_HPP
#define QASTNAMESPACE_HPP

#include "QASTNode.hpp"

namespace csa{
class QAnnotatedTokenSet;

namespace ast{

class QASTNamespace : public QASTNode{

    Q_OBJECT

public:
    enum OffsetId{
        BEGIN = 0,
        END,
        CURSOR
    };

public:
    QASTNamespace(
            QAnnotatedTokenSet* tokenSet,
            QSourceLocation* cursorLocation,
            QSourceLocation* rangeStartLocation,
            QSourceLocation* rangeEndLocation,
            QASTNode* parent = 0);
    virtual ~QASTNamespace();

    virtual QASTNode* propagateUserCursor(const QSourceLocation &location);
    virtual QString content() const;

private:
    QSourceLocation* m_bodyStart;
    QSourceLocation* m_bodyEnd;

    bool m_bodyStartSet;
};

inline QString QASTNamespace::content() const{
    return typeName() + identifier();
}

}}// namespace

#endif // QASTNAMESPACE_HPP
