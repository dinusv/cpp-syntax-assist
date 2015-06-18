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


#ifndef QASTFILECONVERT_HPP
#define QASTFILECONVERT_HPP

#include <QScriptEngine>
#include <QScriptValue>
#include "QASTFile.hpp"

namespace csa{

// JS Conversion
// -------------

QScriptValue Q_CSA_EXPORT fileToScriptValue(QScriptEngine *engine, ast::QASTFile* const & file);
void Q_CSA_EXPORT fileFromScriptValue(const QScriptValue &obj, ast::QASTFile* &file);

}// namespace

#endif // QASTFILECONVERT_HPP
