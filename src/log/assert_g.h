#ifndef ASSERT_H
#define ASSERT_H

#ifdef DEBUG
    #include "log.h"
    #define ASSERT(x, m) if (!(x)) { LOG_ERROR(m); exit(-1); }
#else
    #define ASSERT(x, m)
#endif

#endif