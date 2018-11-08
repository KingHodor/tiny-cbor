#ifndef TINY_CBOR_CBOR_H
#define TINY_CBOR_CBOR_H

#include <stdint.h>
#include <stdio.h>

#define PB_BYTES_ARRAY_T(n) struct { uint8_t size; uint8_t bytes[n]; }

typedef unsigned char u_int8_t;
typedef PB_BYTES_ARRAY_T(512) cbor_bytes;

typedef enum {
    MT_INVALID = -1,
    MT_UNSIGNED = 0,
    MT_NEGATIVE = 1,
    MT_BYTES = 2,
    MT_TEXT = 3,
    MT_ARRAY = 4,
    MT_MAP = 5,
    MT_TAG = 6,
    MT_SPECIAL = 7
} MajorType;

typedef enum {
    DIRECT = -1,
    ONE_BYTE = 24,
    TWO_BYTES = 25,
    FOUR_BYTES = 26,
    EIGHT_BYTES = 27,
    RESERVED = 28,
    INDEFINITE = 31
} AdditionalInformation;


typedef struct cborMapItem {
    char key[128];
    struct cborItem *value;
} cborMapItem;

typedef struct cborItem {
    MajorType type;
    /** CBOR_BYTES */
    cbor_bytes bytes;
    /** CBOR_TEXT */
    char str[512];
    /** CBOR_INT */
    unsigned int uint;
    /** CBOR_TAG*/
    long tag;

    /** Number of children.*/
    int length;
    struct cborItem *firstItem;
    struct cborItem *nextItem;
} cborItem;

int cbor_encode(u_int8_t *new_bytes, size_t *written, cborItem *item);

void cbor_create_integer(cborItem *item, unsigned int value);

void cbor_create_tag(cborItem *item, uint8_t *value, size_t value_size, long tag);

void cbor_create_bytes(cborItem *item, uint8_t *bytes, size_t byte_size);

void cbor_create_array(cborItem *parent);

void cbor_add_item_to_array(cborItem *parent, cborItem *newItem);

void cbor_create_map(cborItem *parent);

#endif //TINY_CBOR_CBOR_H
