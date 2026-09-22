#include "AVLTree.h"

#include <algorithm>
#include <iostream>

using namespace std;

AVLTree::Node::Node(const Book& book)
    : data(book),
      left(nullptr),
      right(nullptr),
      height(1)
{
}

AVLTree::AVLTree()
    : root(nullptr)
{
}

AVLTree::~AVLTree()
{
    deleteTree(root);
    root = nullptr;
}

int AVLTree::getHeight(Node* node) const
{
    if (node == nullptr)
    {
        return 0;
    }

    return node->height;
}

void AVLTree::updateHeight(Node* node)
{
    if (node == nullptr)
    {
        return;
    }

    node->height = 1 + max(
        getHeight(node->left),
        getHeight(node->right)
    );
}

int AVLTree::getBalance(Node* node) const
{
    if (node == nullptr)
    {
        return 0;
    }

    return getHeight(node->left) - getHeight(node->right);
}

bool AVLTree::smaller(const Book& a, const Book& b) const
{
    if (a.year != b.year)
    {
        return a.year < b.year;
    }

    return a.bookCode < b.bookCode;
}

AVLTree::Node* AVLTree::rotateLeft(Node* node)
{
    Node* newRoot = node->right;
    Node* middle = newRoot->left;

    newRoot->left = node;
    node->right = middle;

    updateHeight(node);
    updateHeight(newRoot);

    return newRoot;
}

AVLTree::Node* AVLTree::rotateRight(Node* node)
{
    Node* newRoot = node->left;
    Node* middle = newRoot->right;

    newRoot->right = node;
    node->left = middle;

    updateHeight(node);
    updateHeight(newRoot);

    return newRoot;
}

AVLTree::Node* AVLTree::insert(
    Node* node,
    const Book& book
)
{
    if (node == nullptr)
    {
        return new Node(book);
    }

    if (smaller(book, node->data))
    {
        node->left = insert(node->left, book);
    }
    else
    {
        node->right = insert(node->right, book);
    }

    updateHeight(node);

    int balance = getBalance(node);

    // LL
    if (balance > 1 && smaller(book, node->left->data))
    {
        return rotateRight(node);
    }

    // RR
    if (balance < -1 && !smaller(book, node->right->data))
    {
        return rotateLeft(node);
    }

    // LR
    if (balance > 1 && !smaller(book, node->left->data))
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // RL
    if (balance < -1 && smaller(book, node->right->data))
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

void AVLTree::insert(const Book& book)
{
    root = insert(root, book);
}

void AVLTree::inorder(Node* node) const
{
    if (node == nullptr)
    {
        return;
    }

    inorder(node->left);

    cout << node->data.bookCode
         << " - "
         << node->data.title
         << " - "
         << node->data.author
         << " - "
         << node->data.year
         << endl;

    inorder(node->right);
}

void AVLTree::inorder() const
{
    inorder(root);
}

void AVLTree::findBookByYearRange(
    Node* node,
    int start,
    int end,
    vector<Book*>& result
) const
{
    if (node == nullptr)
    {
        return;
    }

    // Chi di sang trai khi con kha nang co year >= start.
    if (node->data.year >= start)
    {
        findBookByYearRange(
            node->left,
            start,
            end,
            result
        );
    }

    if (node->data.year >= start &&
        node->data.year <= end)
    {
        result.push_back(&node->data);
    }

    // Chi di sang phai khi con kha nang co year <= end.
    if (node->data.year <= end)
    {
        findBookByYearRange(
            node->right,
            start,
            end,
            result
        );
    }
}

vector<Book*> AVLTree::findBookByYearRange(
    int start,
    int end
) const
{
    vector<Book*> result;

    if (start > end)
    {
        return result;
    }

    findBookByYearRange(
        root,
        start,
        end,
        result
    );

    return result;
}

void AVLTree::deleteTree(Node* node)
{
    if (node == nullptr)
    {
        return;
    }

    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

bool AVLTree::empty() const
{
    return root == nullptr;
}
