#include "../easytlv.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h> // 添加这一行来声明system函数

#define etlv_printf printf
#define etlv_print_hex print_hex


// Example TLV data, encoding two 32 bit numbers:
const uint8_t testDataShort[] = {
    0x02, 0x04, 0x00, 0x00, 0x00, 0x2A, // Number 42
    0x02, 0x04, 0x00, 0x00, 0x01, 0x01, // Number 257
};

const uint8_t testDataLong[] = {
    0x1F, 0x88, 0x01,	// Extended tag
    0x82, 0x01, 0x01, 	// Extended length: 257 bytes
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
    0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
    0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
    0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
    0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
    0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
    0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF,
    0x01,
    0x02,		// Short tag
    0x04,		// Short length: 4 bytes
    0x00, 0x00, 0x01, 0x01,
};
/*
定义了两组测试数据testDataShort和testDataLong，分别代表简短和较长的TLV数据结构示例。
*/

void print_hex(const void* src, int len)    //print_hex函数用于打印缓冲区内容的十六进制表示，便于调试和查看数据。
{
    if(!src || len < 0)
        return;

    const uint8_t* s = src;
    printf("(%i) ", len);
    while(len-- > 0)
        printf("%02x", *s++);
}

static inline int is_big_endian()
{
    int i=1;    //(char *)&i 将这个地址强制转换为字符指针（char*），意味着现在可以按字节访问i的内存内容。
    return ! *((char *)&i); //&i 获取变量i的地址，即其在内存中的起始位置。
}

/*
这段代码定义了一个名为is_big_endian的静态内联函数，用于检测系统是大端字节序还是小端字节序。下面是逐步解释：

static inline int is_big_endian()：声明一个静态内联函数，返回整型值。静态表示该函数作用范围限制在定义它的文件内，内联则是建议编译器直接将函数体插入到调用处以提高运行效率。
int i=1;：声明并初始化一个整型变量i，赋值为1。在大多数系统中，整数占用多个字节存储，这里以4字节的整数为例，其二进制表示为 00000000 00000000 00000000 00000001（假设为32位系统）。
return ! *((char *)&i);：这行代码是检测字节序的关键。
首先，&i 获取变量i的地址，即其在内存中的起始位置。
然后，(char *)&i 将这个地址强制转换为字符指针（char*），意味着现在可以按字节访问i的内存内容。在大端字节序系统中，第一个字节（最低有效字节）存储的是00000001，而在小端字节序系统中，第一个字节存储的是00000000。
*((char *)&i) 进行解引用，获取该地址指向的字节值（即第一个字节的内容）。
! 是逻辑取反操作符。如果第一个字节为0，则!0的结果为1（真），表示是小端字节序；如果第一个字节为非0（在这里是1），则!1的结果为0（假），表示是大端字节序。
综上所述，这个函数的作用是判断当前系统的字节序。如果返回值为0，说明系统是大端字节序；如果返回值为1，说明系统是小端字节序。
*/

static inline uint32_t ntohl(uint32_t num)
{
    if (is_big_endian()) {  //如果返回值为0，说明系统是大端字节序；如果返回值为1，说明系统是小端字节序。
        return num;
    } else {    
        return  ((num>>24)&0xff) |      // move byte 3 to byte 0
                ((num<<8)&0xff0000) |   // move byte 1 to byte 2
                ((num>>8)&0xff00) |     // move byte 2 to byte 1
                ((num<<24)&0xff000000); // byte 0 to byte 3
    }
}

/*
这个函数名为 `ntohl`，它是 "network to host long" 的缩写，意为将网络字节序（大端字节序）的32位无符号整数转换为主机字节序。这里的主机字节序可能是大端也可能是小端，具体取决于系统的实现。

函数参数和返回类型：
- `static inline uint32_t ntohl(uint32_t num)`：声明了一个静态内联函数，接受一个32位无符号整数 `num` 作为参数，并返回一个32位无符号整数。`uint32_t` 是一种无符号整型，通常用于确保跨平台的兼容性。

函数逻辑：
1. **判断字节序**：首先调用之前提到的 `is_big_endian()` 函数来判断当前系统是否为大端字节序。
   - 如果系统本身就是大端字节序，那么网络字节序与主机字节序相同，无需转换，直接 `return num;`。
   - 如果系统是小端字节序，则需要进行字节序转换。

2. **字节序转换**：当需要转换时，通过位移操作和按位或运算来进行字节的重新排列。
   - `((num>>24)&0xff)` 将原数字的第4个字节（最高字节）移至最低字节位置。
   - `((num<<8)&0xff0000)` 将原数字的第2个字节移至第3个字节的位置。
   - `((num>>8)&0xff00)` 将原数字的第3个字节移至第2个字节的位置。
   - `((num<<24)&0xff000000)` 将原数字的第1个字节移至第4个字节的位置。
   - 最后，通过按位或运算 (`|`) 将这四个部分合并成一个新的32位整数，完成字节序的转换。

总结来说，这个 `ntohl` 函数的作用是根据当前系统的字节序，将传入的32位无符号整数从网络字节序转换为主机字节序，以便于主机正确解读网络数据。
*/
#define htonl ntohl

int main()
{
    uint8_t tlvRaw[sizeof(testDataLong)];
    uint32_t numChk;

    printf("\n\n\r-------------------- EasyTLV Test --------------------\n");

    // ---- TEST ON SHORT DATA ----
    printf("Parse test (SHORT DATA)\n\r");

    // Parse the two tokens
    ETLVToken t[2];
    int nTok = sizeof(t)/sizeof(t[0]);
    int err = etlv_parse(t, &nTok, testDataShort, sizeof(testDataShort));   //解析测试：使用etlv_parse函数解析两个预定义的TLV数据集，验证解析结果正确性。
    printf(" - result: %i\n\r", err);
    assert(err >= 0);                                   //断言（assert）来在测试失败时立即停止执行并报告错误，这是一种常用的调试手段，确保代码逻辑符合预期。

    // Cast the numbers to uint32_t's and compare
    numChk = ntohl(*(uint32_t *)t[0].val);
    assert(numChk == 42);
    numChk = ntohl(*(uint32_t *)t[1].val);
    assert(numChk == 257);
    printf(" - TEST PASS\n\r");

    printf("Serialization test (SHORT DATA)\n\r");
    printf(" - re-serializing same data as before\n\r");

    // Re-serialize the parsed data and compare to original
    int tlvBufSz = sizeof(tlvRaw);
    err = etlv_serialize(tlvRaw, &tlvBufSz, t, nTok);   //序列化测试：将解析后的数据重新序列化回原始格式，使用etlv_serialize函数，并通过比较序列化前后的数据来验证序列化过程的准确性。
    printf(" - result: %i\n\r", err);
    assert(err >= 0);
    assert(0 == memcmp(tlvRaw, testDataShort, tlvBufSz));
    printf(" - TEST PASS\n\r");

    // ---- TEST ON LONG DATA ----
    printf("Parse test (LONG DATA)\n\r");

    // Parse the two tokens
    nTok = sizeof(t)/sizeof(t[0]);
    err = etlv_parse(t, &nTok, testDataLong, sizeof(testDataLong));
    printf(" - result: %i\n\r", err);
    assert(err >= 0);

    // Check the parsed tokens
    assert(t[0].tag == 0x001F8801);
    assert(t[0].len == 257);
    assert(t[0].val == testDataLong + 3 + 3);
    numChk = ntohl(*(uint32_t *)t[1].val);
    assert(numChk == 257);
    printf(" - TEST PASS\n\r");

    printf("Serialization test (LONG DATA)\n\r");
    printf(" - re-serializing same data as before\n\r");

    // Re-serialize the parsed data and compare to original
    tlvBufSz = sizeof(tlvRaw);
    err = etlv_serialize(tlvRaw, &tlvBufSz, t, nTok);
    printf(" - result: %i\n\r", err);
    assert(err >= 0);
    assert(0 == memcmp(tlvRaw, testDataLong, tlvBufSz));
    printf(" - TEST PASS\n\r");

    // Search for both tags
    printf("Tag search test (LONG DATA -- first tag)\n\r");
    ETLVToken needle;
    err = etlv_find(&needle, 0x001F8801, testDataLong, sizeof(testDataLong));   //标签搜索测试：利用etlv_find函数在长数据集中搜索特定标签，并验证查找结果的正确性。
    printf(" - result: %i\n\r", err);
    assert(err == 0);
    assert(0 == memcmp(&t[0], &needle, sizeof(needle)));
    printf(" - TEST PASS\n\r");
    printf("Tag search test (LONG DATA -- second tag)\n\r");
    err = etlv_find(&needle, 0x02, testDataLong, sizeof(testDataLong));
    printf(" - result: %i\n\r", err);
    assert(err == 263);
    assert(0 == memcmp(&t[1], &needle, sizeof(needle)));
    printf(" - TEST PASS\n\r");
    system("pause");    // 防止运行后自动退出，需头文件stdlib.h
}

