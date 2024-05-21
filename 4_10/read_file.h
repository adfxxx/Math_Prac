#ifndef RF_H
#define RF_H

#define _GNU_SOURCE
#include "status.h"
#include "types.h"
#include <string.h>
#define OP_COUNT 13

Status read_file_with_settings(FILE *file, char **save, char **opers, Syntax *sntx);

#endif