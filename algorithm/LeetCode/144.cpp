//
// Created by 86180 on 25-9-9.
//
#include <iostream>
#include <vector>

// TreeNode 结构体定义
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// 你需要补全的 leafSimilar 函数
bool leafSimilar(TreeNode* root1, TreeNode* root2) {

}

// 辅助函数：创建二叉树
TreeNode* createTree(const std::vector<int>& values, int index) {
    if (index >= values.size() || values[index] == -1) {
        return nullptr;
    }
    TreeNode* node = new TreeNode(values[index]);
    node->left = createTree(values, 2 * index + 1);
    node->right = createTree(values, 2 * index + 2);
    return node;
}

// main 函数
int main() {
    // 示例 1
    std::vector<int> tree1_values = {3, 5, 1, 6, 2, 9, 8, -1, -1, 7, 4};
    std::vector<int> tree2_values = {3, 5, 1, 6, 7, 4, 2, -1, -1, -1, -1, -1, -1, 9, 8};

    TreeNode* root1 = createTree(tree1_values, 0);
    TreeNode* root2 = createTree(tree2_values, 0);

    bool result1 = leafSimilar(root1, root2);
    std::cout << "示例 1 结果: " << (result1 ? "true" : "false") << std::endl;

    // 示例 2
    std::vector<int> tree3_values = {1, 2, 3};
    std::vector<int> tree4_values = {1, 3, 2};

    TreeNode* root3 = createTree(tree3_values, 0);
    TreeNode* root4 = createTree(tree4_values, 0);

    bool result2 = leafSimilar(root3, root4);
    std::cout << "示例 2 结果: " << (result2 ? "true" : "false") << std::endl;

    return 0;
}