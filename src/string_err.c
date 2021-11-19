#include "string_err.h"

static const char err_str_nil[] = "";
static const char err_str_alloc[] = "allocation error";
static const char err_str_fmt[] = "formatting error";
static const char err_str_unknown[] = "unknown error";

/*
 * Return a pointer to a static read only string containing a textual
 * representation of the error code `err` passed in.
 */
const char *ss_err_str(ss_err err) {
    switch (err) {
        case err_none:
            return err_str_nil;
        case err_alloc:
            return err_str_alloc;
        case err_format:
            return err_str_fmt;
        default:
            return err_str_unknown;
    }
}