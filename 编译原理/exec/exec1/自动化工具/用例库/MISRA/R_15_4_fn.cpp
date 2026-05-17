// MISRA R15.4 FN - Hidden second termination via macro expansion
extern void consume(int);
#define GOTO_OUT_IF_NEGATIVE(x) do { if ((x) < 0) { goto out; } } while(0)
void test_fn_macro_hidden_goto(void) {
    int arr[10] = {1, 2, -1, 4, -5, 6, 7, 8, 9, 10};
    int result = 0;
    for (int i = 0; i < 10; i++) {
        if (arr[i] > 5) {
            break;
        }
        GOTO_OUT_IF_NEGATIVE(arr[i]);
        result += arr[i];
    }
out:
    consume(result);
}
