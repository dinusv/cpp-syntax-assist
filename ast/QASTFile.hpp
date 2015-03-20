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


#ifndef QASTFILEROOT_HPP
#define QASTFILEROOT_HPP

#include "QASTNode.hpp"
#include <QDebug>

namespace csa{ namespace ast{

class QInsertionElementPrivate;

class QASTFile : public QASTNode{

    Q_OBJECT

public:
    QASTFile(QAnnotatedTokenSet* tokenSet, const QString& file, QSourceLocation* endOfFile);

    bool hasInsertions();
    void saveInsertions();
    QString content() const;

    void insert(const QString& value, const QSourceLocation& location);

private:
    QList<QInsertionElementPrivate*> m_insertions;
};

inline QString QASTFile::content() const{
    return QString("file \"") + identifier() + "\"";
}

}}//namespace

#endif // QASTFILEROOT_HPP
