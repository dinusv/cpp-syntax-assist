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


#ifndef QASTNAMESPACE_HPP
#define QASTNAMESPACE_HPP

#include "qcsaglobal.h"
#include "qastnode.h"

namespace csa{
class QAnnotatedTokenSet;

namespace ast{

class Q_CSA_EXPORT QASTNamespace : public QASTNode{

    Q_OBJECT

public:
    QASTNamespace(
            QAnnotatedTokenSet* tokenSet,
            QSourceLocation* cursorLocation,
            QSourceLocation* rangeStartLocation,
            QSourceLocation* rangeEndLocation,
            QASTNode* parent = 0);
    virtual ~QASTNamespace();

    virtual QASTNode* propagateUserCursor(const QSourceLocation &location);
    virtual QString description() const;

    virtual QSourceLocation* bodyStartLocation();
    virtual QSourceLocation* bodyEndLocation();

private:
    QSourceLocation* m_bodyStart;
    QSourceLocation* m_bodyEnd;

    bool m_bodyStartSet;
};

inline QString QASTNamespace::description() const{
    return typeName() + " " + identifier();
}

inline QSourceLocation* QASTNamespace::bodyStartLocation(){
    return m_bodyStart;
}

inline QSourceLocation* QASTNamespace::bodyEndLocation(){
    return m_bodyEnd;
}

}}// namespace

#endif // QASTNAMESPACE_HPP
