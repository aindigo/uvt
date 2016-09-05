#ifndef UVT_PROTOCOL_H
#define UVT_PROTOCOL_H

typedef struct uvt_error_msg_t {
  char t[2]; // transaction id
  uint8 error_code;
  char error_msg[50];
} uvt_error_msg_t;

typedef struct uvt_ping_req_s {
  char t[2];
  uvt_node_id node_id;
} uvt_ping_msg_t;

typedef struct uvt_ping_res_s {
  char t[2];
  uvt_node_id node_id;
} uvt_ping_res_t;

typedef struct uvt_find_node_req_s {
  char t[2];
  uvt_node_id node_id;
  uvt_node_id target;
} uvt_find_node_req_t;

typedef struct uvt_find_node_res_s {
  char t[2];
  uint8_t nodes_size;
  uvt_node_id node_id;
  uvt_compact_node_t nodes[8];
} uvt_find_node_res_t;

typedef struct uvt_get_peers_req_s {
  char t[2];
  uvt_node_id node_id;
  uvt_node_id info_hash;
} uvt_get_peers_res_t;

typedef struct uvt_get_peers_res1_s {
  char t[2];
  uint8_t peers_size;
  uvt_node_id node_id;
  uint8 token[8];
  uvt_compact_peer_t peers[30];
} uvt_get_peers_res1_t;

typedef struct uvt_get_peers_res2_s {
  char t[2];
  uint8_t nodes_size;
  uvt_node_id node_id;
  uvt_compact_node_t nodes[8];
} uvt_get_peers_res2_t;



#endif