
#include "../common.h"
#include "node_id.h"

uint32_t uvt_node_id_distance(const uvt_node_id *const node1, const uvt_node_id *const node2, uvt_node_id *result) {

  for (uint32_t i = 0; i < 5; i++) {
    result->h[i] = node1->h[i] ^ node2->h[i];
  }

  return UVT_OK;

}

uint32_t uvt_node_id_count_lead_zeores(const uvt_node_id *const node1) {
  uint32_t c1 = 0;

  c1 += CLZ1(node1->h[0]);
  if (c1 < 32)
    return c1;

  c1 += CLZ1(node1->h[1]);
  if (c1 < 64)
    return c1;

  c1 += CLZ1(node1->h[2]);
  if (c1 < 96)
    return c1;

  c1 += CLZ1(node1->h[3]);
  if (c1 < 128)
    return c1;

  c1 += CLZ1(node1->h[4]);

  return c1;

}

uint32_t uvt_node_id_count_ones(const uvt_node_id *const node1) {
  uint32_t c1 = 0;

  c1 += CO(node1->h[0]);
  c1 += CO(node1->h[1]);
  c1 += CO(node1->h[2]);
  c1 += CO(node1->h[3]);
  c1 += CO(node1->h[4]);

  return c1;

}

uint32_t uvt_node_id_distance_exp(const uvt_node_id *const node1, const uvt_node_id *const node2) {
  uvt_node_id res;
  uvt_node_id_distance(node1, node2, &res);
//  return 160 - uvt_node_id_count_ones(&res);
  return uvt_node_id_count_lead_zeores(&res);
}