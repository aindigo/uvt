//
// Created by Patricio on 8/26/16.
//

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/param.h>
#include <string.h>

#include "../src/dht/node_id.h"
#include "../src/dht/routing_table.h"

// Test utility functions

void print_node_info(const uvt_routing_table *table, const uvt_dht_node *node) {
  printf("%08X %08X %08X %08X %08X \t\t%d\n", node->id.h[0], node->id.h[1], node->id.h[2], node->id.h[3],
         node->id.h[4],
         uvt_node_id_distance_exp(&node->id, &table->node_id));
}

void print_table(uvt_routing_table *table) {

  for (int i = 0; i <= table->current; i++) {
    printf("---- bucket: %d size : %d----\n", i, table->buckets[i].count);
    for (int x = 0; x < table->buckets[i].count; x++) {
      print_node_info(table, &table->nodes[i * 8 + x]);
    }
  }

}

int main(int argc, char **argv) {
  time_t t;
  srand(time(&t));
  uvt_node_id node_id;
  uvt_dht_node node_p;

  FILE *f1 = fopen("data1.dat", "wb+");
  FILE *f2 = fopen("data2.dat", "wb+");

  node_p.id.h[0] = (uint32_t) rand();
  node_p.id.h[0] = (node_p.id.h[0] << 8) ^ (uint32_t) rand();
  node_p.id.h[1] = (uint32_t) rand();
  node_p.id.h[1] = (node_p.id.h[1] << 8) ^ (uint32_t) rand();
  node_p.id.h[2] = (uint32_t) rand();
  node_p.id.h[2] = (node_p.id.h[2] << 8) ^ (uint32_t) rand();
  node_p.id.h[3] = (uint32_t) rand();
  node_p.id.h[3] = (node_p.id.h[3] << 8) ^ (uint32_t) rand();
  node_p.id.h[4] = (uint32_t) rand();
  node_p.id.h[4] = (node_p.id.h[4] << 8) ^ (uint32_t) rand();

  node_p.addr.sin_addr.s_addr = (uint32_t) rand();

  uvt_routing_table table;

  uvt_routing_table_init(&table, &node_p.id);

  uvt_routing_table_add(&table, &node_p);

  print_node_info(&table, &node_p);

  for (int i = 0; i < 192900; i++) {

    uvt_dht_node node;

    node.id.h[0] = (uint32_t) rand();
    node.id.h[0] = (node.id.h[0] << 16) ^ (uint32_t) rand();
    node.id.h[1] = (uint32_t) rand();
    node.id.h[1] = (node.id.h[1] << 16) ^ (uint32_t) rand();
    node.id.h[2] = (uint32_t) rand();
    node.id.h[2] = (node.id.h[2] << 16) ^ (uint32_t) rand();
    node.id.h[3] = (uint32_t) rand();
    node.id.h[3] = (node.id.h[3] << 16) ^ (uint32_t) rand();
    node.id.h[4] = (uint32_t) rand();
    node.id.h[4] = (node.id.h[4] << 16) ^ (uint32_t) rand();

    node.addr.sin_addr.s_addr = (uint32_t) rand();

    uvt_routing_table_add(&table, &node);

  }

  print_table(&table);

  return 0;
}