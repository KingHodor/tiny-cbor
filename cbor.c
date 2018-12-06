#include <string.h>
#include "cbor.h"
#include "error_types.h"


static void cbor_write_bytes(const u_int8_t *bytes, const size_t bytes_size, u_int8_t *new_bytes, size_t *written) {
    for (int i = 0; i < bytes_size; ++i) {
        new_bytes[(*written) + i] = bytes[i];
    }
    *written = *written + bytes_size;
}

static void cbor_write(u_int8_t value, u_int8_t *new_bytes, size_t *written) {
    new_bytes[*written] = value;
    *written = *written + 1;
}

static void cbor_encode_type_and_length(MajorType majorType,  long length, u_int8_t *new_bytes, size_t *written) {
    int symbol = majorType << 5;
    if (length <= 23L) {
        cbor_write((u_int8_t) (symbol | length), new_bytes, written);
    }else if (length <= 255L) {
        symbol |= ONE_BYTE;
        cbor_write((u_int8_t) symbol, new_bytes, written);
        cbor_write((u_int8_t) length, new_bytes, written);
    } else if (length <= 65535L) {
        symbol |= TWO_BYTES;
        cbor_write((u_int8_t) symbol, new_bytes, written);
        cbor_write((u_int8_t) (length >> 8), new_bytes, written);
        cbor_write((u_int8_t) (length & 0xFF), new_bytes, written);
    } else if (length <= 4294967295L) {
        symbol |= FOUR_BYTES;
        cbor_write((u_int8_t) symbol, new_bytes, written);
        cbor_write((u_int8_t) ((length >> 24) & 0xFF), new_bytes, written);
        cbor_write((u_int8_t) ((length >> 16) & 0xFF), new_bytes, written);
        cbor_write((u_int8_t) ((length >> 8) & 0xFF), new_bytes, written);
        cbor_write((u_int8_t) (length & 0xFF), new_bytes, written);
    } else {
        symbol |= EIGHT_BYTES;
        cbor_write((u_int8_t) symbol, new_bytes, written);
        cbor_write((u_int8_t) ((length >> 56) & 0xFF), new_bytes, written);
        cbor_write((u_int8_t) ((length >> 48) & 0xFF), new_bytes, written);
        cbor_write((u_int8_t) ((length >> 40) & 0xFF), new_bytes, written);
        cbor_write((u_int8_t) ((length >> 32) & 0xFF), new_bytes, written);
        cbor_write((u_int8_t) ((length >> 24) & 0xFF), new_bytes, written);
        cbor_write((u_int8_t) ((length >> 16) & 0xFF), new_bytes, written);
        cbor_write((u_int8_t) ((length >> 8) & 0xFF), new_bytes, written);
        cbor_write((u_int8_t) (length & 0xFF), new_bytes, written);
    }
}

static void cbor_encode_array(u_int8_t *new_bytes, size_t *written, cborItem * parent){
    cbor_encode_type_and_length(MT_ARRAY, parent->length, new_bytes, written);
    cborItem *current = parent->firstItem;
    while (current != NULL){
        cbor_encode(new_bytes, written, current);
        current = current->nextItem;
    }
}

static void cbor_encode_positive_int(u_int8_t *new_bytes, size_t *written, long value){
    cbor_encode_type_and_length(MT_UNSIGNED, value, new_bytes, written);
}

static void cbor_encode_negative_int(u_int8_t *new_bytes, size_t *written, long value){
    cbor_encode_type_and_length(MT_NEGATIVE, value, new_bytes, written);
}

static void cbor_encode_bytes(u_int8_t *new_bytes, size_t *written, const u_int8_t *bytes, size_t bytes_size){
    cbor_encode_type_and_length(MT_BYTES, bytes_size, new_bytes, written);
    cbor_write_bytes(bytes, bytes_size, new_bytes, written);
}

static void cbor_encode_raw(u_int8_t *new_bytes, size_t *written, const u_int8_t *bytes, size_t bytes_size){
    cbor_write_bytes(bytes, bytes_size, new_bytes, written);
}

static void cbor_encode_map(u_int8_t *new_bytes, size_t *written, cborItem * parent){
    cbor_encode_type_and_length(MT_MAP, 0, new_bytes, written);
}


static void cbor_encode_tag(u_int8_t *new_bytes, size_t *written, uint8_t *value, size_t value_size, long tag){
    cbor_encode_type_and_length(MT_TAG, tag, new_bytes, written);
    cbor_encode_bytes(new_bytes, written, value, value_size);
}

void cbor_encode_indefinite_length_array(u_int8_t *new_bytes, size_t *written, cborItem * parent){
    uint8_t symbol_array = (MT_ARRAY << 5) + 31;
    cbor_write_bytes(&symbol_array, 1, new_bytes, written);
    cborItem *current = parent->firstItem;
    while (current != NULL){
        cbor_encode(new_bytes, written, current);
        current = current->nextItem;
    }
    uint8_t symbol_primitive = (MT_PRIMITIVE << 5) + 31;
    cbor_write_bytes(&symbol_primitive, 1, new_bytes, written);
}

int cbor_encode(u_int8_t *new_bytes, size_t *written, cborItem * item){
    switch(item->type) {

        case MT_UNSIGNED  :
            cbor_encode_positive_int(new_bytes, written, item->uint);
            break;
        case MT_NEGATIVE  :
            cbor_encode_negative_int(new_bytes, written, item->uint);
            break;
        case MT_BYTES  :
            cbor_encode_bytes(new_bytes, written, item->bytes.bytes, item->bytes.size);
            break;
        case MT_TEXT  :
            //
            break;
        case MT_ARRAY  :
            cbor_encode_array(new_bytes, written, item);
            break;
        case MT_MAP  :
            cbor_encode_map(new_bytes, written, item);
            break;
        case MT_TAG  :
            cbor_encode_tag(new_bytes, written, item->bytes.bytes, item->bytes.size ,item->tag);
            break;
        case MT_INDEFINITE  :
            cbor_encode_indefinite_length_array(new_bytes, written, item);
            break;
        case MT_RAW :
            cbor_encode_raw(new_bytes, written, item->bytes.bytes, item->bytes.size);
            break;
        default :
            return CARDANO_ENCOING_ERROR;
    }
    return NO_ERROR;
}

void cbor_create_integer(cborItem * item, long value){
    long singedValue = value;
    if (singedValue < 0){
        item->type = MT_NEGATIVE;
    }else{
        item->type = MT_UNSIGNED;
    }
    item->uint = value;
}


void cbor_create_tag(cborItem * item, uint8_t *value, size_t value_size, long tag){
    item->type = MT_TAG;
    item->tag = tag;
    memcpy(item->bytes.bytes, value, value_size);
    item->bytes.size = (pb_size_t) value_size;
}


void cbor_create_bytes(cborItem * item, uint8_t *bytes, size_t byte_size){
    item->type = MT_BYTES;
    memset(item->bytes.bytes, 0 , 128);
    item->bytes.size = (pb_size_t) byte_size;
    memcpy(item->bytes.bytes, bytes, byte_size);
}


void cbor_create_raw(cborItem * item, uint8_t *bytes, size_t byte_size){
    item->type = MT_RAW;
    memset(item->bytes.bytes, 0 , 128);
    item->bytes.size = (pb_size_t) byte_size;
    memcpy(item->bytes.bytes, bytes, byte_size);
}

void cbor_create_array(cborItem * parent){
    parent->type = MT_ARRAY;
    parent->length = 0;
    parent->nextItem = NULL;
    parent->firstItem = NULL;
}

void cbor_infinite_create_array(cborItem * parent){
    parent->type = MT_INDEFINITE;
    parent->length = 0;
    parent->nextItem = NULL;
    parent->firstItem = NULL;
}

void cbor_add_item_to_array(cborItem * parent, cborItem * newItem){
    if(parent->firstItem == NULL){
        parent->firstItem = newItem;
    }else{
        cborItem * current = parent->firstItem;
        while(current->nextItem != NULL){
            current = current->nextItem;
        }
        current->nextItem = newItem;
    }
    newItem->nextItem = NULL;
    parent->length = parent->length + 1;
}

void cbor_create_map(cborItem * parent){
    parent->type = MT_MAP;
}



