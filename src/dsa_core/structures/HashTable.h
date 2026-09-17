#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <functional>
template <typename Key, typename Value>
class HashTable {
private:
    struct Node {
        Key key;
        Value value;
        Node* next;
        Node(const Key& key, const Value& value) {
            this->key = key;
            this->value = value;
            this->next = nullptr;
        }
    };
    Node** buckets;
    int capacity;
    int count;
private:
    int hashFunction(const Key& key) const {
        return static_cast<int>(std::hash<Key>{}(key) % capacity);
    }
public:
    HashTable(int size = 10) {
        if (size <= 0) {
            size = 1;
        }
        capacity = size;
        count = 0;
        buckets = new Node * [capacity];
        for (int i = 0; i < capacity; ++i) {
            buckets[i] = nullptr;
        }
    }
    ~HashTable() {
        for (int i = 0; i < capacity; ++i) {
            Node* current = buckets[i];
            while (current != nullptr) {
                Node* next = current->next;
                delete current;
                current = next;
            }
        }
        delete[] buckets;
    }
    void insert(const Key& key, const Value& value) {
        int index = hashFunction(key);
        Node* current = buckets[index];
        while (current != nullptr) {
            if (current->key == key) {
                current->value = value;
                return;
            }
            current = current->next;
        }
        Node* newNode = new Node(key, value);
        newNode->next = buckets[index];
        buckets[index] = newNode;
        ++count;
    }
    Value* find(const Key& key) {
        int index = hashFunction(key);
        Node* current = buckets[index];
        while (current != nullptr) {
            if (current->key == key) {
                return &current->value;
            }
            current = current->next;
        }
        return nullptr;
    }
    const Value* find(const Key& key) const {
        int index = hashFunction(key);
        Node* current = buckets[index];
        while (current != nullptr) {
            if (current->key == key) {
                return &current->value;
            }
            current = current->next;
        }
        return nullptr;
    }
    bool contains(const Key& key) const {
        int index = hashFunction(key);
        Node* current = buckets[index];
        while (current != nullptr) {
            if (current->key == key) {
                return true;
            }
            current = current->next;
        }
        return false;
    }
    bool remove(const Key& key) {
        int index = hashFunction(key);
        Node* current = buckets[index];
        Node* previous = nullptr;
        while (current != nullptr) {
            if (current->key == key) {
                if (previous == nullptr) {
                    buckets[index] = current->next;
                }
                else {
                    previous->next = current->next;
                }
                delete current;
                --count;
                return true;
            }
            previous = current;
            current = current->next;
        }
        return false;
    }
    int size() const {
        return count;
    }
    bool empty() const {
        return count == 0;
    }
};
#endif