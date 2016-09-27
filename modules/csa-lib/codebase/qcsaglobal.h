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


#ifndef QCSAGLOBAL_HPP
#define QCSAGLOBAL_HPP

#include <qglobal.h>

#ifndef Q_CSA_STATIC
#  ifdef Q_CSA_LIB
#    define Q_CSA_EXPORT Q_DECL_EXPORT
#  else
#    define Q_CSA_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define Q_CSA_EXPORT
#endif

#define CSA_VERSION_MAJOR 0
#define CSA_VERSION_MINOR 4
#define CSA_VERSION_PATCH 0

#define CSA_VERSION_STRINGIZE(major, minor, patch)   \
    #major"."#minor"."#patch

#define CSA_VERSION_STRING CSA_VERSION_STRINGIZE( \
    CSA_VERSION_MAJOR, \
    CSA_VERSION_MINOR, \
    CSA_VERSION_PATCH)

#endif // QCSAGLOBAL_HPP
