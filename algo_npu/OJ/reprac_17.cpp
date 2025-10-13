//
// Created by 86180 on 25-9-18.
//
#include <iostream>
#include <stack>
using namespace std;

string outputS(string s) {
    string res;
    stack<int> stack; // 保留你原来的变量名
    for (int i = 0; i < (int)s.size(); ++i) {
        if (s[i] == '[') {
            stack.push(i);
        } else if (s[i] == ']') {
            if (stack.empty()) continue; // 格式合法时不应发生
            int l = stack.top();
            stack.pop();

            // 提取方括号内的原始内容
            string inner = s.substr(l + 1, i - l - 1);

            // 解析 inner 的前缀数字（若存在）作为重复次数
            int idx = 0;
            while (idx < (int)inner.size() && inner[idx] >= '0' && inner[idx] <= '9') ++idx;
            int repeat = 1;
            if (idx > 0) {
                repeat = stoi(inner.substr(0, idx));
            }
            // 方括号内真正要递归解码的部分（去掉前缀数字）
            string content = inner.substr(idx);

            // 递归解码 content（可能包含嵌套）
            string decoded = outputS(content);

            // 按 repeat 重复 decoded
            string rep;
            rep.reserve(decoded.size() * (size_t)repeat);
            for (int t = 0; t < repeat; ++t) rep += decoded;

            // 用 rep 替换原来的 "[ ... ]"（注意使用 l 和 i 的旧位置）
            s.erase(l, i - l + 1);
            s.insert(l, rep);

            // 调整 i 到插入段末尾（下一次循环从该位置的下一个字符继续）
            i = l + (int)rep.size() - 1;
        }
    }
    return s;
}

int main() {
    string s;
    if (!(cin >> s)) return 0;
    string res = outputS(s);
    cout << res;
    return 0;
}
