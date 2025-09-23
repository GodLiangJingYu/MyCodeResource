//
// Created by 86180 on 25-9-21.
//
#include <iostream>
#include <string>
using namespace std;

int parse(const string& s, int l, int r) {

}

int main() {
    string expr;
    getline(cin, expr);
    cout << parse(expr, 0, expr.size()) << endl;
    return 0;
}