#ifndef QTWRAPPER_GLOBAL_H
#define QTWRAPPER_GLOBAL_H

#include <Qt/qglobal.h>

#ifdef QTWRAPPER_LIB
# define QTWRAPPER_EXPORT Q_DECL_EXPORT
#else
# define QTWRAPPER_EXPORT Q_DECL_IMPORT
#endif

#endif // QTWRAPPER_GLOBAL_H
