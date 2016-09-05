//
// Created by Patricio on 9/4/16.
//

#ifndef UVT_NODE_H
#define UVT_NODE_H

#include <stdint.h>

#include "node_id.h"

typedef struct uvt_compact_peer_s {
  uint8_t ip[4];
  uint8_t port[2];
} uvt_compact_peer_t;

typedef struct uvt_compact_node_s {
  uvt_node_id node_id;
  uvt_compact_peer_t peer_info;
} uvt_compact_node_t;


#endif //UVT_NODE_H
