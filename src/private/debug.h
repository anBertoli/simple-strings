
#ifndef SS_DEBUG_H
#define SS_DEBUG_H

#include "../string.h"
#include "../string_iter.h"
#include "../string_fmt.h"

void ss_debug(ss *s, int mem);
void ss_iter_debug(ss_iter *s, int mem);

#endif
