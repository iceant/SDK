#include <sdk_md5.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int hex_digit_to_int(char c) {
    switch (c) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return c - '0';
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
            return c + 10 - 'A';
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
            return c + 10 - 'a';
        default: assert(0 && "invalid hex character");
    }
    return -1;
}

void hex_string_to_bytes(char* hex, uint8_t bytes[16]) {
    assert(strlen(hex) == 32);
    int j = 0;
    for (int i = 0; i < 32; i+= 2) {
        bytes[j] = 16 * hex_digit_to_int(hex[i])
                   + hex_digit_to_int(hex[i+1]);
        j++;
    }
}

void print_bytes(uint8_t bytes[16]) {
    for (int i = 0; i < 16; i++) {
        printf("%02X", bytes[i]);
    }
    printf("\n");
}

void test(char* input, char* expected) {
    sdk_md5_ctx_t ctx;
    sdk_md5_init(&ctx);
    sdk_md5_update(&ctx, input, strlen(input));

    uint8_t md5_actual[16] = {0};
    sdk_md5_final(md5_actual, &ctx);

    uint8_t md5_expected[16] = {0};
    hex_string_to_bytes(expected, md5_expected);

    if (memcmp(md5_actual, md5_expected, 16) != 0) {
        printf("test:     %s\n", input);
        printf("actual:   "); print_bytes(md5_actual);
        printf("expected: "); print_bytes(md5_expected);
        assert(0 && "test cases failed");
    }
}

int main() {
    // test cases from RFC 1321
    test("", "d41d8cd98f00b204e9800998ecf8427e");
    test("a", "0cc175b9c0f1b6a831c399e269772661");
    test("abc", "900150983cd24fb0d6963f7d28e17f72");
    test("message digest", "f96b697d7cb7938d525a2f31aaf161d0");
    test("abcdefghijklmnopqrstuvwxyz", "c3fcd3d76192e4007dfb496cca67e13b");
    test("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
         "d174ab98d277d9f5a5611c2c9f419d9f");
    test("12345678901234567890123456789012345678901234567890123456789012345678901234567890",
         "57edf4a22be3c955ac49da2e2107b67a");

    // additional test cases
    test("a", "0cc175b9c0f1b6a831c399e269772661");
    test("ab", "187ef4436122d1cc2f40dc2b92f0eba0");
    test("abc", "900150983cd24fb0d6963f7d28e17f72");
    test("abcd", "e2fc714c4727ee9395f324cd2e7f331f");
    test("abcde", "ab56b4d92b40713acc5af89985d4b786");
    test("abcdef", "e80b5017098950fc58aad83c8c14978e");
    test("abcdefg", "7ac66c0f148de9519b8bd264312c4d64");
    test("abcdefgh", "e8dc4081b13434b45189a720b77b6818");
    test("abcdefghi", "8aa99b1f439ff71293e95357bac6fd94");
    test("abcdefghij", "a925576942e94b2ef57a066101b48876");
    test("abcdefghijk", "92b9cccc0b98c3a0b8d0df25a421c0e3");
    test("abcdefghijkl", "9fc9d606912030dca86582ed62595cf7");
    test("abcdefghijklm", "22aebdd14e72f6b379476a146347d546");
    test("abcdefghijklmn", "0845a5972cd9ad4a46bad66f1253581f");
    test("abcdefghijklmno", "8a7319dbf6544a7422c9e25452580ea5");
    test("abcdefghijklmnop", "1d64dce239c4437b7736041db089e1b9");
    test("abcdefghijklmnopq", "9a8d9845a6b4d82dfcb2c2e35162c830");
    test("abcdefghijklmnopqr", "fbaf6ecf6acbe54aa33858818d351197");
    test("abcdefghijklmnopqrs", "320620468e1c404151741bbbaff2ada5");
    test("abcdefghijklmnopqrst", "6aa8de45918023095f6e831efe48d00b");
    test("abcdefghijklmnopqrstu", "ce7e0645565441733f794b29a30afd05");
    test("abcdefghijklmnopqrstuv", "44a66044834cbe55040089cabfc102d5");
    test("abcdefghijklmnopqrstuvw", "74525c3a55bdcaee9139bd1e33c764fb");
    test("abcdefghijklmnopqrstuvwx", "0c0e84cf0bb7a8688e5238dbbc1c3953");
    test("abcdefghijklmnopqrstuvwxy", "f1784031a03a8f5b11ead16ab90cc18e");
    test("abcdefghijklmnopqrstuvwxyz", "c3fcd3d76192e4007dfb496cca67e13b");
    test("abcdefghijklmnopqrstuvwxyz0", "953f2c6ba8e5293075470b6982386c70");
    test("abcdefghijklmnopqrstuvwxyz01", "af3a8331ab2aa7e0e39ebd34316f3726");
    test("abcdefghijklmnopqrstuvwxyz012", "a754a04fb0a57f19ec918c285f93d594");
    test("abcdefghijklmnopqrstuvwxyz0123", "71da0db811d4c94847964ecef1e28f4e");
    test("abcdefghijklmnopqrstuvwxyz01234", "d51a934194d2ec472ef1d459037027fa");
    test("abcdefghijklmnopqrstuvwxyz012345", "357e82db934fc45f4a25b4b83dc8bd19");
    test("abcdefghijklmnopqrstuvwxyz0123456", "2e34e06618c6dc00a857b09cd22e3ab2");
    test("abcdefghijklmnopqrstuvwxyz01234567", "5a219f31a9f5539c8c3146ef1059cac3");
    test("abcdefghijklmnopqrstuvwxyz012345678", "8aec8672f465ae1503f90428479ace5a");
    test("abcdefghijklmnopqrstuvwxyz0123456789", "6d2286301265512f019781cc0ce7a39f");
    test("abcdefghijklmnopqrstuvwxyz0123456789a", "6900112698cde7b2a6f3febd2a13220c");
    test("abcdefghijklmnopqrstuvwxyz0123456789ab", "bffb54e8622bf911fb71e0d81fdef97e");
    test("abcdefghijklmnopqrstuvwxyz0123456789abc", "f7c79b5457c74a247051d1b976060f9f");
    test("abcdefghijklmnopqrstuvwxyz0123456789abcd", "fa74c9791663dfa4da17226804752bba");
    test("abcdefghijklmnopqrstuvwxyz0123456789abcde", "fe06d350409272a62ec465518e143d5e");
    test("abcdefghijklmnopqrstuvwxyz0123456789abcdef", "91a7fb99f3ea046f86de08322d149beb");
    test("abcdefghijklmnopqrstuvwxyz0123456789abcdefg", "8e9937ced1bf9c0e743e5ac69febc9e6");
    test("abcdefghijklmnopqrstuvwxyz0123456789abcdefgh", "96c6c3d71f9b47547a6e2ca759163b15");
    test("abcdefghijklmnopqrstuvwxyz0123456789abcdefghi", "c1163d9cb56290f62b5ca3d0f274c396");
    test("abcdefghijklmnopqrstuvwxyz0123456789abcdefghij", "576cbaf35532c05b256223e401345d4b");
    test("abcdefghijklmnopqrstuvwxyz0123456789abcdefghijk", "26e6d1cd9f1a9c556549a190a7aa4020");
    test("abcdefghijklmnopqrstuvwxyz0123456789abcdefghijkl", "c9e2f788260a5548fcb6b70cba8c56dd");
    test("abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklm", "acafc98134c87e63f802b7676de1df7c");
    test("abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmn", "c52b7685f01375182427c6c379e19578");
    test("abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmno", "1bb1758cd6859b739d3380bc7cea21bd");
    test("abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnop", "93161812269ced4eb77bc8119a77d095");
    test("abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopq", "f5090549ecd7350109a7a04bf15f2f9e");
    test("abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqr", "11f67500e60fdf623ec9c8ccf0dc0499");
    test("abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrs", "a49d85aaac8495cbb53b120f3b987478");
    test("abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrst", "0b74570ac5c5b441888f67619534aa88");
    test("abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstu", "c63717b1c4cb3f3db2e37d5052eccd2a");
    test("abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuv", "db33a3917d0fa52cd350f8ee2b7ac9d1");
    test("abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvw", "4afae235610295e426b4bd6dba1a13d5");
    test("abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwx", "f206a2c7855398982a305aedf776bb70");
    test("abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxy", "e58ba530cb66f461a57d8fb5061c229e");
    test("abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz", "d1caadf5c979bfcdbe1b3962ac6015d1");
    test("abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz0", "87d2cdc81ca700a7259acd6bc75abf0b");
    test("abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz01", "bbd17cbd1784152cd93cca62dee11b5b");

    printf("All tests passed!\n");
}
