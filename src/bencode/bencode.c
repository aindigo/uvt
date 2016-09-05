//
// Created by Patricio on 8/24/16.
//
#include <stdio.h>
#include "bencode.h"

int be_init(be_node_t *self, const char *buf, const size_t len) {
  self->buf = buf;
  self->len = len;
  self->start = 0;

  return BE_OK;
}

be_type be_get_type(const be_node_t *self) {

  if (self->len <= 0)
    return BE_NOT_VALID;

  switch (self->buf[0]) {
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':return BE_STR;
  case 'i':return BE_INT;
  case 'd':return BE_DIC;
  case 'l':return BE_LST;
  default:return BE_NOT_VALID;
  }
}

int be_read_string(be_node_t *self, char *str, size_t size) {

  size_t n = self->start;
  size_t str_len = 0;

  if (self->len < 3)
    return BE_NOT_VALID;

  if (self->buf[n] == 'd' || self->buf[n] == 'l') {
    n += 1;
  }

  while (self->buf[n] > 47 && self->buf[n] < 58 && n < self->len) {
    str_len += (str_len * 10) + (self->buf[n] - 48);
    n++;
  }

  if (n > self->len) {
    self->type = BE_NOT_VALID;
    return BE_NOT_VALID;
  }

  if (size >= str_len) {
    memcpy(str, &self->buf[n], str_len);
  } else {
    memcpy(str, &self->buf[n], size);
  }

  self->start = n + str_len;
  self->len = self->len - n;

  return BE_OK;

}

int be_read_integer(be_node_t *self, int64_t *val) {

  size_t n = self->start + 1;
  int mul = 1;

  if (self->buf[0] != 'i' || self->len < 3)
    return BE_NOT_VALID;

  if (self->buf[n] == '0' && self->buf[n + 1] == 'e') {
    *val = 0;
    if (n + 1 > self->len)
      self->type = BE_NOT_VALID;
    self->start = n + 2;
    return BE_OK;
  }

  if (self->buf[n] == '-') {
    mul = -1;
    n++;
  }

  while ((self->buf[n] > 47 && self->buf[n] < 58)) {
    *val = ((*val) * 10) + (self->buf[n] - 48);
    n++;
  }

  if (self->buf[n] != 'e')
    return BE_NOT_VALID;

  if (n >= self->len)
    self->type = BE_NOT_VALID;
  self->start = n + 1;

  *val = *val * mul;
  return BE_OK;

}

