#ifndef DEBUG_H
#define DEBUG_H

#include "unitris.h"

#ifdef __cplusplus
extern "C" {
#endif

void DBG_DumpGrid(const unitris_t *ctx);
void DBG_BufferToFile(const char *buffer, uint32_t size, const char *filename);

#ifdef __cplusplus
}
#endif

#endif // DEBUG_H
