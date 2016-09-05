
#include <stdint.h>

void sha1_compress(uint32_t state[5], const uint8_t block[64]);
void sha1_hash(const uint8_t *message, uint32_t len, uint32_t hash[5]);
