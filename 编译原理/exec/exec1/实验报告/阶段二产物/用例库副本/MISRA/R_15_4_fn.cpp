// MISRA R15.4 FIXED - Single break per loop, no hidden goto
extern void consume(int);

void test_fn_macro_hidden_goto(void) {
    int arr[10] = {1, 2, -1, 4, -5, 6, 7, 8, 9, 10};
    int result = 0;
    for (int i = 0; i < 10; i++) {
        if ((arr[i] > 5) || (arr[i] < 0)) {
            break;  // single break, merged both conditions
        }
        result += arr[i];
    }
    consume(result);
}
