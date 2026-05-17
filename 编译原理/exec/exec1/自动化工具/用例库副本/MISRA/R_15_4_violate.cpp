// MISRA R15.4 FIXED - Loop with at most one break statement
extern void consume(int);

void test_violate(void) {
    int arr[10] = {0};
    int found = 0;
    for (int i = 0; i < 10; i++) {
        if (arr[i] == 1) {
            found = 1;
        }
        if ((arr[i] == 1) || (i == 8)) {
            break;  // single break, merged two conditions
        }
    }
    consume(found);
}

void test_violate_goto(void) {
    int sum = 0;
    for (int i = 0; i < 10; i++) {
        if ((i == 3) || (i == 5)) {
            break;  // single break, merged two conditions
        }
        sum += i;
    }
    consume(sum);
}
