#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <vector>
#include <functional>
#include <utility>

template <typename Key, typename Value>
class HashTable {
private:
    struct HashNode {
        Key key;
        Value value;
        HashNode* next;

        HashNode(const Key& k, const Value& v)
            : key(k), value(v), next(nullptr) {}
    };

    std::vector<HashNode*> buckets;
    size_t capacity;
    size_t count;
    const double MAX_LOAD_FACTOR = 0.7;

    size_t hashFunction(const Key& key) const {
        return static_cast<size_t>(std::hash<Key>{}(key)) % capacity;
    }

    void rehash() {
        size_t oldCapacity = capacity;
        std::vector<HashNode*> oldBuckets = std::move(buckets);

        capacity *= 2;
        buckets.assign(capacity, nullptr);
        count = 0;

        for (size_t i = 0; i < oldCapacity; ++i) {
            HashNode* curr = oldBuckets[i];
            while (curr) {
                HashNode* next = curr->next;
                insert(curr->key, curr->value);
                delete curr;
                curr = next;
            }
        }
    }

public:
    explicit HashTable(size_t initialCapacity = 10)
        : capacity(initialCapacity), count(0) {
        buckets.assign(capacity, nullptr);
    }

    HashTable(const HashTable& other)
        : capacity(other.capacity), count(other.count) {
        buckets.assign(capacity, nullptr);
        for (size_t i = 0; i < other.capacity; ++i) {
            HashNode* curr = other.buckets[i];
            while (curr) {
                insert(curr->key, curr->value);
                curr = curr->next;
            }
        }
    }

    HashTable& operator=(const HashTable& other) {
        if (this != &other) {
            HashTable temp(other);
            std::swap(buckets, temp.buckets);
            std::swap(capacity, temp.capacity);
            std::swap(count, temp.count);
        }
        return *this;
    }

    ~HashTable() {
        for (size_t i = 0; i < capacity; ++i) {
            HashNode* curr = buckets[i];
            while (curr) {
                HashNode* next = curr->next;
                delete curr;
                curr = next;
            }
        }
    }

    void insert(const Key& key, const Value& value) {
        if (static_cast<double>(count) / capacity > MAX_LOAD_FACTOR) {
            rehash();
        }

        size_t index = hashFunction(key);
        HashNode* curr = buckets[index];

        while (curr) {
            if (curr->key == key) {
                curr->value = value;
                return;
            }
            curr = curr->next;
        }

        HashNode* newNode = new HashNode(key, value);
        newNode->next = buckets[index];
        buckets[index] = newNode;
        ++count;
    }

    Value* find(const Key& key) {
        size_t index = hashFunction(key);
        HashNode* curr = buckets[index];
        while (curr) {
            if (curr->key == key) return &(curr->value);
            curr = curr->next;
        }
        return nullptr;
    }

    const Value* find(const Key& key) const {
        size_t index = hashFunction(key);
        HashNode* curr = buckets[index];
        while (curr) {
            if (curr->key == key) return &(curr->value);
            curr = curr->next;
        }
        return nullptr;
    }

    bool remove(const Key& key) {
        size_t index = hashFunction(key);
        HashNode* curr = buckets[index];
        HashNode* prev = nullptr;

        while (curr) {
            if (curr->key == key) {
                if (prev) prev->next = curr->next;
                else buckets[index] = curr->next;
                delete curr;
                --count;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false;
    }

    bool contains(const Key& key) const {
        return find(key) != nullptr;
    }

    size_t size() const { return count; }
    bool empty() const { return count == 0; }
};

#endif 


