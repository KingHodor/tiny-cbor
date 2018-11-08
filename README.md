# CBOR Encoding C library

The Concise Binary Object Representation (CBOR) is a data format whose design goals include the possibility of extremely small code size, fairly small message size, and extensibility without the need for version negotiation

Read more in the [documentation on ReadTheDocs](http://cbor.io/). 

# Examples
## encode integer 
```c
    uint8_t result[64];
    size_t written;
   
    cborItem cborInteger;
    cbor_create_integer(&cborInteger, 1000000);
    cbor_encode(result, &written, &cborInteger);
```
## encode bytes 
```c
    uint8_t result[64];
    size_t written = 0;
    
    cborItem cborBytes;
    uint8_t value[4] = { 0x01, 0x02, 0x03, 0x04};
    cbor_create_bytes(&cborBytes, value, 4);
    cbor_encode(result, &written, &cborBytes);
```
## encode tag 
```c
    uint8_t result[64];
    size_t written = 0;
    
    cborItem cborTag;
    uint8_t value[4] = { 0x01, 0x02, 0x03, 0x04};
    long tag = 23;
    cbor_create_tag(&cborTag, value, 4, tag);
    cbor_encode(result, &written, &cborTag);
```
## encode array 
```c
    uint8_t result[64];
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
```

## Getting help

Please contact ahmet@evercoin.com
