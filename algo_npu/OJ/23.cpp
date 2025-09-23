//
// Created by 86180 on 25-9-19.
//
#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct BigInt {
    static const int BASE = 100000000;
    static const int WIDTH = 8;
    vector<int> s;

    BigInt(long long num = 0) { *this = num; }
    BigInt& operator=(long long num) {
        s.clear();
        do {
            s.push_back(num % BASE);
            num /= BASE;
        } while (num > 0);
        return *this;
    }

    BigInt operator*(int b) const {
        BigInt c;
        c.s.clear();
        long long carry = 0;
        for (int i = 0; i < s.size(); ++i) {
            carry += 1LL * s[i] * b;
            c.s.push_back(carry % BASE);
            carry /= BASE;
        }
        while (carry > 0) {
            c.s.push_back(carry % BASE);
            carry /= BASE;
        }
        return c;
    }

    BigInt operator-(int b) const {
        BigInt c = *this;
        c.s[0] -= b;
        for (int i = 0; i < c.s.size(); ++i) {
            if (c.s[i] < 0) {
                c.s[i] += BASE;
                c.s[i+1]--;
            }
        }
        while (c.s.size() > 1 && c.s.back() == 0) c.s.pop_back();
        return c;
    }

    BigInt operator*(const BigInt &b) const {
        BigInt c;
        c.s.assign(s.size() + b.s.size(), 0);
        for (int i = 0; i < s.size(); ++i)
            for (int j = 0; j < b.s.size(); ++j) {
                long long mul = (long long)s[i] * b.s[j];
                c.s[i+j] += mul;
            }
        for (int i = 0; i < c.s.size()-1; ++i) {
            c.s[i+1] += c.s[i] / BASE;
            c.s[i] %= BASE;
        }
        while (c.s.size() > 1 && c.s.back() == 0) c.s.pop_back();
        return c;
    }

    BigInt operator+(const BigInt &b) const {
        BigInt c;
        c.s.clear();
        int n = max(s.size(), b.s.size()), carry = 0;
        for (int i = 0; i < n || carry; ++i) {
            int x = carry;
            if (i < s.size()) x += s[i];
            if (i < b.s.size()) x += b.s[i];
            c.s.push_back(x % BASE);
            carry = x / BASE;
        }
        return c;
    }

    void print() const {
        printf("%d", s.back());
        for (int i = s.size()-2; i >= 0; --i)
            printf("%08d", s[i]);
        printf("\n");
    }
};

BigInt pow2(int m) {
    BigInt x = 1;
    for (int i = 0; i < m; ++i) {
        x = x * 2;
    }
    return x;
}

int main() {
    int m;
    cin >> m;
    BigInt ans = pow2(m);
    ans = ans - 1;
    ans = ans * 2;
    ans.print();
    return 0;
}