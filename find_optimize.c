#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define DATA_SIZE 200
#define HASH_SIZE 256

typedef struct HashNode {
    int key;
    struct HashNode* next;
} HashNode;

typedef struct {
    HashNode* buckets[HASH_SIZE];
} HashTable;

// 初始化哈希表
void init_hash_table(HashTable* hash_table) {
    for (int i = 0; i < HASH_SIZE; i++) {
        hash_table->buckets[i] = NULL;
    }
}

// 插入数据到哈希表
void insert_to_hash_table(HashTable* hash_table, int value) {
    int index = value % HASH_SIZE;
    HashNode* new_node = malloc(sizeof(HashNode));
    new_node->key = value;
    new_node->next = hash_table->buckets[index];
    hash_table->buckets[index] = new_node;
}

// 在哈希表中查找数据
bool search_in_hash_table(HashTable* hash_table, int value) {
    int index = value % HASH_SIZE;
    HashNode* current = hash_table->buckets[index];
    while (current) {
        if (current->key == value) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// 普通线性遍历
int linear_search(int arr[], int size, int target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            return i; // 返回目标值的索引
        }
    }
    return -1; // 未找到
}
///////
// AVL 树节点定义
typedef struct AVLNode {
    int key;
    int height;
    struct AVLNode* left;
    struct AVLNode* right;
} AVLNode;

// 获取节点高度
int get_height(AVLNode* node) {
    return node ? node->height : 0;
}

// 计算平衡因子
int get_balance_factor(AVLNode* node) {
    return node ? get_height(node->left) - get_height(node->right) : 0;
}

// 创建新节点
AVLNode* create_node(int key) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    node->key = key;
    node->height = 1; // 新节点的高度为 1
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 右旋
AVLNode* rotate_right(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T = x->right;

    // 旋转
    x->right = y;
    y->left = T;

    // 更新高度
    y->height = 1 + (get_height(y->left) > get_height(y->right) ? get_height(y->left) : get_height(y->right));
    x->height = 1 + (get_height(x->left) > get_height(x->right) ? get_height(x->left) : get_height(x->right));

    return x;
}

// 左旋
AVLNode* rotate_left(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T = y->left;

    // 旋转
    y->left = x;
    x->right = T;

    // 更新高度
    x->height = 1 + (get_height(x->left) > get_height(x->right) ? get_height(x->left) : get_height(x->right));
    y->height = 1 + (get_height(y->left) > get_height(y->right) ? get_height(y->left) : get_height(y->right));

    return y;
}

// 插入节点
AVLNode* insert(AVLNode* node, int key) {
    if (!node) {
        return create_node(key);
    }

    if (key < node->key) {
        node->left = insert(node->left, key);
    } else if (key > node->key) {
        node->right = insert(node->right, key);
    } else {
        return node; // 不允许插入重复值
    }

    // 更新高度
    node->height = 1 + (get_height(node->left) > get_height(node->right) ? get_height(node->left) : get_height(node->right));

    // 计算平衡因子
    int balance = get_balance_factor(node);

    // 平衡树
    if (balance > 1 && key < node->left->key) {
        return rotate_right(node); // 左左情况
    }
    if (balance < -1 && key > node->right->key) {
        return rotate_left(node); // 右右情况
    }
    if (balance > 1 && key > node->left->key) {
        node->left = rotate_left(node->left); // 左右情况
        return rotate_right(node);
    }
    if (balance < -1 && key < node->right->key) {
        node->right = rotate_right(node->right); // 右左情况
        return rotate_left(node);
    }

    return node;
}

// 查找节点
int search(AVLNode* root, int key) {
    if (!root) {
        return 0; // 未找到
    }
    if (key == root->key) {
        return 1; // 找到
    } else if (key < root->key) {
        return search(root->left, key);
    } else {
        return search(root->right, key);
    }
}

// 释放 AVL 树
void free_tree(AVLNode* root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}
///////

int main() {
    int data[DATA_SIZE];
    int target = 150; // 要查找的目标值
    HashTable hash_table;

    // 初始化随机数据
    srand(time(NULL));
    for (int i = 0; i < DATA_SIZE; i++) {
        data[i] = rand() % DATA_SIZE; // 随机生成整数
    }
    // 确保目标值存在
    data[rand() % DATA_SIZE] = target;

    // 测试普通线性遍历
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    int result = linear_search(data, DATA_SIZE, target);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double linear_time = (end.tv_sec - start.tv_sec) * 1e3 + (end.tv_nsec - start.tv_nsec) / 1e6;

    if (result != -1) {
        printf("Found target %d at index %d using linear search.\n", target, result);
    } else {
        printf("Target %d not found using linear search.\n", target);
    }
    printf("Linear search time: %.6f ms\n", linear_time);

    // 初始化哈希表
    init_hash_table(&hash_table);

    // 将数据插入哈希表
    for (int i = 0; i < DATA_SIZE; i++) {
        insert_to_hash_table(&hash_table, data[i]);
    }

    // 测试哈希表查找
    clock_gettime(CLOCK_MONOTONIC, &start);
    bool found = search_in_hash_table(&hash_table, target);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double hash_time = (end.tv_sec - start.tv_sec) * 1e3 + (end.tv_nsec - start.tv_nsec) / 1e6;

    if (found) {
        printf("Found target %d using hash table.\n", target);
    } else {
        printf("Target %d not found using hash table.\n", target);
    }
    printf("Hash table search time: %.6f ms\n", hash_time);

    // 测试 AVL 树查找
    AVLNode* root = NULL;
    // 构建 AVL 树
    for (int i = 0; i < DATA_SIZE; i++) {
        root = insert(root, data[i]);
    }

    // 测试 AVL 树查找
    clock_gettime(CLOCK_MONOTONIC, &start);
    int found_avl = search(root, target);
    clock_gettime(CLOCK_MONOTONIC, &end);

    if (found_avl) {
        printf("Found target %d in AVL tree.\n", target);
    } else {
        printf("Target %d not found in AVL tree.\n", target);
    }

    double elapsed_time = (end.tv_sec - start.tv_sec) * 1e3 + (end.tv_nsec - start.tv_nsec) / 1e6;
    printf("AVL tree search time: %.6f ms\n", elapsed_time);

    // 释放 AVL 树
    free_tree(root);
    return 0;
}