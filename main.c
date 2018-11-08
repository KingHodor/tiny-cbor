#include <stdio.h>
#include <stdint.h>
#include "cbor.h"
#include "utils.h"

void test1() {
    uint8_t result[64];
    char result_hex[128];
    size_t written;
    cborItem cborInteger;
    cbor_create_integer(&cborInteger, 1000000);
    cbor_encode(result, &written, &cborInteger);
    int8_to_char(result, written, result_hex);
    check_print("1a000f4240", result_hex, "cbor test 1");
}

void test2() {
    uint8_t result[64];
    char result_hex[128];
    size_t written = 0;
    cborItem cborBytes;

    uint8_t value[4] = { 0x01, 0x02, 0x03, 0x04};
    cbor_create_bytes(&cborBytes, value, 4);

    cbor_encode(result, &written, &cborBytes);
    int8_to_char(result, written, result_hex);
    check_print("4401020304", result_hex, "cbor test 2");
}

void test3() {
    uint8_t result[64];
    char result_hex[128];
    size_t written = 0;
    cborItem cborTag;

    uint8_t value[4] = { 0x01, 0x02, 0x03, 0x04};
    long tag = 23;
    cbor_create_tag(&cborTag, value, 4, tag);

    cbor_encode(result, &written, &cborTag);
    int8_to_char(result, written, result_hex);
    check_print("d74401020304", result_hex, "cbor test 1");
}

void test4() {
    uint8_t result[64];
    char result_hex[128];
    size_t written = 0;
    cborItem cborInteger1;
    cbor_create_integer(&cborInteger1, 1);
    cborItem cborInteger2;
    cbor_create_integer(&cborInteger2, 2);
    cborItem cborInteger3;
    cbor_create_integer(&cborInteger3, 3);
    cborItem cborArray;
    cbor_create_array(&cborArray);
    cbor_add_item_to_array(&cborArray, &cborInteger1);
    cbor_add_item_to_array(&cborArray, &cborInteger2);
    cbor_add_item_to_array(&cborArray, &cborInteger3);

    written = 0;
    cbor_encode(result, &written, &cborArray);
    int8_to_char(result, written, result_hex);
    check_print("83010203", result_hex, "cbor test 1");
}

void test5() {
    uint8_t result[64];
    char result_hex[128];
    size_t written = 0;

    cborItem cborParentArr;
    cbor_create_array(&cborParentArr);

    cborItem cborParentInt;
    cbor_create_integer(&cborParentInt, 1);


    cborItem cborChildArr1;
    cbor_create_array(&cborChildArr1);
    cborItem cborChild1Int1;
    cborItem cborChild1Int2;
    cbor_create_integer(&cborChild1Int1, 2);
    cbor_create_integer(&cborChild1Int2, 3);
    cbor_add_item_to_array(&cborChildArr1, &cborChild1Int1);
    cbor_add_item_to_array(&cborChildArr1, &cborChild1Int2);


    cborItem cborChildArr2;
    cbor_create_array(&cborChildArr2);
    cborItem cborChild2Int1;
    cborItem cborChild2Int2;
    cbor_create_integer(&cborChild2Int1, 4);
    cbor_create_integer(&cborChild2Int2, 5);
    cbor_add_item_to_array(&cborChildArr2, &cborChild2Int1);
    cbor_add_item_to_array(&cborChildArr2, &cborChild2Int2);


    cbor_add_item_to_array(&cborParentArr, &cborParentInt);
    cbor_add_item_to_array(&cborParentArr, &cborChildArr1);
    cbor_add_item_to_array(&cborParentArr, &cborChildArr2);

    cbor_encode(result, &written, &cborParentArr);
    int8_to_char(result, written, result_hex);
    check_print("8301820203820405", result_hex, "cbor test 6");
}


int main() {
    test1(); // encoding integer
    test2(); // encoding bytes
    test3(); // encoding tag
    test4(); // encoding array
    test5(); // encoding array
    return 0;
}
