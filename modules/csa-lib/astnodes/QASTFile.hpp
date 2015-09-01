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


#ifndef QASTFILEROOT_HPP
#define QASTFILEROOT_HPP

#include "QCSAGlobal.hpp"
#include "QASTNode.hpp"

namespace csa{ namespace ast{

class QModifierElementPrivate;

class Q_CSA_EXPORT QASTFile : public QASTNode{

    Q_OBJECT

public:
    QASTFile(QAnnotatedTokenSet* tokenSet, const QString& file, QSourceLocation* endOfFile);

    bool hasModifiers();
    void save();
    QString description() const;

public slots:
    bool insert(const QString& value, csa::QSourceLocation* location);
    bool erase(csa::QSourceLocation* from, csa::QSourceLocation* to);
    QString readAll();
    QString read(csa::QSourceLocation* start, csa::QSourceLocation* end) const;
    unsigned int size();

    csa::QSourceLocation* createLocation(unsigned int lineOrOffset, unsigned int column = 0);

    QString fileName();
    QString fileNameWithouExtension();
    QString extension();

protected:
    virtual QString text(QSourceLocation *from, QSourceLocation *to);

private:
    QList<QModifierElementPrivate*> m_modifiers;

};

inline QString QASTFile::description() const{
    return QString("file \"") + identifier() + "\"";
}

}}//namespace csa, ast

Q_DECLARE_METATYPE(csa::ast::QASTFile*)

#endif // QASTFILEROOT_HPP
