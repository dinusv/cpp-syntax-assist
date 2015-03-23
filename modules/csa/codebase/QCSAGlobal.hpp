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

#endif // QCSAGLOBAL_HPP
