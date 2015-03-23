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


#include "QASTConstructor.hpp"
#include "QASTClass.hpp"
#include "QAnnotatedTokenSet.hpp"
#include "QTokenClassifier.hpp"
#include "clang-c/Index.h"

namespace csa{ namespace ast{

QASTConstructor::QASTConstructor(
        QAnnotatedTokenSet *tokenSet,
        QTokenClassifier* classifier,
        QSourceLocation* cursorLocation,
        QSourceLocation* rangeStartLocation,
        QSourceLocation* rangeEndLocation,
        QASTNode* parent)

    : QASTNode("constructor", tokenSet, cursorLocation, rangeStartLocation, rangeEndLocation, parent){

    if ( tokenSet->size() == 0 ){
        CXType type          = clang_getCursorType(tokenSet->cursor());
        CXString typeStr     = clang_getTypeSpelling(type);
        const char* cTypeStr = clang_getCString(typeStr);
        setIdentifier(cTypeStr);
        clang_disposeString(typeStr);
    }




    CXString displayName = clang_getCursorDisplayName(tokenSet->cursor());
    setIdentifier(clang_getCString(displayName));
    clang_disposeString(displayName);

    /*for ( QAnnotatedTokenSet::Iterator it = tokenSet->begin(); it != tokenSet->end(); ++it ){
        CXToken t = *it;
        CXString tSpelling = clang_getTokenSpelling(tokenSet->translationUnit(), t);
        CXTokenKind tKind  = clang_getTokenKind(t);
        if ( tKind == CXToken_Punctuation && std::string(clang_getCString(tSpelling)) == "("){

            setIdentifier(identifier() + clang_getCString(tSpelling));
            int numargs = clang_Cursor_getNumArguments(tokenSet->cursor());
            for ( int i = 0; i < numargs; ++i ){

                CXCursor crs                      = clang_Cursor_getArgument(tokenSet->cursor(), i);
                QAnnotatedTokenSet* cursorTokenSet = classifier->findTokenSet(crs);

                if ( cursorTokenSet ){
                    bool spaceFlag = false;
                    for ( QAnnotatedTokenSet::Iterator it = cursorTokenSet->begin(); it != cursorTokenSet->end(); ++it ){
                        CXToken t = *it;
                        CXString tSpelling = clang_getTokenSpelling(tokenSet->translationUnit(), t);
                        CXTokenKind tKind  = clang_getTokenKind(t);
                        if ( tKind == CXToken_Punctuation ){
                            setIdentifier(identifier() + clang_getCString(tSpelling));
                            spaceFlag    = false;
                        } else {
                            if ( spaceFlag ){
                                setIdentifier(identifier() + " ");
                                spaceFlag = false;
                            }
                            setIdentifier(identifier() + clang_getCString(tSpelling));
                            spaceFlag     = true;
                        }
                        clang_disposeString(tSpelling);
                    }
                    if ( i != numargs - 1 )
                        setIdentifier(identifier() + ", ");
                }
            }

        } else if ( tKind != CXToken_Punctuation || std::string(clang_getCString(tSpelling)) != "," ){
            setIdentifier(identifier() + clang_getCString(tSpelling));
        }
        clang_disposeString(tSpelling);
    }*/

}

}}// namespace
