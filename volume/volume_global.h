#ifndef VOLUME_GLOBAL_H
#define VOLUME_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(VOLUME_LIBRARY)
#  define VOLUME_EXPORT Q_DECL_EXPORT
#else
#  define VOLUME_EXPORT Q_DECL_IMPORT
#endif

#endif // VOLUME_GLOBAL_H
