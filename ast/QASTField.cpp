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


#include "QASTField.hpp"
#include "QASTClass.hpp"
#include "QAnnotatedTokenSet.hpp"
#include "QSourceLocation.hpp"
#include "clang-c/Index.h"

#include <QDebug>

namespace csa{ namespace ast{

QASTField::QASTField(
        QAnnotatedTokenSet *tokenSet,
        QSourceLocation* cursorLocation,
        QSourceLocation* rangeStartLocation,
        QSourceLocation* rangeEndLocation,
        QASTNode* parent)

    : QASTNode("field", tokenSet, cursorLocation, rangeStartLocation, rangeEndLocation, parent){


    if ( tokenSet->size() == 0 ){
        CXType type          = clang_getCursorType(tokenSet->cursor());
        CXString typeStr     = clang_getTypeSpelling(type);
        const char* cTypeStr = clang_getCString(typeStr);
        setIdentifier(cTypeStr);
        clang_disposeString(typeStr);
    }

    QString fieldData;
    bool spaceFlag = false;
    for ( QAnnotatedTokenSet::Iterator it = tokenSet->begin(); it != tokenSet->end(); ++it ){
        CXToken t = *it;
        CXString tSpelling = clang_getTokenSpelling(tokenSet->translationUnit(), t);
        CXTokenKind tKind  = clang_getTokenKind(t);
        if ( tKind == CXToken_Punctuation ){
            fieldData += clang_getCString(tSpelling);
            spaceFlag    = false;
        } else {
            if ( spaceFlag ){
                fieldData += " ";
                spaceFlag = false;
            }
            fieldData += clang_getCString(tSpelling);
            spaceFlag     = true;
        }
        clang_disposeString(tSpelling);
    }

    int splitPos = fieldData.indexOf(' ');
    if ( splitPos == -1 )
        setIdentifier(fieldData);
    else {
        m_fieldType = fieldData.mid(0, splitPos);
        setIdentifier(fieldData.mid(splitPos + 1));
    }
}

QString QASTField::prop(const QString &key) const{
    if ( key == "type" )
        return m_fieldType;
    return QASTNode::prop(key);
}

}}// namespace
