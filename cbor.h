//
// Created by Zehra Alptekin on 11/5/18.
//

#ifndef CORE_WALLET_CBOR_H
#define CORE_WALLET_CBOR_H

#include "convert_util.h"

//typedef PB_BYTES_ARRAY_T(128) cbor_bytes;
struct cbor_bytes {
    pb_size_t size;
    pb_byte_t bytes[128];
};
typedef struct cbor_bytes cbor_bytes;

typedef enum {
    MT_INVALID = -1,
    MT_UNSIGNED = 0,
    MT_NEGATIVE = 1,
    MT_BYTES = 2,
    MT_TEXT = 3,
    MT_ARRAY = 4,
    MT_MAP = 5,
    MT_TAG = 6,
    MT_PRIMITIVE = 7,
    MT_INDEFINITE = 8,
    MT_RAW = 9

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
    long uint;
    /** CBOR_TAG*/
    long tag;

    /** Number of children.*/
    int length;
    struct cborItem *firstItem;
    struct cborItem *nextItem;
} cborItem;

void cbor_encode_indefinite_length_array(u_int8_t *new_bytes, size_t *written, cborItem * parent);

int cbor_encode(u_int8_t *new_bytes, size_t *written, cborItem *item);

void cbor_create_integer(cborItem *item, long value);

void cbor_create_tag(cborItem *item, uint8_t *value, size_t value_size, long tag);

void cbor_create_bytes(cborItem *item, uint8_t *bytes, size_t byte_size);

void cbor_create_raw(cborItem *item, uint8_t *bytes, size_t byte_size);

void cbor_create_array(cborItem *parent);

void cbor_infinite_create_array(cborItem * parent);

void cbor_add_item_to_array(cborItem *parent, cborItem *newItem);

void cbor_create_map(cborItem *parent);

#endif //CORE_WALLET_CBOR_H
