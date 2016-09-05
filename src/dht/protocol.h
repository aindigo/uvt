#ifndef UVT_PROTOCOL_H
#define UVT_PROTOCOL_H

#include "../common.h"
#include "node_id.h"
#include "node.h"

typedef enum uvt_msg_types_e {
  UVT_ERROR_MSG = 0,
  UVT_PING_MSG = 1,
  UVT_PING_RES_MSG = 2,
  UVT_FIND_NODE_MSG = 3,
  UVT_FIND_NODE_RES_MSG = 4,
  UVT_GET_PEERS_MSG = 5,
  UVT_GET_PEERS_RES1_MSG = 5,
  UVT_GET_PEERS_RES2_MSG = 6,
  UVT_ANNOUNCE_MSG = 7,
  UVT_ANNOUNCE_RES_MSG = 8
} uvt_msg_types_t;

typedef struct uvt_msg_header_s {
  uvt_msg_types_t type;
  char t[2];
  uvt_node_id node_id;
} uvt_msg_header_t;

typedef struct uvt_error_msg_t {
  uvt_msg_header_t header;
  uint8_t error_code;
  char error_msg[50];
} uvt_error_msg_t;

typedef struct uvt_ping_req_s {
  uvt_msg_header_t header;
} uvt_ping_msg_t;

typedef struct uvt_ping_res_s {
  uvt_msg_header_t header;
} uvt_ping_res_t;

typedef struct uvt_find_node_req_s {
  uvt_msg_header_t header;
  uvt_node_id target;
} uvt_find_node_req_t;

typedef struct uvt_find_node_res_s {
  uvt_msg_header_t header;
  uint8_t nodes_size;
  uvt_compact_node_t nodes[8];
} uvt_find_node_res_t;

typedef struct uvt_get_peers_req_s {
  uvt_msg_header_t header;
  uvt_node_id info_hash;
} uvt_get_peers_res_t;

typedef struct uvt_get_peers_res1_s {
  uvt_msg_header_t header;
  uint8_t peers_size;
  uint8_t token[8];
  uvt_compact_peer_t peers[30];
} uvt_get_peers_res1_t;

typedef struct uvt_get_peers_res2_s {
  uvt_msg_header_t header;
  uint8_t nodes_size;
  uvt_compact_node_t nodes[8];
} uvt_get_peers_res2_t;

typedef struct uvt_announce_peer_req_s {
  uvt_msg_header_t header;
  uint8_t implied_port;
  uint16_t port;
  uint8_t token[8];
} uvt_announce_peer_req_t;

typedef struct uvt_announce_peer_res_s {
  uvt_msg_header_t header;
};

#endif