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


#ifndef QANNOTATEDTOKENSET_HPP
#define QANNOTATEDTOKENSET_HPP

#include <QList>
#include <string>
#include "clang-c/Index.h"
#include "QAnnotatedToken.hpp"
#include "QCSAGlobal.hpp"

namespace csa{

namespace ast{
class QASTNode;
}

class QAnnotatedTokenSetPrivate;
class Q_CSA_EXPORT QAnnotatedTokenSet : public QObject{

    Q_OBJECT

public:
    typedef QList<QAnnotatedToken*>   TokenList;
    typedef TokenList::iterator       Iterator;
    typedef TokenList::const_iterator ConstIterator;

public:
    QAnnotatedTokenSet(const CXCursor& cursor, const CXTranslationUnit& tu, ast::QASTNode* parent = 0);
    ~QAnnotatedTokenSet();

    void append(const CXToken& token);

    bool operator ==(const QAnnotatedTokenSet& other);
    bool operator !=(const QAnnotatedTokenSet& other);

    const CXTranslationUnit& translationUnit();
    const CXCursor& cursor() const;
    unsigned int  size() const;
    Iterator      begin();
    Iterator      end();
    ConstIterator begin() const;
    ConstIterator end() const;

    void erase(Iterator it);

    void dump(std::string &str);

    ast::QASTNode* associatedNode();

    const QList<QAnnotatedToken*>& tokenList() const;

private:
    QAnnotatedTokenSet(const QAnnotatedTokenSet& other);
    QAnnotatedTokenSet& operator=(const QAnnotatedTokenSet& other);

    QAnnotatedTokenSetPrivate* const d_ptr;
    Q_DECLARE_PRIVATE(QAnnotatedTokenSet)
};

}// namespace

#endif // QANNOTATEDTOKENSET_HPP
