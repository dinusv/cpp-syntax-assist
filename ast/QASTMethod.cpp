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


#include "QASTMethod.hpp"
#include "QASTClass.hpp"
#include "QAnnotatedTokenSet.hpp"
#include "QTokenClassifier.hpp"
#include "QSourceLocation.hpp"
#include "clang-c/Index.h"

#include <QDebug>

namespace csa{ namespace ast{

QASTMethod::QASTMethod(
        QAnnotatedTokenSet *tokenSet,
        QTokenClassifier* classifier,
        QSourceLocation* cursorLocation,
        QSourceLocation* rangeStartLocation,
        QSourceLocation* rangeEndLocation,
        QASTNode* parent)

    : QASTNode("method", tokenSet, cursorLocation, rangeStartLocation, rangeEndLocation, parent){

    if ( tokenSet->size() == 0 ){
        CXType type          = clang_getCursorType(tokenSet->cursor());
        CXString typeStr     = clang_getTypeSpelling(type);
        const char* cTypeStr = clang_getCString(typeStr);
        setIdentifier(cTypeStr);
        clang_disposeString(typeStr);
    }

    bool spaceFlag = false;

    for ( QAnnotatedTokenSet::Iterator it = tokenSet->begin(); it != tokenSet->end(); ++it ){
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
                    bool argSpaceFlag = false;
                    for ( QAnnotatedTokenSet::Iterator it = cursorTokenSet->begin(); it != cursorTokenSet->end(); ++it ){
                        CXToken t = *it;
                        CXString tArgSpelling = clang_getTokenSpelling(tokenSet->translationUnit(), t);
                        CXTokenKind tArgKind  = clang_getTokenKind(t);
                        if ( tArgKind == CXToken_Punctuation ){

                            setIdentifier(identifier() + clang_getCString(tArgSpelling));

                            if ( std::string(clang_getCString(tArgSpelling)) == "&" ||
                                 std::string(clang_getCString(tArgSpelling)) == "*" )
                                setIdentifier(identifier() + " ");
                            argSpaceFlag    = false;
                        } else {
                            if ( argSpaceFlag ){
                                setIdentifier(identifier() + " ");
                                argSpaceFlag = false;
                            }
                            setIdentifier(identifier() + clang_getCString(tArgSpelling));
                            argSpaceFlag     = true;
                        }
                        clang_disposeString(tArgSpelling);
                    }
                    if ( i != numargs - 1 )
                        setIdentifier(identifier() + ", ");
                }
            }

        } else if ( tKind != CXToken_Punctuation || std::string(clang_getCString(tSpelling)) != "," ){
            if ( tKind == CXToken_Punctuation ){

                setIdentifier(identifier() + clang_getCString(tSpelling));

                if ( std::string(clang_getCString(tSpelling)) == "&" ||
                     std::string(clang_getCString(tSpelling)) == "*" )
                    setIdentifier(identifier() + " ");
                spaceFlag    = false;
            } else {
                if ( spaceFlag ){
                    setIdentifier(identifier() + " ");
                    spaceFlag = false;
                }
                setIdentifier(identifier() + clang_getCString(tSpelling));
                spaceFlag     = true;
            }
        }
        clang_disposeString(tSpelling);
    }

}

}}// namespace
