//
// Created by ckjsuperhh6602 on 25-5-26.
//

#ifndef UNORDERED_SET_H
#define UNORDERED_SET_H
#pragma once

#include <functional>
#include <stdexcept>
#include <vector>

namespace sjtu {

template <typename Key, typename Hash = std::hash<Key>>
class unordered_set {
private:
    // 链表节点结构
    struct Node {
        Key key;
        Node* next;
        Node(const Key& k) : key(k), next(nullptr) {}
    };

    // 哈希表容器
    std::vector<Node*> buckets;
    size_t element_count;
    float max_load_factor;
    Hash hasher;

    // 辅助函数：获取桶索引
    size_t get_bucket_index(const Key& key) const {
        return hasher(key) % buckets.size();
    }

    // 重新哈希函数
    void rehash(size_t new_size) {
        std::vector<Node*> new_buckets(new_size, nullptr);

        for (Node* node : buckets) {
            while (node != nullptr) {
                Node* next = node->next;
                size_t index = hasher(node->key) % new_size;
                node->next = new_buckets[index];
                new_buckets[index] = node;
                node = next;
            }
        }

        buckets.swap(new_buckets);
    }

public:
    // 迭代器类
    class iterator {
    private:
        Node* current_node;
        const std::vector<Node*>* buckets_ptr;
        size_t bucket_index;

        // 找到下一个非空桶
        void find_next_bucket() {
            ++bucket_index;
            while (bucket_index < buckets_ptr->size() && (*buckets_ptr)[bucket_index] == nullptr) {
                ++bucket_index;
            }
            if (bucket_index < buckets_ptr->size()) {
                current_node = (*buckets_ptr)[bucket_index];
            } else {
                current_node = nullptr;
            }
        }

    public:
        iterator(Node* node = nullptr, const std::vector<Node*>* buckets = nullptr, size_t index = 0)
            : current_node(node), buckets_ptr(buckets), bucket_index(index) {}

        iterator& operator++() {
            if (current_node->next != nullptr) {
                current_node = current_node->next;
            } else {
                find_next_bucket();
            }
            return *this;
        }

        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const iterator& other) const {
            return current_node == other.current_node;
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }

        const Key& operator*() const {
            return current_node->key;
        }

        const Key* operator->() const {
            return &(current_node->key);
        }
    };

    // 构造函数
    explicit unordered_set(size_t initial_size = 16, float load_factor = 1.0f)
        : buckets(initial_size, nullptr), element_count(0), max_load_factor(load_factor) {}

    // 析构函数
    ~unordered_set() {
        clear();
    }

    // 拷贝构造函数
    unordered_set(const unordered_set& other)
        : buckets(other.buckets.size(), nullptr), element_count(0),
          max_load_factor(other.max_load_factor), hasher(other.hasher) {
        for (Node* node : other.buckets) {
            while (node != nullptr) {
                insert(node->key);
                node = node->next;
            }
        }
    }

    // 赋值运算符
    unordered_set& operator=(const unordered_set& other) {
        if (this != &other) {
            clear();
            buckets.resize(other.buckets.size());
            std::fill(buckets.begin(), buckets.end(), nullptr);
            max_load_factor = other.max_load_factor;
            hasher = other.hasher;

            for (Node* node : other.buckets) {
                while (node != nullptr) {
                    insert(node->key);
                    node = node->next;
                }
            }
        }
        return *this;
    }

    // 迭代器相关
    iterator begin() const {
        for (size_t i = 0; i < buckets.size(); ++i) {
            if (buckets[i] != nullptr) {
                return iterator(buckets[i], &buckets, i);
            }
        }
        return end();
    }

    iterator end() const {
        return iterator(nullptr, &buckets, buckets.size());
    }

    // 容量相关
    bool empty() const {
        return element_count == 0;
    }

    size_t size() const {
        return element_count;
    }

    // 修改器
    std::pair<iterator, bool> insert(const Key& key) {
        if (load_factor() >= max_load_factor) {
            rehash(buckets.size() * 2);
        }

        size_t index = get_bucket_index(key);
        Node* current = buckets[index];

        while (current != nullptr) {
            if (current->key == key) {
                return {iterator(current, &buckets, index), false};
            }
            current = current->next;
        }

        Node* new_node = new Node(key);
        new_node->next = buckets[index];
        buckets[index] = new_node;
        ++element_count;

        return {iterator(new_node, &buckets, index), true};
    }

    size_t erase(const Key& key) {
        size_t index = get_bucket_index(key);
        Node* current = buckets[index];
        Node* prev = nullptr;

        while (current != nullptr) {
            if (current->key == key) {
                if (prev == nullptr) {
                    buckets[index] = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                --element_count;
                return 1;
            }
            prev = current;
            current = current->next;
        }

        return 0;
    }

    void clear() {
        for (Node*& node : buckets) {
            while (node != nullptr) {
                Node* temp = node;
                node = node->next;
                delete temp;
            }
        }
        element_count = 0;
    }

    // 查找
    iterator find(const Key& key) const {
        size_t index = get_bucket_index(key);
        Node* current = buckets[index];

        while (current != nullptr) {
            if (current->key == key) {
                return iterator(current, &buckets, index);
            }
            current = current->next;
        }

        return end();
    }

    size_t count(const Key& key) const {
        return find(key) != end() ? 1 : 0;
    }

    // 哈希策略
    float load_factor() const {
        return static_cast<float>(element_count) / buckets.size();
    }

    float max_load_factor() const {
        return max_load_factor;
    }

    void max_load_factor(float new_factor) {
        max_load_factor = new_factor;
        if (load_factor() > max_load_factor) {
            rehash(buckets.size() * 2);
        }
    }

    void rehash(size_t count) {
        if (count > buckets.size() || count == 0) {
            rehash(count);
        }
    }
};

} // namespace sjtu
#endif //UNORDERED_SET_H
