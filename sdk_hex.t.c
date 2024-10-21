#include <sdk_hex.h>
#include <stdio.h>
#include <stdlib.h>
#include <sdk_macros.h>
#include <string.h>

#define BUFFER_SIZE 256
#define HEX_STRING_SIZE (2*BUFFER_SIZE+1)

static uint8_t buffer[BUFFER_SIZE];
static char hex_string[HEX_STRING_SIZE];

int main(int argc, char** argv){
    for(int i=0; i< SDK_ARRAY_SIZE(buffer); i++){
        buffer[i] = i;
    }

    sdk_hex_dump("buffer", buffer, SDK_ARRAY_SIZE(buffer));

    SDK_HEX_ENCODE_BE(hex_string, SDK_ARRAY_SIZE(hex_string), buffer, SDK_ARRAY_SIZE(buffer));
    sdk_hex_dump("hex_string BE", hex_string, SDK_ARRAY_SIZE(hex_string));

    SDK_HEX_ENCODE_LE(hex_string, SDK_ARRAY_SIZE(hex_string), buffer, SDK_ARRAY_SIZE(buffer));
    sdk_hex_dump("hex_string LE", hex_string, SDK_ARRAY_SIZE(hex_string));

    memset(buffer, 0, SDK_ARRAY_SIZE(buffer));
    sdk_hex_decode(buffer, 16, SDK_ARRAY_SIZE(buffer), hex_string, 0, SDK_ARRAY_SIZE(hex_string)-16);
    sdk_hex_dump("decode-buffer", buffer, SDK_ARRAY_SIZE(buffer));

    return 0;
}