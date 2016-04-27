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


#ifndef QASTMETHODARGUMENT_HPP
#define QASTMETHODARGUMENT_HPP

#include "qcsaglobal.h"
#include "qcsaglobal.h"
#include "qastnode.h"

namespace csa{ namespace ast{

class Q_CSA_EXPORT QASTMethodArgument : public QASTNode{

    Q_OBJECT

public:
    QASTMethodArgument(
            QAnnotatedTokenSet* tokenSet,
            QSourceLocation* cursorLocation,
            QSourceLocation* rangeStartLocation,
            QSourceLocation* rangeEndLocation,
            QASTNode* parent = 0);
    virtual ~QASTMethodArgument();

    virtual QString description() const;
    virtual QString prop(const QString &key) const;

signals:

public slots:

private:
    QString m_type;

};

}} // namespace ast, csa

#endif // QASTMETHODARGUMENT_HPP
