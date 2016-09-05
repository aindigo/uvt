//
// Created by Patricio on 8/24/16.
//

#ifndef UV_BENCODE_H_H
#define UV_BENCODE_H_H

#include <stdio.h>
#include <uv.h>

typedef enum {
  BE_OK = 0,
  BE_NOT_STR = -1,  // Not a string
  BE_NOT_VALID = -2, // Invalid string
  BE_STR = 1,
  BE_LST = 2,
  BE_DIC = 3,
  BE_INT = 4,
} be_type;

typedef struct be_node_s {
  const char *buf;
  size_t start;
  size_t len;
  be_type type;
} be_node_t;

int be_init(be_node_t *self, const char *buf, const size_t len);
be_type be_get_type(const be_node_t *self);
int be_read_string(be_node_t *self, char *str, size_t size);
int be_read_integer(be_node_t *self, int64_t *val);

#endif //UV_BENCODE_H_H
