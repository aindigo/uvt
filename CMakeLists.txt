cmake_minimum_required(VERSION 3.6)
project(uv)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

# Point CMake at any custom modules we may ship
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(LIBUV_USE_STATIC true)

find_package(LibUV REQUIRED)

include_directories(SYSTEM ${LIBUV_INCLUDE_DIRS})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(SOURCE_FILES src/main.c src/bencode/bencode.c src/bencode/bencode.h src/sha1/sha1-fast.c)
add_executable(uv ${SOURCE_FILES})

set(BENCODE_FILES test/bencode_test.c src/bencode/bencode.c)
add_executable(bencode_test ${BENCODE_FILES})

set(DHT_FILES test/dht_test.c )
add_executable(dht_test ${DHT_FILES})

set(SHA1_FILES test/sha1_test.c src/sha1/sha1-fast.c)
add_executable(sha1_test ${SHA1_FILES})

target_link_libraries(uv ${LIBUV_LIBRARIES})
target_link_libraries(bencode_test ${LIBUV_LIBRARIES})
target_link_libraries(sha1_test ${LIBUV_LIBRARIES})
target_link_libraries(dht_test ${LIBUV_LIBRARIES})
