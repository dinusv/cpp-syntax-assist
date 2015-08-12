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

#include "QASTMethod.hpp"
#include "QASTMethodArgument.hpp"
#include "QAnnotatedTokenSet.hpp"
#include "QTokenClassifier.hpp"
#include "QSourceLocation.hpp"
#include "QSourceLocation_p.hpp"
#include "clang-c/Index.h"

namespace csa{ namespace ast{

QASTMethod::QASTMethod(
        QAnnotatedTokenSet *tokenSet,
        QTokenClassifier* classifier,
        QSourceLocation* cursorLocation,
        QSourceLocation* rangeStartLocation,
        QSourceLocation* rangeEndLocation,
        QASTNode* parent)

    : QASTNode("method", tokenSet, cursorLocation, rangeStartLocation, rangeEndLocation, parent)
    , m_isStatic(false)
    , m_isVirtual(false)
    , m_isInline(false)
{

    // Get Identifier
    // --------------

    CXString id = clang_getCursorSpelling(tokenSet->cursor());
    setIdentifier(clang_getCString(id));

    // Get Return Type
    // ---------------

    CXType type           = clang_getCursorResultType(tokenSet->cursor());
    CXString typeSpelling = clang_getTypeSpelling(type);
    m_returnType          = clang_getCString(typeSpelling);
    clang_disposeString(typeSpelling);

    // If unknown, determine type from tokens
    // --------------------------------------

    QAnnotatedTokenSet::Iterator it = tokenSet->begin();
    if ( type.kind == CXType_Unexposed || type.kind == CXType_Invalid || m_returnType.contains("int") ){
        m_returnType = "";

        bool doubleColonFlag = false;

        while ( it != tokenSet->end() ){
            CXToken t              = (*it)->token();
            CXString tSpelling     = clang_getTokenSpelling(tokenSet->translationUnit(), t);
            const char* tCSpelling = clang_getCString(tSpelling);
            CXTokenKind tKind      = clang_getTokenKind(t);

            if ( tKind == CXToken_Identifier && std::string(clang_getCString(id)) == tCSpelling ){
                clang_disposeString(tSpelling);
                break;
            } else if ( tKind == CXToken_Keyword && std::string("inline") == tCSpelling ){
                m_isInline = true;
            } else if ( tKind == CXToken_Keyword && std::string("static") == tCSpelling ){
                m_isStatic = true;
            } else if ( tKind == CXToken_Keyword && std::string("virtual") == tCSpelling ){
                m_isVirtual = true;
            } else if ( tKind == CXToken_Keyword && std::string("operator") == tCSpelling ){
                clang_disposeString(tSpelling);
                break;
            } else if ( tKind == CXToken_Punctuation && std::string("::") == tCSpelling ){
                doubleColonFlag = true;
                m_returnType   += tCSpelling;
            } else if (( tKind == CXToken_Identifier || tKind == CXToken_Keyword ) &&
                       !m_returnType.isEmpty() && !doubleColonFlag ){
                m_returnType   += QString(" ") + tCSpelling;
            } else {
                m_returnType   += tCSpelling;
                doubleColonFlag = false;
            }

            ++it;
            clang_disposeString(tSpelling);
        }
    }

    // Check Static
    // ------------

    m_isStatic = clang_CXXMethod_isStatic(tokenSet->cursor());

    // Get Arguments
    // -------------

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
                    CXToken t              = (*it)->token();
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
                    argTokenSet->append(argTokenSetList[i][j]->token());
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

    return;
}

QList<QObject*> QASTMethod::arguments() const{
    return QASTNode::castNodeListToObjectList(m_arguments);
}

QString QASTMethod::content() const{
    QString base = "";
    base += m_isStatic ? "static " : "";
    base += m_isVirtual ? "virtual " : "";
    base += m_returnType + " ";
    base += identifier();
    base += "(";

    bool firstArg = true;
    for( QList<csa::ast::QASTNode*>::const_iterator it = m_arguments.begin(); it != m_arguments.end(); ++it ){
        if ( firstArg ){
            base    += (*it)->content();
            firstArg = false;
        } else {
            base    += ", " + (*it)->content();
        }
    }
    base += ")";
    base += m_isConst ? " const" : "";
    base += m_isPureVirtual ? " = 0" : "";

    return base;
}

}}// namespace
