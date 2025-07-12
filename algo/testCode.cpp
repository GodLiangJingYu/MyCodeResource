#include <iostream>
#include <vector>
#include <queue>
using namespace std;

// 定义二叉树节点结构
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// 递归生成完美二叉树
TreeNode* generatePerfectBinaryTree(int depth, int& value) {
    if (depth == 0) {
        return nullptr;
    }
    TreeNode* node = new TreeNode(value++);
    cout<< "Creating node with value: " << node->val << endl;
    node->left = generatePerfectBinaryTree(depth - 1, value);
    node->right = generatePerfectBinaryTree(depth - 1, value);
    return node;
}

vector<int> levelOrder(TreeNode* root) {
    vector<int> vec;
    queue<TreeNode*> queue;
    queue.push(root);
    while (!queue.empty()) {
        TreeNode* head = queue.front();
        queue.pop();
        vec.push_back(head->val);
        if (head->left != nullptr) {
            queue.push(head->left);
        }
        if (head->right != nullptr) {
            queue.push(head->right);
        }
    }
    return vec;
}

/* 前序遍历 */
void preOrder(TreeNode *root,vector<int> &vec) {
    if (root == nullptr) {
        return;
    }
    vec.push_back(root->val);
    if (root->left != nullptr) {
        preOrder(root->left, vec);
    }
    if (root->right != nullptr) {
        preOrder(root->right,vec);
    }
}

/* 中序遍历 */
void inOrder(TreeNode *root,vector<int> &vec) {
    if (root == nullptr) {
        return;
    }
    if (root->left != nullptr) {
        inOrder(root->left, vec);
    }
    vec.push_back(root->val);
    if (root->right != nullptr) {
        inOrder(root->right,vec);
    }
}

int main() {
    int depth = 3; // 设置树的深度
    int value = 1; // 节点值从1开始
    TreeNode* root = generatePerfectBinaryTree(depth, value);

    vector<int> vec;
    inOrder(root,vec);
    for (auto num : vec) {
        cout<< num << " ";
    }
    return 0;
}