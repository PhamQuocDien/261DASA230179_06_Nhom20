#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <vector>
#include "../models/Book.h"

class AVLTree
{
private:
    struct Node
    {
        Book data;
        Node* left;
        Node* right;
        int height;

        explicit Node(const Book& book);
    };

    Node* root;

    int getHeight(Node* node) const;
    int getBalance(Node* node) const;
    void updateHeight(Node* node);

    bool smaller(const Book& a, const Book& b) const;

    Node* rotateLeft(Node* node);
    Node* rotateRight(Node* node);

    Node* insert(Node* node, const Book& book);

    void inorder(Node* node) const;

    void findBookByYearRange(
        Node* node,
        int start,
        int end,
        std::vector<Book*>& result
    ) const;

    void deleteTree(Node* node);

public:
    AVLTree();
    ~AVLTree();

    void insert(const Book& book);
    void inorder() const;

    std::vector<Book*> findBookByYearRange(
        int start,
        int end
    ) const;

    bool empty() const;
};

#endif
