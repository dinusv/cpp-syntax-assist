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

#ifndef QASTCLASS_HPP
#define QASTCLASS_HPP

#include "QCSAGlobal.hpp"
#include "QASTNode.hpp"

namespace csa{ namespace ast{

class Q_CSA_EXPORT QASTClass : public QASTNode{

public:
    QASTClass(
            QAnnotatedTokenSet* tokenSet,
            QSourceLocation* cursorLocation,
            QSourceLocation* rangeStartLocation,
            QSourceLocation* rangeEndLocation,
            QASTNode* parent = 0);
    virtual ~QASTClass();

    QString description() const;

    virtual QASTNode* propagateUserCursor(const QSourceLocation &location);

    virtual QSourceLocation* bodyStartLocation();
    virtual QSourceLocation* bodyEndLocation();

private:
    QSourceLocation* m_cursor;
    QSourceLocation* m_bodyStart;
    QSourceLocation* m_bodyEnd;

    bool m_bodyStartSet;
};

inline QString QASTClass::description() const{
    return typeName() + " " + identifier();
}

inline QSourceLocation* QASTClass::bodyStartLocation(){
    return m_bodyStart;
}

inline QSourceLocation* QASTClass::bodyEndLocation(){
    return m_bodyEnd;
}

}}// namespace

#endif // QASTCLASS_HPP
