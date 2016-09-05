//
// Created by Patricio on 9/3/16.
//

#include "routing_table.h"

//  functions that mutates routing table struct
int uvt_routing_table_init(uvt_routing_table *const table, const uvt_node_id *node_id) {

  memcpy(&table->node_id, node_id, sizeof(uvt_node_id));

  table->node_count = 0;
  table->current = 0;

  memset(table->buckets, 0, sizeof(uvt_dht_bucket) * 30);

  return UVT_OK;
}

int uvt_routing_table_add(uvt_routing_table *const table, const uvt_dht_node *const node) {

  int dist = uvt_node_id_distance_exp(&node->id, &table->node_id);

  if (uvt_routing_table_node_exist(table, node) == UVT_TRUE) {
    printf("NOT ADDED\n");
    return UVT_NOT_ADDED;
  }

  int idx = UVT_IMIN(table->current, dist);

  if (table->buckets[idx].count == 8) {
    if (table->current == idx && dist > idx) {
      uvt__routing_table_split_bucket(table);
      idx = table->current;
    } else {
      return UVT_ERROR;
    }
  }

  if (table->buckets[idx].count < 8) {
    memcpy(&table->buckets[idx].nodes[table->buckets[idx].count], node, sizeof(uvt_dht_node));
    table->buckets[idx].count++;
  }

  return UVT_OK;
}

//  const functions that works on routing table struct data
int uvt_routing_table_node_exist(const uvt_routing_table *const table, const uvt_dht_node *const node) {
  for (int bi = 0; bi < table->current; bi++) {
    for (int ni = 0; ni < table->buckets[bi].count; ni++) {
      if (node->addr.sin_addr.s_addr == table->buckets[bi].nodes[ni].addr.sin_addr.s_addr) {
        return UVT_TRUE;
      }
    }
  }

  return UVT_FALSE;
}

// Private for routing table struct

// Split the current bucket of a routing table
int uvt__routing_table_split_bucket(uvt_routing_table *table) {

  uvt_dht_bucket *bucket = &table->buckets[table->current];

  uvt_dht_bucket tmp_bucket_1;
  uvt_dht_bucket tmp_bucket_2;

  if (table->current == 160)
    return UVT_ERROR;

  tmp_bucket_1.count = 0;
  tmp_bucket_2.count = 0;

  for (int i = 0; i < bucket->count; i++) {
    if (uvt_node_id_distance_exp(&bucket->nodes[i].id, &table->node_id) > table->current) {
      memcpy(&tmp_bucket_1.nodes[tmp_bucket_1.count], &bucket->nodes[i], sizeof(uvt_dht_node));
      tmp_bucket_1.count++;
    } else {
      memcpy(&tmp_bucket_2.nodes[tmp_bucket_2.count], &bucket->nodes[i], sizeof(uvt_dht_node));
      tmp_bucket_2.count++;
    }
  }

  memcpy(&table->buckets[table->current], &tmp_bucket_2, sizeof(uvt_dht_bucket));
  memcpy(&table->buckets[table->current + 1], &tmp_bucket_1, sizeof(uvt_dht_bucket));

  table->current++;

  return UVT_OK;
}
