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


#ifndef CSANAMESPACE_HPP
#define CSANAMESPACE_HPP

#include "QASTNode.hpp"

namespace csa{
class AnnotatedTokenSet;

namespace ast{

class QASTNamespace : public QASTNode{

public:
    enum OffsetId{
        BEGIN = 0,
        END,
        CURSOR
    };

public:
    QASTNamespace(
            AnnotatedTokenSet* tokenSet,
            SourceLocation* cursorLocation,
            SourceLocation* rangeStartLocation,
            SourceLocation* rangeEndLocation,
            QASTNode* parent = 0);
    virtual ~QASTNamespace();

    virtual std::string content() const;
    virtual std::string identifier() const;

    virtual const SourceLocation* location(const std::string& id) const;
    virtual const SourceLocation* location(int id) const;

    virtual QASTNode* propagateUserCursor(const SourceLocation &location);

private:
    std::string        m_identifier;
    QASTNode::OffsetMap m_offsets;

    static const QASTNode::OffsetKeyMap OFFSET_KEYS;
    static const QASTNode::OffsetKeyMap createOffsetKeys();
};

inline std::string QASTNamespace::content() const{
    return typeString() + identifier();
}

inline std::string QASTNamespace::identifier() const{
    return m_identifier;
}


}}// namespace

#endif // CSANAMESPACE_HPP
