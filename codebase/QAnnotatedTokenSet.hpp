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


#ifndef QANNOTATEDTOKENSET_HPP
#define QANNOTATEDTOKENSET_HPP

#include "clang-c/Index.h"
#include "QAnnotatedToken.hpp"
#include <QList>
#include <string>

namespace csa{

namespace ast{
class QASTNode;
}

class QAnnotatedTokenSet : public QObject{

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

    void dump(std::string &str);

    ast::QASTNode* associatedNode();

    QList<QAnnotatedToken*> tokenList();

private:
    CXCursor             m_cursor;
    CXTranslationUnit    m_translationUnit;
    TokenList            m_tokens;

};

inline const CXCursor &QAnnotatedTokenSet::cursor() const{
    return m_cursor;
}

inline unsigned int QAnnotatedTokenSet::size() const{
    return (unsigned int)m_tokens.size();
}

inline QAnnotatedTokenSet::Iterator QAnnotatedTokenSet::begin(){
    return m_tokens.begin();
}

inline QAnnotatedTokenSet::Iterator QAnnotatedTokenSet::end(){
    return m_tokens.end();
}

inline QAnnotatedTokenSet::ConstIterator QAnnotatedTokenSet::begin() const{
    return m_tokens.begin();
}

inline QAnnotatedTokenSet::ConstIterator QAnnotatedTokenSet::end() const{
    return m_tokens.end();
}

}// namespace

#endif // QANNOTATEDTOKENSET_HPP
