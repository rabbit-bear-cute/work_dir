#ifndef _IC_HASH_H
#define _IC_HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

uint32_t murmurhash32(const void *data, size_t nbytes);

bool murmurhash128(const void *data, size_t nbytes, void *retbuf);

#endif
