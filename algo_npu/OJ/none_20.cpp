#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// 使用 long double 来提高斜率计算的精度
using ldouble = long double;

// 线段树节点存储最大斜率
vector<ldouble> tree;
// 存储每栋楼的高度
vector<long long> heights;
// 存储每栋楼的斜率
vector<ldouble> slopes;
int n_buildings;

// 构建线段树
void build(int node, int start, int end) {
    if (start == end) {
        tree[node] = slopes[start];
    } else {
        int mid = (start + end) / 2;
        build(2 * node, start, mid);
        build(2 * node + 1, mid + 1, end);
        tree[node] = max(tree[2 * node], tree[2 * node + 1]);
    }
}

// 更新线段树
void update(int node, int start, int end, int idx, ldouble val) {
    if (start == end) {
        slopes[idx] = val;
        tree[node] = val;
    } else {
        int mid = (start + end) / 2;
        if (start <= idx && idx <= mid) {
            update(2 * node, start, mid, idx, val);
        } else {
            update(2 * node + 1, mid + 1, end, idx, val);
        }
        tree[node] = max(tree[2 * node], tree[2 * node + 1]);
    }
}

// 查询 [l, r] 区间的最大斜率
ldouble query_max_slope(int node, int start, int end, int l, int r) {
    if (r < start || end < l || l > r) {
        return 0.0;
    }
    if (l <= start && end <= r) {
        return tree[node];
    }
    int mid = (start + end) / 2;
    ldouble p1 = query_max_slope(2 * node, start, mid, l, r);
    ldouble p2 = query_max_slope(2 * node + 1, mid + 1, end, l, r);
    return max(p1, p2);
}

// 递归计算可见楼数
int count_visible(int node, int start, int end, ldouble max_slope_from_left) {
    // 剪枝: 如果当前区间的最大斜率都不足以超过左侧的最大斜率，则该区间内没有可见楼
    if (tree[node] <= max_slope_from_left) {
        return 0;
    }
    // 基本情况: 叶子节点
    if (start == end) {
        return slopes[start] > max_slope_from_left ? 1 : 0;
    }

    int mid = (start + end) / 2;

    // 如果左子树的最大斜率都不足以超过左侧最大斜率，则左子树没有可见楼，直接搜索右子树
    if (tree[2 * node] <= max_slope_from_left) {
        return count_visible(2 * node + 1, mid + 1, end, max_slope_from_left);
    }

    // 否则，左子树中可能有可见楼
    int visible_count = count_visible(2 * node, start, mid, max_slope_from_left);

    // 用左子树的最大斜率更新 max_slope_from_left
    ldouble left_max_slope = tree[2 * node];
    ldouble new_max_slope = max(max_slope_from_left, left_max_slope);

    // 递归计算右子树的可见楼数
    visible_count += count_visible(2 * node + 1, mid + 1, end, new_max_slope);

    return visible_count;
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int m_days;
    cin >> n_buildings >> m_days;

    // 初始化，所有楼高度为0，索引从1到N
    heights.resize(n_buildings + 1, 0);
    slopes.resize(n_buildings + 1, 0.0);
    tree.resize(4 * (n_buildings + 1), 0.0);

    for (int i = 0; i < m_days; ++i) {
        int x;
        long long y;
        cin >> x >> y;

        // 更新高度和斜率
        heights[x] = y;
        ldouble new_slope = (x == 0) ? 0.0 : static_cast<ldouble>(y) / x;

        // 在线段树中更新
        update(1, 1, n_buildings, x, new_slope);

        // 计算并输出可见楼数
        cout << count_visible(1, 1, n_buildings, 0.0) << "\n";
    }

    return 0;
}