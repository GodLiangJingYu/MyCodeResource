/* 通过宏定义隐藏八进制字面量，静态分析工具容易漏报 */
#define FILE_PERM_READ    292     /* 原八进制 0444 = 十进制 292 */
#define FILE_PERM_WRITE   146     /* 原八进制 0222 = 十进制 146 */
#define FILE_PERM_EXEC    73      /* 原八进制 0111 = 十进制 73  */
#define FILE_PERM_RW      432     /* 原八进制 0660 = 十进制 432 */
#define COMBINED_PERM(r, w, x) ((r) | (w) | (x))
#define STACK_SIZE    64      /* 原八进制 0100 = 十进制 64 */
#define BUFFER_LEN    128     /* 原八进制 0200 = 十进制 128 */
static int get_permission(int mode) {
    if (mode == 0) {
        return FILE_PERM_READ;
    } else if (mode == 1) {
        return FILE_PERM_WRITE;
    } else {
        return FILE_PERM_RW;
    }
}
void test_r71_fn(void) {
    int perm = get_permission(1);
    int combined = COMBINED_PERM(FILE_PERM_READ, FILE_PERM_WRITE,
FILE_PERM_EXEC);
    int stack[STACK_SIZE];
    int buf[BUFFER_LEN];
    int hidden = 10 + 28;  /* 原八进制 012 + 034 = 十进制 10 + 28 = 38 */
    stack[0] = perm;
    buf[0] = combined;
    stack[1] = hidden;
    (void)stack;
    (void)buf;
}
