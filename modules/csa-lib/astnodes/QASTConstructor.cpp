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


#include "QASTConstructor.hpp"
#include "QSourceLocation.hpp"
#include "QSourceLocation_p.hpp"
#include "QASTMethodArgument.hpp"
#include "QAnnotatedToken_p.hpp"
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

    // Get Identifier
    // --------------

    CXString id = clang_getCursorSpelling(tokenSet->cursor());
    setIdentifier(clang_getCString(id));

    // Get Arguments
    // -------------

    QAnnotatedTokenSet::Iterator it = tokenSet->begin();

    int numArguments = clang_Cursor_getNumArguments(tokenSet->cursor());
    QList< QList<QAnnotatedToken*> > argTokenSetList;

    for( int i = 0; i < numArguments; ++i ){
        CXCursor argCursor = clang_Cursor_getArgument(tokenSet->cursor(), i);

        QAnnotatedTokenSet* argTokenSet = classifier->findTokenSet(argCursor);
        if ( argTokenSet == 0 ){

            // See if argTokenSet has been attributed to this method
            // -----------------------------------------------------

            if ( argTokenSetList.empty() && it != tokenSet->end() ){
                bool locationInArg = false;

                QList<QAnnotatedToken*> currentTokenSetList;

                while ( it != tokenSet->end() ){
                    CXToken t              = (*it)->token().token;
                    CXString tSpelling     = clang_getTokenSpelling(tokenSet->translationUnit(), t);
                    const char* tCSpelling = clang_getCString(tSpelling);
                    CXTokenKind tKind      = clang_getTokenKind(t);

                    if ( tKind == CXToken_Punctuation && std::string("(") == tCSpelling ){
                        locationInArg = true;
                        ++it;
                    } else if ( locationInArg ){

                        if ( tKind == CXToken_Punctuation && std::string(",") == tCSpelling ){
                            argTokenSetList.append(currentTokenSetList);
                            currentTokenSetList = QList<QAnnotatedToken*>();
                        } else if ( tKind == CXToken_Punctuation && std::string(")") == tCSpelling ){
                            argTokenSetList.append(currentTokenSetList);
                            locationInArg = false;
                        } else {
                            currentTokenSetList.append(*it);
                        }

                        tokenSet->erase(it);
                    } else {
                        ++it;
                    }

                    clang_disposeString(tSpelling);
                }
            }

            // Add parsed tokens from this method to the args
            // ----------------------------------------------

            argTokenSet = new QAnnotatedTokenSet(argCursor, classifier->translationUnit());
            if ( i < argTokenSetList.size() ){
                for ( int j = 0; j < argTokenSetList[i].size(); ++j ){
                    argTokenSet->append(argTokenSetList[i][j]->token().token);
                }
            }

            classifier->appendTokenSet(argTokenSet);
        }

        CXSourceLocation loc = clang_getCursorLocation(argCursor);
        CXSourceRange range  = clang_getCursorExtent(argCursor);

        CXSourceLocation locStart = clang_getRangeStart(range);
        CXSourceLocation locEnd   = clang_getRangeEnd(range);

        QASTMethodArgument* argument = new QASTMethodArgument(
            argTokenSet,
            new QSourceLocation(createSourceLocation(loc)),
            new QSourceLocation(createSourceLocation(locStart)),
            new QSourceLocation(createSourceLocation(locEnd)),
            0
        );
        argument->setParent(this);

        m_arguments.append(argument);
    }

    clang_disposeString(id);
}

QString QASTConstructor::description() const{
    QString base = "";
    base += identifier();
    base += "(";

    bool firstArg = true;
    for ( QList<csa::ast::QASTNode*>::const_iterator it = m_arguments.begin(); it != m_arguments.end(); ++it ){
        if ( firstArg ){
            base    += (*it)->description();
            firstArg = false;
        } else {
            base    += ", " + (*it)->description();
        }
    }
    base += ")";

    return base;
}

}}// namespace
