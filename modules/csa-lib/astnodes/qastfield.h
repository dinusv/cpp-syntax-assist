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


#ifndef QASTFIELD_HPP
#define QASTFIELD_HPP

#include "qcsaglobal.h"
#include "qastnode.h"

namespace csa{ namespace ast{

class Q_CSA_EXPORT QASTField : public QASTNode{

public:
    QASTField(
            QAnnotatedTokenSet* tokenSet,
            QSourceLocation* cursorLocation,
            QSourceLocation* rangeStartLocation,
            QSourceLocation* rangeEndLocation,
            QASTNode* parent = 0);

    virtual QString declaration() const;
    virtual QVariant prop(const QString &key) const;

private:
    QString m_fieldType;
};

inline QString QASTField::declaration() const{
    return m_fieldType + " " + identifier();
}

}}// namespace

#endif // QASTFIELD_HPP
