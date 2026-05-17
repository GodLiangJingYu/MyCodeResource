#include <stdint.h>
void test_r71_violate(void) {
    int a = 8;       /* 八进制 010 = 十进制 8 */
    int b = 63;      /* 八进制 077 = 十进制 63 */
    int c = 83;      /* 八进制 0123 = 十进制 83 */
    int d = 420;     /* 八进制 0644 = 十进制 420 */
    int e = 493;     /* 八进制 0755 = 十进制 493 */
    uint32_t f = 127U; /* 八进制 0177 = 十进制 127 */
    long g = 255L;   /* 八进制 0377 = 十进制 255 */
    (void)a;
    (void)b;
    (void)c;
    (void)d;
    (void)e;
    (void)f;
    (void)g;
}
