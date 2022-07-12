#ifndef LOCATION_GLOBAL_H
#define LOCATION_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LOCATION_LIBRARY)
#  define LOCATION_EXPORT Q_DECL_EXPORT
#else
#  define LOCATION_EXPORT Q_DECL_IMPORT
#endif

#endif // LOCATION_GLOBAL_H
