/*
 * Copyright (c) 2012 Rogerz Zhang <rogerz.zhang@gmail.com>
 *
 * Jansson is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#include <jansson.h>
#include <string.h>
#include "util.h"

static void test_object(void)
{
    json_t *json;

    // {
    //  "obj": {
    //      "int": 2,
    //      "arr": [
    //          3,
    //          { "nest": 4}
    //      ]
    //  }
    // }

    json = json_pack("{s:{s:i,s:[i,{s:i},[i]]}}", "obj", "int", 2, "arr", 3, "nest", 4, 5);

        if(json_path_get(json, "$") != json)
                fail("json_path_get() fails to get root json");
    if(json_integer_value(json_path_get(json, "$.obj.int")) != 2)
        fail("json_path_get() fails to get integer");
    if(!json_is_array(json_path_get(json, "$.obj.arr")))
        fail("json_path_get() fails to get array");
    if(json_integer_value(json_path_get(json, "$.obj.arr[0]")) != 3)
        fail("json_path_get() fails to get array member");
    if(json_integer_value(json_path_get(json, "$.obj.arr[1].nest")) !=4)
        fail("json_path_get() fails to get key of array member");
    if(json_integer_value(json_path_get(json, "$.obj.arr[2][0]")) != 5)
        fail("json_path_get() fails to get nest array");

//    if(json_path_get(json, "$this.obj") != NULL)
//        fail("json_path_get() fails to detect error: invalid root path");
    if(json_path_get(json, "$obj") != NULL)
        fail("json_path_get() fails to detect errro: missing '.'");
    if(json_path_get(json, "obj") != NULL)
        fail("json_path_get() fails to detect error: missing root '$'");
    if(json_path_get(json, "$[0]") != NULL)
        fail("json_path_get() fails to detect error: index in json");
    if(json_path_get(json, "$[0].int") != NULL)
        fail("json_path_get() fails to detect error: key in undefined");
    if(json_path_get(json, "$.ob") != NULL)
        fail("json_path_get() fails to detect error: non-exist path");

    json_decref(json);
}

static void test_array(void)
{
    json_t *json;

    // [
    //  10,
    //  { "foo": 11},
    //  [12]
    // ]

    json = json_pack("[i, {s:i}, [i]]", 10, "foo", 11, 12);

        if(json_path_get(json, "$") != json)
                fail("json_path_get() fails to get root array");

    if(json_integer_value(json_path_get(json, "$[0]")) != 10)
        fail("json_path_get() fails to get member in root array");

    if(json_integer_value(json_path_get(json, "$[1].foo")) != 11)
        fail("json_path_get() fails to get key of array member in root array");

    if(json_integer_value(json_path_get(json, "$[2][0]")) != 12)
        fail("json_path_get() fails to get nested array in root array");

    if(json_path_get(json, "[0]") != NULL)
        fail("json_path_get() fails to detect error: missing root in array");
    if(json_path_get(json, "$.[0]") != NULL)
        fail("json_path_get() fails to detect error: extra '.' in array");
    if(json_path_get(json, "$.0") != NULL)
        fail("json_path_get() fails to detect error: index as key");
    if(json_path_get(json, "$[0z]") != NULL)
        fail("json_path_get() fails to detect error: invalid index");
    if(json_path_get(json, "$.foo") != NULL)
        fail("json_path_get() fails to detect error: key in array");
    if(json_path_get(json, "$.foo[0]") != NULL)
        fail("json_path_get() fails to detect error: index on undefined");
    if(json_path_get(json, "$[3]") != NULL)
        fail("json_path_get() fails to detect error: array overflow");

    json_decref(json);

}

static void test_null()
{
    json_t *json = json_pack("[i, {s:i}, [i]]", 10, "foo", 11, 12);
    if(json_path_get(NULL, "$.foo") != NULL)
        fail("json_path_get() fails to detect NULL json");

    if(json_path_get(json, NULL) != NULL)
        fail("json_path_get() fails to detect NULL path");

    if(json_path_get(json, "") != NULL)
        fail("json_path_get() fails to detect empty string");

    json_decref(json);
}


static void run_tests()
{
    test_object();
    test_array();
    test_null();
}
