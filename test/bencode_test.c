//
// Created by Patricio on 8/26/16.
//



#include <stdio.h>
#include <inttypes.h>

#include "../src/bencode/bencode.h"


int main() {
    char *test_string1 = "4spam";
    char *test_string2 = "i12345678900e";
    char *test_string3 = "i30e";
    char *test_string4 = "d4spam5ojetee";

    char str[512];
    int64_t val;
    int64_t val2;

    be_node_t be;

    int err = be_init(&be, test_string1, strlen(test_string1));

    memset(str, 0x00, 512);

    if (err < 0) {
        printf("Error! %d", err);
    }

    err = be_read_string(&be, str, 512);

    if (err < 0) {
        printf("Error leyendo string %d\n", err);
    } else {
        printf("String : %s\n", str);
    }

    err = be_init(&be, test_string2, strlen(test_string2));

    err = be_read_integer(&be, &val);

    err = be_init(&be, test_string3, strlen(test_string2));

    err = be_read_integer(&be, &val2);

    err = be_init(&be, test_string4, strlen(test_string4));

    if (err < 0) {
        printf("Error leyendo integer %d\n", err);
    } else {
        printf("Integer : %" PRId64 "\n", val + val2);
    }

    if(be_get_type(&be) == BE_DIC) {
        err = be_read_string(&be, str, 512);
        if(err) printf("Error leyendo dic");
        printf("KEY : %s\n", str);
        err = err >= 0 ? be_read_string(&be, str, 512) : err;
        printf("VAL: %s\n", str);
    }


    return 0;
}