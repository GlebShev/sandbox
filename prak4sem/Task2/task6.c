#include <stdio.h>
#include <stdlib.h>

// структура для узла бинарного дерева поиска
typedef struct Node {
    int key;
    struct Node* left;
    struct Node* right;
} Node;

// создание нового узла
Node* create_node(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// вставка нового элемента в дерево
Node* insert(Node* root, int key) {
    if (root == NULL) {
        return create_node(key);
    }

    if (key < root->key) {
        root->left = insert(root->left, key);
    } else if (key > root->key) {
        root->right = insert(root->right, key);
    }

    return root;
}

// поиск минимального значения в дереве (для удаления)
Node* find_min(Node* root) {
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

// удаление элемента из дерева
Node* delete(Node* root, int key) {
    if (root == NULL) {
        return root;
    }

    if (key < root->key) {
        root->left = delete(root->left, key);
    } else if (key > root->key) {
        root->right = delete(root->right, key);
    } else {
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }

        Node* temp = find_min(root->right);
        root->key = temp->key;
        root->right = delete(root->right, temp->key);
    }

    return root;
}

// поиск элемента в дереве
int search(Node* root, int key) {
    if (root == NULL) {
        return 0;
    }

    if (key == root->key) {
        return 1; 
    } else if (key < root->key) {
        return search(root->left, key);
    } else {
        return search(root->right, key);
    }
}

int main() {
    Node* root = NULL;
    char operation;
    int value;

    while (scanf(" %c%d", &operation, &value) != EOF) {
        if (operation == '+') {
            if (!search(root, value)) {
                root = insert(root, value);
            }
        } else if (operation == '-') {
            if (search(root, value)) {
                root = delete(root, value);
            }
        } else if (operation == '?') {
            if (search(root, value)) {
                printf("%d yes\n", value);
            } else {
                printf("%d no\n", value);
            }
        }
    }

}

