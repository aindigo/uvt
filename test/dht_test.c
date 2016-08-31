//
// Created by Patricio on 8/26/16.
//

#include <stdio.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <inttypes.h>
#include <math.h>
#include <sys/param.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

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

typedef enum {
    UVT_FALSE = 0,
    UVT_TRUE = 1
} uvt_bool;

typedef enum uvt_return_codes_e {
    UVT_OK = 0,
    UVT_ERROR = -1,
    UVT_NOT_ADDED = -2
} uvt_return_codes;

typedef struct uvt_node_id_s {
    uint32_t h[5];
} uvt_node_id;

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
    uvt_dht_bucket buckets[30]; // reserve 30 buckets with k = 8, it should not pass the 26/27 buckets large.
} uvt_routing_table;

int uvt_dht_distance(const uvt_node_id *node1, const uvt_node_id *node2, uvt_node_id *result) {

    for (uint32_t i = 0; i < 5; i++) {
        result->h[i] = node1->h[i] ^ node2->h[i];
    }

    return UVT_OK;

}

int uvt_count_lead_zeores(const uvt_node_id *node1) {
    uint32_t c1 = 0;

    for (int i = 0; i < 5; i++) {
        c1 += CLZ1(node1->h[i]);
        if (c1 < (i + 1) * 32) break;
    }

    return c1;

}

int uvt_distance_exp(const uvt_node_id *node1, const uvt_node_id *node2) {

    uvt_node_id res;
    uvt_dht_distance(node1, node2, &res);
    return uvt_count_lead_zeores(&res);

}

int uvt_routing_table_init(uvt_routing_table *routing_table) {
    routing_table->current = 0;
    routing_table->node_count = 0;

    return UVT_OK;
}

int node_exist(const uvt_routing_table *table, const uvt_dht_node *node) {
    for (int bi = 0; bi < table->current; bi++) {
        for (int ni = 0; ni < table->buckets[bi].count; ni++) {
            if (node->addr.sin_addr.s_addr == table->buckets[bi].nodes[ni].addr.sin_addr.s_addr) {
                return UVT_TRUE;
            }
        }
    }

    return UVT_FALSE;
}

int uvt_split_bucket(uvt_routing_table *table) {

    uvt_dht_bucket *bucket = &table->buckets[table->current];

    uvt_dht_bucket tmp_bucket_1;
    uvt_dht_bucket tmp_bucket_2;

    size_t tbl_idx = table->current;
    size_t bkt_idx = 0;

    if (table->current == 30) return UVT_ERROR;

    tmp_bucket_1.count = 0;
    tmp_bucket_2.count = 0;

    for (int i = 0; i < bucket->count; i++) {
        if (uvt_distance_exp(&bucket->nodes[i].id, &table->node_id) > table->current) {
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

int uvt_routing_table_add(uvt_routing_table *table, uvt_dht_node *node) {

    int dist = uvt_distance_exp(&node->id, &table->node_id);

    if (node_exist(table, node) == UVT_TRUE) {
        printf("NOT ADDED\n");
        return UVT_NOT_ADDED;
    }

    int idx = MIN(table->current, dist);

    if (table->buckets[idx].count == 8) {
        if (table->current == idx && dist > idx ) {
            uvt_split_bucket(table);
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

void print_node_info(const uvt_routing_table *table, const uvt_dht_node *node) {
    printf("%08X %08X %08X %08X %08X \t\t%d\n", node->id.h[0], node->id.h[1], node->id.h[2], node->id.h[3], node->id.h[4],
           uvt_distance_exp(&node->id, &table->node_id));
}

void print_table(uvt_routing_table *table) {

    for (int i = 0; i <= table->current; i++) {
        printf("---- 请收藏我们的网址 bucket: %d size : %d----\n", i, table->buckets[i].count);
        for (int x = 0; x < table->buckets[i].count; x++) {
            print_node_info(table, &table->buckets[i].nodes[x]);
        }
    }

}

int uvt_init_table(uvt_routing_table* table, const uvt_node_id* node_id) {

    memcpy(&table->node_id, node_id, sizeof(uvt_node_id));

    table->node_count = 0;
    table->current = 0;

    memset(table->buckets, 0, sizeof(uvt_dht_bucket) * 30);

    return UVT_OK;
}

int main(int argc, char **argv) {
    time_t t;
    srand(time(&t));
    uvt_node_id node_id;
    uvt_dht_node node_p;

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

    uvt_init_table(&table, &node_p.id);

    uvt_routing_table_add(&table, &node_p);

    print_node_info(&table, &node_p);

    for(int i = 0; i < 1224900; i++) {

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