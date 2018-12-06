#include <stdio.h>
#include <stdint.h>
#include "cbor.h"
#include "utils.h"


void test1() {
    uint8_t result[64];
    char result_hex[128];
    size_t written = 0;

    cborItem cborInteger;
    cbor_create_integer(&cborInteger, 1000000000000L);
    cbor_encode(result, &written, &cborInteger);
    int8_to_char(result, written, result_hex);
    check_print("1b000000e8d4a51000", result_hex, "cbor test 1a");

    written = 0;
    cborItem cborInteger2;
    cbor_create_integer(&cborInteger2, 1000000L);
    cbor_encode(result, &written, &cborInteger2);
    int8_to_char(result, written, result_hex);
    check_print("1a000f4240", result_hex, "cbor test 1b");

    written = 0;
    cborItem cborInteger3;
    cbor_create_integer(&cborInteger3, 1000L);
    cbor_encode(result, &written, &cborInteger3);
    int8_to_char(result, written, result_hex);
    check_print("1903e8", result_hex, "cbor test 1c");

    written = 0;
    cborItem cborInteger4;
    cbor_create_integer(&cborInteger4, 100);
    cbor_encode(result, &written, &cborInteger4);
    int8_to_char(result, written, result_hex);
    check_print("1864", result_hex, "cbor test 1d");

    written = 0;
    cborItem cborInteger5;
    cbor_create_integer(&cborInteger5, 24);
    cbor_encode(result, &written, &cborInteger5);
    int8_to_char(result, written, result_hex);
    check_print("1818", result_hex, "cbor test 1f");

    written = 0;
    cborItem cborInteger6;
    cbor_create_integer(&cborInteger6, 1);
    cbor_encode(result, &written, &cborInteger6);
    int8_to_char(result, written, result_hex);
    check_print("01", result_hex, "cbor test 1g");

    written = 0;
    cborItem cborInteger7;
    cbor_create_integer(&cborInteger7, 0);
    cbor_encode(result, &written, &cborInteger7);
    int8_to_char(result, written, result_hex);
    check_print("00", result_hex, "cbor test 1h");
}

void test2() {
    uint8_t result[64];
    char result_hex[128];
    size_t written = 0;
    cborItem cborBytes;

    uint8_t value[4] = {0x01, 0x02, 0x03, 0x04};
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

    uint8_t value[4] = {0x01, 0x02, 0x03, 0x04};
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

    cbor_encode(result, &written, &cborArray);
    int8_to_char(result, written, result_hex);
    check_print("83010203", result_hex, "cbor test 1");
}

void test5() {
    uint8_t result[64];
    char result_hex[128];
    size_t written = 0;
    cborItem cborMap;

    cbor_create_map(&cborMap);

    cbor_encode(result, &written, &cborMap);
    int8_to_char(result, written, result_hex);
    check_print("a0", result_hex, "cbor test 5");
}


void test6() {
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

void test7() {
    uint8_t result[64];
    char result_hex[128];
    size_t written = 0;

    cborItem cborInfiniteArray;

    cborItem cborChildInt1;
    cbor_create_integer(&cborChildInt1, 1);

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

    cbor_infinite_create_array(&cborInfiniteArray);
    cbor_add_item_to_array(&cborInfiniteArray, &cborChildInt1);
    cbor_add_item_to_array(&cborInfiniteArray, &cborChildArr1);
    cbor_add_item_to_array(&cborInfiniteArray, &cborChildArr2);

    cbor_encode(result, &written, &cborInfiniteArray);
    int8_to_char(result, written, result_hex);
    check_print("9f01820203820405ff", result_hex, "cbor test 7");
}

void test8() {
    uint8_t result[64];
    char result_hex[128];
    size_t written = 0;

    cborItem cborInfiniteArray;

    cborItem cborChildInt1;
    cbor_create_integer(&cborChildInt1, 1);

    cborItem cborChildArr1;
    cbor_create_array(&cborChildArr1);
    cborItem cborChild1Int1;
    cborItem cborChild1Int2;
    cbor_create_integer(&cborChild1Int1, 2);
    cbor_create_integer(&cborChild1Int2, 3);
    cbor_add_item_to_array(&cborChildArr1, &cborChild1Int1);
    cbor_add_item_to_array(&cborChildArr1, &cborChild1Int2);

    cborItem cborInfiniteArrayChild1;
    cbor_infinite_create_array(&cborInfiniteArrayChild1);
    cborItem cborChild2Int1;
    cborItem cborChild2Int2;
    cbor_create_integer(&cborChild2Int1, 4);
    cbor_create_integer(&cborChild2Int2, 5);
    cbor_add_item_to_array(&cborInfiniteArrayChild1, &cborChild2Int1);
    cbor_add_item_to_array(&cborInfiniteArrayChild1, &cborChild2Int2);

    cbor_infinite_create_array(&cborInfiniteArray);
    cbor_add_item_to_array(&cborInfiniteArray, &cborChildInt1);
    cbor_add_item_to_array(&cborInfiniteArray, &cborChildArr1);
    cbor_add_item_to_array(&cborInfiniteArray, &cborInfiniteArrayChild1);

    cbor_encode(result, &written, &cborInfiniteArray);
    int8_to_char(result, written, result_hex);
    check_print("9f018202039f0405ffff", result_hex, "cbor test 8");
}

int main() {
    test1(); // encoding integer
    test2(); // encoding bytes
    test3(); // encoding tag
    test4(); // encoding array
    test5(); // encoding map
    test6();
    test7(); // encoding IndefiniteLengthArray
    test8(); // encoding IndefiniteLengthArray
    return 0;
}
