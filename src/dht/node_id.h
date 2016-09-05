
#ifndef UVT_NODE_ID
#define UVT_NODE_ID

#include <stdint.h>

typedef struct uvt_node_id_s {
  uint32_t h[5];
} uvt_node_id;

uint32_t uvt_node_id_distance(const uvt_node_id *const node1, const uvt_node_id *const node2, uvt_node_id *result);
uint32_t uvt_node_id_count_lead_zeores(const uvt_node_id *const node1);
uint32_t uvt_node_id_count_ones(const uvt_node_id *const node1);
uint32_t uvt_node_id_distance_exp(const uvt_node_id *const node1, const uvt_node_id *const node2);

static inline uint32_t CLZ1(uint32_t x) {
  static uint8_t const clz_lkup[] = {
      32U, 31U, 30U, 30U, 29U, 29U, 29U, 29U,
      28U, 28U, 28U, 28U, 28U, 28U, 28U, 28U,
      27U, 27U, 27U, 27U, 27U, 27U, 27U, 27U,
      27U, 27U, 27U, 27U, 27U, 27U, 27U, 27U,
      26U, 26U, 26U, 26U, 26U, 26U, 26U, 26U,
      26U, 26U, 26U, 26U, 26U, 26U, 26U, 26U,
      26U, 26U, 26U, 26U, 26U, 26U, 26U, 26U,
      26U, 26U, 26U, 26U, 26U, 26U, 26U, 26U,
      25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
      25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
      25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
      25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
      25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
      25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
      25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
      25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U
  };
  uint32_t n;
  if (x >= (1U << 16)) {
    if (x >= (1U << 24)) {
      n = 24U;
    } else {
      n = 16U;
    }
  } else {
    if (x >= (1U << 8)) {
      n = 8U;
    } else {
      n = 0U;
    }
  }
  return (uint32_t) clz_lkup[x >> n] - n;
}

static inline uint32_t CO(uint32_t i) {
  i = i - ((i >> 1) & 0x55555555);
  i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
  return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

#endif