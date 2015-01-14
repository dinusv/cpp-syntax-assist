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


#include "CSAFileRoot.hpp"
#include "SourceLocation.hpp"
#include "TokenClassifier.hpp"

namespace csa{ namespace ast{

QASTFile::QASTFile(const std::string& file, SourceLocation *endOfFile)
    : QASTNode("file",
              new SourceLocation(file, 0, 0, 0),
              new SourceLocation(file, 0, 0, 0),
              endOfFile, 0)
    , m_identifier(file){


}

}}//namespace
