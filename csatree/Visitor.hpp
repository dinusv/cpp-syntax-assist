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


#ifndef VISITOR_HPP
#define VISITOR_HPP

#include "clang-c/Index.h"

namespace csa{

class TokenClassifier;
namespace ast{
class CSANode;
}

class Visitor{

public:
    Visitor();

    static void createCSANodeTree(const CXCursor& rootCursor, ast::CSANode* root, TokenClassifier* classifier );
    static CXChildVisitResult callback(CXCursor cursor, CXCursor, CXClientData data);

};

}// namespace

#endif // VISITOR_HPP
