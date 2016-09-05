#ifndef UVT_ROUTING_TABLE
#define UVT_ROUTING_TABLE

#include <netinet/in.h>

#include "../common.h"
#include "node_id.h"


typedef struct uvt_dht_node_s {
  uvt_node_id id;
  struct sockaddr_in addr;
} uvt_dht_node;

typedef struct uvt_dht_bucket_s {
  uint8_t count;
  uvt_dht_node nodes[8]; // K = 8
} uvt_dht_bucket;

typedef struct uvt_routing_table_s {
  uint8_t current;
  uvt_node_id node_id; // this node id
  size_t node_count;
  uvt_dht_bucket buckets[160]; // reserve 30 buckets with k = 8, it should not pass the 26/27 buckets large.
} uvt_routing_table;


//  functions that mutates routing table struct
int uvt_routing_table_init(uvt_routing_table *const table, const uvt_node_id *node_id);

int uvt_routing_table_add(uvt_routing_table *const table, const uvt_dht_node *const node);

//  const functions that works on routing table struct data
int uvt_routing_table_node_exist(const uvt_routing_table *const table, const uvt_dht_node *const node);

// Private for routing table struct

// Split the current bucket of a routing table
int uvt__routing_table_split_bucket(uvt_routing_table *table);

#endif