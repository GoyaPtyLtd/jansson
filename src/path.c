/*
 * Copyright (c) 2012 Rogerz Zhang <rogerz.zhang@gmail.com>
 *
 * Jansson is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#include <string.h>

#include <jansson.h>
#include "jansson_private.h"

json_t *json_path_get(const json_t *json, const char *path)
{
    static const char *root_token = "$", *path_delim = ".[", *array_close = "]";
    static const char array_delim = '[';
    json_t *target;
    char *token = NULL, *buf, *peek, *endptr;
    const char *expect;
    int error = 0;

    if (!json || !path || path[0] != '$')
        return NULL;
    else
        buf = jsonp_strdup(path);

    peek = buf;
    target = (json_t *)json;
    expect = root_token;

    while (peek && *peek && target)
    {
        char stop_chr;

        peek = strpbrk(peek, expect);
        if (peek) {
            stop_chr = *peek;
            *peek++ = '\0';
        } else {
            if (expect == path_delim && token) {
                stop_chr = '\0';
            } else {
                error = 1;  // incomplete path
                break;
            }
        }

        if (expect == path_delim) {
            if (token) {
                target = json_object_get(target, token);
            }
            expect = (stop_chr == array_delim ? array_close : path_delim);
            token = peek;
        } else if (expect == array_close) {
            size_t index = strtol(token, &endptr, 0);
            if (*endptr) {  // invalid number format
                error = 1;
                break;
            }
            target = json_array_get(target, index);
            token = NULL;
            expect = path_delim;
        } else if (expect == root_token) {
            expect = path_delim;
        } else {
            error = 1;  // expecting nothing?
            break;
        }
    }

    jsonp_free(buf);

    return error ? NULL : target;
}

