//
// Created by ckjsuperhh6602 on 24-12-22.
//

#ifndef DATABASE_1_H
#define DATABASE_1_H
#include <cassert>
#include <cstring>
#include <fstream>
#include <string>
#include <utility>
#include <unordered_map>

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;
template<typename T1, typename T2>
struct Pair {
    T1 first;
    T2 second;

    // 默认构造函数
    constexpr Pair() : first(), second() {}

    // 普通构造函数（处理非数组类型）
    template<typename U1, typename U2,
             std::enable_if_t<!std::is_array_v<U1> && !std::is_array_v<U2>, int> = 0>
    constexpr Pair(U1&& a, U2&& b)
        : first(std::forward<U1>(a)), second(std::forward<U2>(b)) {}

    // 特化：第一个模板参数为字符数组（任意长度N）
    template<size_t N, typename U2>
    constexpr Pair(char(&a)[N], U2&& b)
        : second(std::forward<U2>(b)) {
        // 静态断言确保T1是长度为N的char数组
        static_assert(std::is_same_v<T1, char[N]>, "Array length mismatch");
        // 拷贝数组元素（使用memcpy提高效率）
        std::memcpy(first, a, N * sizeof(char));
    }

    // 特化：第二个模板参数为字符数组（任意长度M）
    template<typename U1, size_t M>
    constexpr Pair(U1&& a, char(&b)[M])
        : first(std::forward<U1>(a)) {
        static_assert(std::is_same_v<T2, char[M]>, "Array length mismatch");
        std::memcpy(second, b, M * sizeof(char));
    }

    // 特化：两个模板参数均为字符数组（N和M可不同）
    template<size_t N, size_t M>
    constexpr Pair(char(&a)[N], char(&b)[M]) {
        static_assert(std::is_same_v<T1, char[N]>, "First array length mismatch");
        static_assert(std::is_same_v<T2, char[M]>, "Second array length mismatch");
        std::memcpy(first, a, N * sizeof(char));
        std::memcpy(second, b, M * sizeof(char));
    }
    // 比较运算符
    friend bool operator==(const Pair &lhs, const Pair &rhs) {
        return lhs.first == rhs.first && lhs.second == rhs.second;
    }

    friend bool operator!=(const Pair &lhs, const Pair &rhs) {
        return !(lhs == rhs);
    }

    friend bool operator<(const Pair &lhs, const Pair &rhs) {
        return lhs.first < rhs.first ||
               (!(rhs.first < lhs.first) && lhs.second < rhs.second);
    }

    friend bool operator>(const Pair &lhs, const Pair &rhs) {
        return rhs < lhs;
    }

    friend bool operator<=(const Pair &lhs, const Pair &rhs) {
        return !(rhs < lhs);
    }

    friend bool operator>=(const Pair &lhs, const Pair &rhs) {
        return !(lhs < rhs);
    }
};


// make_pair 函数模板
template<typename T1, typename T2>
constexpr Pair<std::decay_t<T1>, std::decay_t<T2> >
make_pair(T1 &&a, T2 &&b) {
    return Pair<std::decay_t<T1>, std::decay_t<T2> >(
        std::forward<T1>(a), std::forward<T2>(b)
    );
}
#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP
#ifndef SJTU_EXCEPTIONS_HPP
#define SJTU_EXCEPTIONS_HPP

#include <cstddef>
#include <cstring>
#include <string>

namespace sjtu {
#ifndef SJTU_SET_HPP
#define SJTU_SET_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>


    template<typename T>
    class set {
    private:
        enum class Color { RED, BLACK };

        struct Node {
            T data;
            Node *left;
            Node *right;
            Node *parent;
            Color color;

            Node(const T &value, Node *p = nullptr)
                : data(value), left(nullptr), right(nullptr), parent(p), color(Color::RED) {
            }
        };

        Node *root;
        Node *nil; // 哨兵节点
        size_t count;

        // 红黑树辅助函数
        void leftRotate(Node *x) {
            Node *y = x->right;
            x->right = y->left;
            if (y->left != nil) {
                y->left->parent = x;
            }
            y->parent = x->parent;
            if (x->parent == nil) {
                root = y;
            } else if (x == x->parent->left) {
                x->parent->left = y;
            } else {
                x->parent->right = y;
            }
            y->left = x;
            x->parent = y;
        }

        void rightRotate(Node *y) {
            Node *x = y->left;
            y->left = x->right;
            if (x->right != nil) {
                x->right->parent = y;
            }
            x->parent = y->parent;
            if (y->parent == nil) {
                root = x;
            } else if (y == y->parent->right) {
                y->parent->right = x;
            } else {
                y->parent->left = x;
            }
            x->right = y;
            y->parent = x;
        }

        void insertFixup(Node *z) {
            while (z->parent->color == Color::RED) {
                if (z->parent == z->parent->parent->left) {
                    Node *y = z->parent->parent->right;
                    if (y->color == Color::RED) {
                        z->parent->color = Color::BLACK;
                        y->color = Color::BLACK;
                        z->parent->parent->color = Color::RED;
                        z = z->parent->parent;
                    } else {
                        if (z == z->parent->right) {
                            z = z->parent;
                            leftRotate(z);
                        }
                        z->parent->color = Color::BLACK;
                        z->parent->parent->color = Color::RED;
                        rightRotate(z->parent->parent);
                    }
                } else {
                    Node *y = z->parent->parent->left;
                    if (y->color == Color::RED) {
                        z->parent->color = Color::BLACK;
                        y->color = Color::BLACK;
                        z->parent->parent->color = Color::RED;
                        z = z->parent->parent;
                    } else {
                        if (z == z->parent->left) {
                            z = z->parent;
                            rightRotate(z);
                        }
                        z->parent->color = Color::BLACK;
                        z->parent->parent->color = Color::RED;
                        leftRotate(z->parent->parent);
                    }
                }
            }
            root->color = Color::BLACK;
        }

        void transplant(Node *u, Node *v) {
            if (u->parent == nil) {
                root = v;
            } else if (u == u->parent->left) {
                u->parent->left = v;
            } else {
                u->parent->right = v;
            }
            v->parent = u->parent;
        }

        void eraseFixup(Node *x) {
            while (x != root && x->color == Color::BLACK) {
                if (x == x->parent->left) {
                    Node *w = x->parent->right;
                    if (w->color == Color::RED) {
                        w->color = Color::BLACK;
                        x->parent->color = Color::RED;
                        leftRotate(x->parent);
                        w = x->parent->right;
                    }
                    if (w->left->color == Color::BLACK && w->right->color == Color::BLACK) {
                        w->color = Color::RED;
                        x = x->parent;
                    } else {
                        if (w->right->color == Color::BLACK) {
                            w->left->color = Color::BLACK;
                            w->color = Color::RED;
                            rightRotate(w);
                            w = x->parent->right;
                        }
                        w->color = x->parent->color;
                        x->parent->color = Color::BLACK;
                        w->right->color = Color::BLACK;
                        leftRotate(x->parent);
                        x = root;
                    }
                } else {
                    Node *w = x->parent->left;
                    if (w->color == Color::RED) {
                        w->color = Color::BLACK;
                        x->parent->color = Color::RED;
                        rightRotate(x->parent);
                        w = x->parent->left;
                    }
                    if (w->right->color == Color::BLACK && w->left->color == Color::BLACK) {
                        w->color = Color::RED;
                        x = x->parent;
                    } else {
                        if (w->left->color == Color::BLACK) {
                            w->right->color = Color::BLACK;
                            w->color = Color::RED;
                            leftRotate(w);
                            w = x->parent->left;
                        }
                        w->color = x->parent->color;
                        x->parent->color = Color::BLACK;
                        w->left->color = Color::BLACK;
                        rightRotate(x->parent);
                        x = root;
                    }
                }
            }
            x->color = Color::BLACK;
        }

        Node *minimum(Node *x) const {
            while (x->left != nil) {
                x = x->left;
            }
            return x;
        }

        Node *maximum(Node *x) const {
            while (x->right != nil) {
                x = x->right;
            }
            return x;
        }

        Node *successor(Node *x) const {
            if (x->right != nil) {
                return minimum(x->right);
            }
            Node *y = x->parent;
            while (y != nil && x == y->right) {
                x = y;
                y = y->parent;
            }
            return y;
        }

        Node *predecessor(Node *x) const {
            if (x->left != nil) {
                return maximum(x->left);
            }
            Node *y = x->parent;
            while (y != nil && x == y->left) {
                x = y;
                y = y->parent;
            }
            return y;
        }

        void destroyTree(Node *x) {
            if (x == nil) return;
            destroyTree(x->left);
            destroyTree(x->right);
            delete x;
        }

        Node *findNode(const T &value) const {
            Node *x = root;
            while (x != nil) {
                if (value < x->data) {
                    x = x->left;
                } else if (x->data < value) {
                    x = x->right;
                } else {
                    return x;
                }
            }
            return nil;
        }

    public:
        class const_iterator;

        class iterator {
            friend class set;

        private:
            Node *current;
            const set *container;

            iterator(Node *node, const set *set) : current(node), container(set) {
            }

        public:
            iterator() : current(nullptr), container(nullptr) {
            }

            iterator(const iterator &other) : current(other.current), container(other.container) {
            }

            iterator &operator=(const iterator &other) {
                if (this != &other) {
                    current = other.current;
                    container = other.container;
                }
                return *this;
            }

            iterator operator++(int) {
                iterator temp = *this;
                ++(*this);
                return temp;
            }

            iterator &operator++() {
                if (current == container->nil) {
                    throw std::runtime_error("Increment past the end iterator");
                }
                current = container->successor(current);
                return *this;
            }

            iterator operator--(int) {
                iterator temp = *this;
                --(*this);
                return temp;
            }

            iterator &operator--() {
                if (current == container->begin().current) {
                    throw std::runtime_error("Decrement past the begin iterator");
                }
                if (current == container->nil) {
                    current = container->maximum(container->root);
                } else {
                    current = container->predecessor(current);
                }
                return *this;
            }

            const T &operator*() const {
                if (current == container->nil) {
                    throw std::runtime_error("Dereference of end iterator");
                }
                return current->data;
            }

            const T *operator->() const {
                if (current == container->nil) {
                    throw std::runtime_error("Dereference of end iterator");
                }
                return &(current->data);
            }

            bool operator==(const iterator &rhs) const {
                return current == rhs.current && container == rhs.container;
            }

            bool operator==(const const_iterator &rhs) const {
                return current == rhs.current && container == rhs.container;
            }

            bool operator!=(const iterator &rhs) const {
                return !(*this == rhs);
            }

            bool operator!=(const const_iterator &rhs) const {
                return !(*this == rhs);
            }
        };

        class const_iterator {
            friend class set;

        private:
            Node *current;
            const set *container;

            const_iterator(Node *node, const set *set) : current(node), container(set) {
            }

        public:
            const_iterator() : current(nullptr), container(nullptr) {
            }

            const_iterator(const const_iterator &other) : current(other.current), container(other.container) {
            }

            const_iterator(const iterator &other) : current(other.current), container(other.container) {
            }

            const_iterator &operator=(const const_iterator &other) {
                if (this != &other) {
                    current = other.current;
                    container = other.container;
                }
                return *this;
            }

            const_iterator operator++(int) {
                const_iterator temp = *this;
                ++(*this);
                return temp;
            }

            const_iterator &operator++() {
                if (current == container->nil) {
                    throw std::runtime_error("Increment past the end iterator");
                }
                current = container->successor(current);
                return *this;
            }

            const_iterator operator--(int) {
                const_iterator temp = *this;
                --(*this);
                return temp;
            }

            const_iterator &operator--() {
                if (current == container->begin().current) {
                    throw std::runtime_error("Decrement past the begin iterator");
                }
                if (current == container->nil) {
                    current = container->maximum(container->root);
                } else {
                    current = container->predecessor(current);
                }
                return *this;
            }

            const T &operator*() const {
                if (current == container->nil) {
                    throw std::runtime_error("Dereference of end iterator");
                }
                return current->data;
            }

            const T *operator->() const {
                if (current == container->nil) {
                    throw std::runtime_error("Dereference of end iterator");
                }
                return &(current->data);
            }

            bool operator==(const iterator &rhs) const {
                return current == rhs.current && container == rhs.container;
            }

            bool operator==(const const_iterator &rhs) const {
                return current == rhs.current && container == rhs.container;
            }

            bool operator!=(const iterator &rhs) const {
                return !(*this == rhs);
            }

            bool operator!=(const const_iterator &rhs) const {
                return !(*this == rhs);
            }
        };

        set() : count(0) {
            nil = new Node(T(), nullptr);
            nil->color = Color::BLACK;
            nil->left = nil;
            nil->right = nil;
            root = nil;
        }

        set(const set &other) : count(0) {
            nil = new Node(T(), nullptr);
            nil->color = Color::BLACK;
            nil->left = nil;
            nil->right = nil;
            root = nil;
            for (const_iterator it = other.begin(); it != other.end(); ++it) {
                insert(*it);
            }
        }

        ~set() {
            destroyTree(root);
            delete nil;
        }

        set &operator=(const set &other) {
            if (this != &other) {
                clear();
                for (const_iterator it = other.begin(); it != other.end(); ++it) {
                    insert(*it);
                }
            }
            return *this;
        }

        Pair<iterator, bool> insert(const T &value) {
            Node *z = findNode(value);
            if (z != nil) {
                return make_pair(iterator(z, this), false);
            }

            z = new Node(value, nil);
            Node *y = nil;
            Node *x = root;

            while (x != nil) {
                y = x;
                if (z->data < x->data) {
                    x = x->left;
                } else {
                    x = x->right;
                }
            }

            z->parent = y;
            if (y == nil) {
                root = z;
            } else if (z->data < y->data) {
                y->left = z;
            } else {
                y->right = z;
            }

            z->left = nil;
            z->right = nil;
            z->color = Color::RED;

            insertFixup(z);
            ++count;
            return make_pair(iterator(z, this), true);
        }

        void erase(iterator pos) {
            if (pos.container != this || pos.current == nil) {
                throw std::runtime_error("Invalid iterator");
            }

            Node *z = pos.current;
            Node *y = z;
            Node *x;
            Color y_original_color = y->color;

            if (z->left == nil) {
                x = z->right;
                transplant(z, z->right);
            } else if (z->right == nil) {
                x = z->left;
                transplant(z, z->left);
            } else {
                y = minimum(z->right);
                y_original_color = y->color;
                x = y->right;

                if (y->parent == z) {
                    x->parent = y;
                } else {
                    transplant(y, y->right);
                    y->right = z->right;
                    y->right->parent = y;
                }

                transplant(z, y);
                y->left = z->left;
                y->left->parent = y;
                y->color = z->color;
            }

            if (y_original_color == Color::BLACK) {
                eraseFixup(x);
            }

            delete z;
            --count;
        }

        size_t size() const {
            return count;
        }

        bool empty() const {
            return count == 0;
        }

        void clear() {
            destroyTree(root);
            root = nil;
            count = 0;
        }

        iterator find(const T &value) {
            Node *x = findNode(value);
            return iterator(x, this);
        }

        const_iterator find(const T &value) const {
            Node *x = findNode(value);
            return const_iterator(x, this);
        }

        iterator begin() {
            return iterator(minimum(root), this);
        }

        const_iterator begin() const {
            return const_iterator(minimum(root), this);
        }

        const_iterator cbegin() const {
            return const_iterator(minimum(root), this);
        }

        iterator end() {
            return iterator(nil, this);
        }

        const_iterator end() const {
            return const_iterator(nil, this);
        }

        const_iterator cend() const {
            return const_iterator(nil, this);
        }
    };
} // namespace sjtu

#endif // SJTU_SET_HPP

namespace sjtu {
    class exception {
    protected:
        const std::string variant = "";
        std::string detail = "";

    public:
        exception() {
        }

        exception(const exception &ec) : variant(ec.variant), detail(ec.detail) {
        }

        virtual std::string what() {
            return variant + " " + detail;
        }
    };

    class index_out_of_bound : public exception {
        std::string what() override {
            return "index_out_of_bound";
        }

        /* __________________________ */
    };

    class runtime_error : public exception {
        std::string what() override {
            return "runtime_error";
        }

        /* __________________________ */
    };

    class invalid_iterator : public exception {
        std::string what() override {
            return "invalid_iterator";
        }

        /* __________________________ */
    };

    class container_is_empty : public exception {
        std::string what() override {
            return "container_is_empty";
        }

        /* __________________________ */
    };
}

#endif


#include <climits>
#include <cstddef>
#include <iostream>

namespace sjtu {
    /**
     * a data container like std::vector
     * store data in a successive memory and support random access.
     */
    template<typename T>
    class vector {
    private:
        char *p;
        int maxSize;
        int currentLength;

        T *Tptr() {
            return reinterpret_cast<T *>(p);
        }

    public:
        /**
         * TODO
         * a type for actions of the elements of a vector, and you should write
         *   a class named const_iterator with same interfaces.
         */
        /**
         * you can see RandomAccessIterator at CppReference for help.
         */
        class const_iterator;

        class iterator {
            friend class vector;
            // The following code is written for the C++ type_traits library.
            // Type traits is a C++ feature for describing certain properties of a type.
            // For instance, for an iterator, iterator::value_type is the type that the
            // iterator points to.
            // STL algorithms and containers may use these type_traits (e.g. the following
            // typedef) to work properly. In particular, without the following code,
            // @code{std::sort(iter, iter1);} would not compile.
            // See these websites for more information:
            // https://en.cppreference.com/w/cpp/header/type_traits
            // About value_type: https://blog.csdn.net/u014299153/article/details/72419713
            // About iterator_category: https://en.cppreference.com/w/cpp/iterator
        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T *;
            using reference = T &;
            using iterator_category = std::output_iterator_tag;

        private:
            char *v = nullptr;
            int pos = 0;

            /**
             * TODO add data members
             *   just add whatever you want.
             */
        public:
            /**
             * return a new iterator which pointer n-next elements
             * as well as operator-
             */
            iterator operator+(const int &n) const {
                iterator tmp;
                tmp.v = this->v;
                tmp.pos = this->pos + n;
                return tmp;
            }

            iterator operator-(const int &n) const {
                iterator tmp;
                tmp.v = this->v;
                tmp.pos = this->pos - n;
                return tmp;
            }

            // return the distance between two iterators,
            // if these two iterators point to different vectors, throw invalid_iterator.
            int operator-(const iterator &rhs) const {
                if (this->v != rhs.v) {
                    throw invalid_iterator();
                }
                return this->pos - rhs.pos;
            }

            iterator &operator+=(const int &n) {
                pos += n;
                return *this;
            }

            iterator &operator-=(const int &n) {
                pos -= n;
                return *this;
            }

            /**
             * TODO iter++
             */
            iterator operator++(int) {
                iterator tmp;
                tmp.v = this->v;
                tmp.pos = this->pos;
                ++this->pos;
                return tmp;
            }

            /**
             * TODO ++iter
             */
            iterator &operator++() {
                ++this->pos;
                return *this;
            }

            /**
             * TODO iter--
             */
            iterator operator--(int) {
                iterator tmp;
                tmp.v = this->v;
                tmp.pos = this->pos;
                --this->pos;
                return tmp;
            }

            /**
             * TODO --iter
             */
            iterator &operator--() {
                --this->pos;
                return *this;
            }

            /**
             * TODO *it
             */
            T &operator*() const {
                return *(reinterpret_cast<T *>(v + sizeof(T) * pos));
            }

            /**
             * an operator to check whether two iterators are same (pointing to the same memory address).
             */
            bool operator==(const iterator &rhs) const {
                return this->v == rhs.v && this->pos == rhs.pos;
            }

            bool operator==(const const_iterator &rhs) const {
                return this->v == rhs.v && this->pos == rhs.pos;
            }

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return !(*this == rhs);
            }

            bool operator!=(const const_iterator &rhs) const {
                return !(*this == rhs);
            }
        };

        /**
         * TODO
         * has same function as iterator, just for a const object.
         */
        class const_iterator {
            friend class vector;

        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T *;
            using reference = T &;
            using iterator_category = std::output_iterator_tag;

        private:
            /*TODO*/
            char *v = nullptr;
            int pos = 0;

        public:
            /**
             * return a new iterator which pointer n-next elements
             * as well as operator-
             */
            iterator operator+(const int &n) const {
                iterator tmp;
                tmp.v = this->v;
                tmp.pos = this->pos + n;
                return tmp;
            }

            iterator operator-(const int &n) const {
                iterator tmp;
                tmp.v = this->v;
                tmp.pos = this->pos - n;
                return tmp;
            }

            // return the distance between two iterators,
            // if these two iterators point to different vectors, throw invalid_iterator.
            int operator-(const iterator &rhs) const {
                if (this->v != rhs.v) {
                    throw sjtu::invalid_iterator();
                }
                return this->pos - rhs.pos;
            }

            iterator &operator+=(const int &n) {
                pos += n;
                return *this;
            }

            iterator &operator-=(const int &n) {
                pos -= n;
                return *this;
            }

            /**
             * TODO iter++
             */
            const_iterator operator++(int) {
                const_iterator tmp;
                tmp.v = this->v;
                tmp.pos = this->pos;
                ++this->pos;
                return tmp;
            }

            /**
             * TODO ++iter
             */
            const_iterator &operator++() {
                ++this->pos;
                return *this;
            }

            /**
             * TODO iter--
             */
            const_iterator operator--(int) {
                const_iterator tmp;
                tmp.v = this->v;
                tmp.pos = this->pos;
                --this->pos;
                return tmp;
            }

            /**
             * TODO --iter
             */
            const_iterator &operator--() {
                --this->pos;
                return *this;
            }

            /**
             * TODO *it
             */
            const T &operator*() const {
                return *reinterpret_cast<T *>(v + sizeof(T) * pos);
            }

            /**
             * an operator to check whether two iterators are same (pointing to the same memory address).
             */
            bool operator==(const iterator &rhs) const {
                return this->v == rhs.v && this->pos == rhs.pos;
            }

            bool operator==(const const_iterator &rhs) const {
                return this->v == rhs.v && this->pos == rhs.pos;
            }

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return !(*this == rhs);
            }

            bool operator!=(const const_iterator &rhs) const {
                return !(*this == rhs);
            }
        };

        /**
         * TODO Constructs
         * At least two: default constructor, copy constructor
         */
        vector(): maxSize(4), currentLength(0) { p = new char[maxSize * sizeof(T)]; }

        explicit vector(int siz): maxSize(siz), currentLength(0) {
            p = new char[maxSize * sizeof(T)];
        }

        explicit vector(int siz, const T &value = T()) : maxSize(siz), currentLength(siz) {
            p = new char[maxSize * sizeof(T)];
            for (int i = 0; i < currentLength; ++i) {
                new(reinterpret_cast<T *>(p + i * sizeof(T))) T(value);
            }
        }

        vector(const vector &other) {
            if (this == &other) {
                return;
            }
            this->maxSize = other.maxSize;
            this->currentLength = other.currentLength;
            p = new char[maxSize * sizeof(T)];
            for (int i = 0; i < currentLength; i++) {
                new(reinterpret_cast<T *>(p + i * sizeof(T))) T(*reinterpret_cast<T *>(other.p + i * sizeof(T)));
            }
        }

        void swap(vector &other) {
            std::swap(p, other.p);
            std::swap(maxSize, other.maxSize);
            std::swap(currentLength, other.currentLength);
        }

        /**
         * TODO Destructor
         */
        ~vector() {
            if constexpr (!std::is_trivially_destructible_v<T>) {
                for (size_t i = 0; i < currentLength; ++i) {
                    // 正确获取对象指针并调用析构函数
                    reinterpret_cast<T *>(p + i * sizeof(T))->~T();
                }
            }
            delete[]p;
        }

        /*
         * TODO Double Space
         */
        void DoubleSize() {
            char *q = p;
            p = new char[2 * maxSize * sizeof(T)];
            for (int i = 0; i < currentLength; i++) {
                new(p + sizeof(T) * i) T(std::move(reinterpret_cast<T *>(q)[i]));
                reinterpret_cast<T *>(q)[i].~T();
            }
            // memcpy(reinterpret_cast<T*>(p),reinterpret_cast<T*>(q),maxSize*sizeof(T));
            delete[] q;
            maxSize *= 2;
        }

        /**
         * TODO Assignment operator
         */
        vector &operator=(const vector &other) {
            if (this == &other) {
                return *this;
            }
            for (int i = 0; i < currentLength; i++)
                reinterpret_cast<T *>(p + i * sizeof(T))->~T();
            delete[] p;
            this->maxSize = other.maxSize;
            this->currentLength = other.currentLength;
            p = new char[maxSize * sizeof(T)];
            for (int i = 0; i < currentLength; i++) {
                new(reinterpret_cast<T *>(p + i * sizeof(T))) T(*reinterpret_cast<T *>(other.p + i * sizeof(T)));
            }
            return *this;
        }

        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, size)
         */
        T &at(const size_t &pos) {
            if (pos < 0 || pos >= currentLength) {
                throw index_out_of_bound();
            }
            return *reinterpret_cast<T *>(p + pos * sizeof(T));
        }

        const T &at(const size_t &pos) const {
            if (pos < 0 || pos >= currentLength) {
                throw index_out_of_bound();
            }
            return *reinterpret_cast<T *>(p + pos * sizeof(T));
        }

        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, size)
         * !!! Pay attentions
         *   In STL this operator does not check the boundary, but I want you to do.
         */
        T &operator[](const size_t &pos) {
            if (pos < 0 || pos >= currentLength) {
                throw index_out_of_bound();
            }
            return *reinterpret_cast<T *>(p + pos * sizeof(T));
        }

        const T &operator[](const size_t &pos) const {
            if (pos < 0 || pos >= currentLength) {
                throw index_out_of_bound();
            }
            return *reinterpret_cast<T *>(p + pos * sizeof(T));
        }

        /**
         * access the first element.
         * throw container_is_empty if size == 0
         */
        const T &front() const {
            return *reinterpret_cast<T *>(p);
        }

        /**
         * access the last element.
         * throw container_is_empty if size == 0
         */
        const T &back() const {
            return *reinterpret_cast<T *>(p + (currentLength - 1) * sizeof(T));
        }

        /**
         * returns an iterator to the beginning.
         */
        iterator begin() {
            iterator tmp;
            tmp.v = this->p;
            tmp.pos = 0;
            return tmp;
        }

        const_iterator begin() const {
            const_iterator tmp;
            tmp.v = this->p;
            tmp.pos = 0;
            return tmp;
        }

        const_iterator cbegin() const {
            const_iterator tmp;
            tmp.v = this->p;
            tmp.pos = 0;
            return tmp;
        }

        /**
         * returns an iterator to the end.
         */
        iterator end() {
            iterator tmp;
            tmp.v = this->p;
            tmp.pos = this->currentLength;
            return tmp;
        }

        const_iterator end() const {
            const_iterator tmp;
            tmp.v = this->p;
            tmp.pos = this->currentLength;
            return tmp;
        }

        const_iterator cend() const {
            const_iterator tmp;
            tmp.v = this->p;
            tmp.pos = this->currentLength;
            return tmp;
        }

        /**
         * checks whether the container is empty
         */
        bool empty() const {
            return !currentLength;
        }

        /**
         * returns the number of elements
         */
        int size() const {
            return currentLength;
        }
        int capacity() const {
            return maxSize;
        }
        /**
         * clears the contents
         */
        void clear() {
            for (int i = 0; i < currentLength; i++) {
                reinterpret_cast<T *>(p + i * sizeof(T))->~T();
            }
            currentLength = 0;
        }

        /**
         * inserts value before pos
         * returns an iterator pointing to the inserted value.
         */
        iterator insert(iterator pos, const T &value) {
            return insert(pos.pos, value);
        }

        /**
         * inserts value at index ind.
         * after inserting, this->at(ind) == value
         * returns an iterator pointing to the inserted value.
         * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
         */
        iterator insert(const size_t &ind, const T &value) {
            if (ind > currentLength + 1) {
                throw index_out_of_bound();
            }
            if (currentLength + 1 == maxSize) {
                DoubleSize();
            }
            for (int i = currentLength; i > ind; i--) {
                new(p + (i + 1) * sizeof(T)) T(std::move(Tptr()[i]));
                // memcpy(reinterpret_cast<T*>(p+i*sizeof(T)),reinterpret_cast<T*>(p+(i-1)*sizeof(T)),sizeof(T));
            }
            currentLength++;
            new(reinterpret_cast<T *>(p + ind * sizeof(T))) T(value);
            iterator tmp;
            tmp.pos = ind;
            tmp.v = this->p;
            return tmp;
        }

        /**
         * removes the element at pos.
         * return an iterator pointing to the following element.
         * If the iterator pos refers the last element, the end() iterator is returned.
         */
        iterator erase(iterator pos) {
            return erase(pos.pos);
        }

        /**
         * removes the element with index ind.
         * return an iterator pointing to the following element.
         * throw index_out_of_bound if ind >= size
         */
        iterator erase(const size_t &ind) {
            if (ind < 0 || ind >= currentLength) {
                throw index_out_of_bound();
            }
            reinterpret_cast<T *>(p + ind * sizeof(T))->~T();
            for (int i = ind; i + 1 < currentLength; i++) {
                new(p + i * sizeof(T)) T(std::move(Tptr()[i + 1]));
                // memcpy(reinterpret_cast<T*>(p+i*sizeof(T)),reinterpret_cast<T*>(p+(i+1)*sizeof(T)),sizeof(T));
            }
            currentLength--;
            iterator tmp;
            tmp.pos = ind;
            tmp.v = this->p;
            return tmp;
        }

        /**
         * adds an element to the end.
         */
        void push_back(const T &value) {
            insert(currentLength, value);
        }

        /**
         * remove the last element from the end.
         * throw container_is_empty if size() == 0
         */
        void pop_back() {
            erase(currentLength - 1);
        }

        T *data() {
            return reinterpret_cast<T *>(p);
        }

        /**
         * Resizes the container so that it contains n elements.
         * If n is smaller than the current container size, the content is reduced to its first n elements, removing those beyond (and destroying them).
         * If n is greater than the current container size, the content is expanded by inserting at the end as many elements as needed to reach a size of n.
         * The new elements are initialized as copies of value, if provided, otherwise, they are value-initialized.
         */
        void resize(const size_t &n, const T &value = T()) {
            if (n < currentLength) {
                // 缩小容器，析构多余的元素
                for (size_t i = n; i < currentLength; ++i) {
                    reinterpret_cast<T *>(p + i * sizeof(T))->~T();
                }
                currentLength = n;
            } else if (n > currentLength) {
                // 扩大容器，需要分配更多内存
                if (n > maxSize) {
                    // 容量不足，需要重新分配
                    char *new_p = new char[n * sizeof(T)];

                    // 复制原有元素
                    for (size_t i = 0; i < currentLength; ++i) {
                        new(new_p + i * sizeof(T)) T(*reinterpret_cast<T *>(p + i * sizeof(T)));
                        reinterpret_cast<T *>(p + i * sizeof(T))->~T();
                    }

                    // 释放原有内存
                    delete[] p;
                    p = new_p;
                    maxSize = n;
                }

                // 构造新元素
                for (size_t i = currentLength; i < n; ++i) {
                    new(p + i * sizeof(T)) T(value);
                }
                currentLength = n;
            }
            // 如果 n == currentLength，不需要做任何事
        }
    }; // 结束 vector 类
};


#endif

inline int n;
struct name_and_password;
constexpr int MAXN = 174;

template<class T>
struct TreeNode {
    T reference[MAXN];
    int pointer_son[MAXN + 1]{};
    int pointer_father;

    TreeNode(): pointer_father(-1) {
        memset(pointer_son, -1, sizeof(pointer_son));
    }
};

template<class T, int info_len = 4>
class MemoryRiver {
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);

public:
    MemoryRiver() = default;

    explicit MemoryRiver(string file_name) : file_name(std::move(file_name)) {
    }

    ~MemoryRiver() {
        file.close();
    }

    void initialise(const string &FN = "") {
        if (!FN.empty()) file_name = FN;
        fstream check_file;
        check_file.open(file_name, std::ios::in|std::ios::out);
        if (!check_file.good()) {
            // if constexpr (true){
            check_file.close();
            check_file.open(file_name, std::ios::out);
            int tmp = 0;
            for (int i = 0; i < info_len; ++i)
                check_file.write(reinterpret_cast<char *>(&tmp), sizeof(int));

        }
        check_file.close();
        file.open(file_name, std::ios::in | std::ios::out);
        std::cerr << "I, " << file_name << ", AM ALSO FUCKING INITIALIZED." << std::endl;
        assert(file.good());
    }

    //读出第n个int的值赋给tmp，1_base
    void get_info(int &tmp, const int n) {
        if (n > info_len) return;
        file.seekg((n - 1) * sizeof(int));
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
    }

    int get_info(const int n) {
        int tmp;
        get_info(tmp, n);
        return tmp;
    }

    //将tmp写入第n个int的位置，1_base
    void write_info(int tmp, int n) {
        if (n > info_len) return;
        file.seekp(static_cast<int>(sizeof(int) * (n - 1)));
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
    }

    //在文件合适位置写入类对象t，并返回写入的位置索引index
    //位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
    //返回的位置索引index可以取为对象写入的起始位置
    //过程中会将index+sizeofT写入info(1)
    int write(T &t) {
        int tmp;
        get_info(tmp, 1);
        int index;
        if (tmp == 0) {
            index = sizeof(int) * info_len;
        } else {
            index = tmp;
        }
        file.seekp(index);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        write_info(index + sizeofT, 1);
        return index;
    }


    //用t的值更新位置索引index对应的对象，保证调用的index都是由write函数产生
    void update(T &t, const int index) {
        file.seekp(index);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
    }

    //读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
    void read(T &t, const int index) {
        file.seekg(index);
        file.read(reinterpret_cast<char *>(&t), sizeofT);
    }

    T read(const int index) {
        T tmp;
        read(tmp, index);
        return tmp;
    }
};


template<class T>
struct Leaf {
    sjtu::vector<Pair<T, bool> > value;
    int maxsize = 4;
    int current_size = 0;

    Leaf() = default;

    Leaf(T t, bool boolean) {
        value.resize(4);
        value[current_size++] = make_pair(t, boolean);
    }
};

template<class T, int info_len = 2>
class MemoryRiver_Leaf {
    //第一个数字为index:现在的最后一个Leaf类存到了哪个位置(取尾位置)
private:
    fstream file;
    string file_name;

public:
    MemoryRiver_Leaf() = default;

    explicit MemoryRiver_Leaf(string file_name) : file_name(std::move(file_name)) {
    }

    ~MemoryRiver_Leaf() {
        file.close();
    }

    void initialise(const string &FN = "") {
        if (!FN.empty()) file_name = FN;
        fstream check_file;
        check_file.open(file_name, std::ios::in|std::ios::out);
        if (!check_file.good()) {
            // if constexpr (true){
            check_file.close();
            check_file.open(file_name, std::ios::out);
            int tmp = 0;
            for (int i = 0; i < info_len; ++i)
                check_file.write(reinterpret_cast<char *>(&tmp), sizeof(int));

        }
        check_file.close();
        file.open(file_name, std::ios::in | std::ios::out);
        std::cerr << "I, " << file_name << ", AM ALSO FUCKING INITIALIZED." << std::endl;
        assert(file.good());
    }

    void get_info(int &tmp, const int n) {
        if (n > info_len) return;
        file.seekg(static_cast<int>((n - 1) * sizeof(int)));
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
        assert(file.good());
    }

    int get_info(const int n) {
        int tmp;
        get_info(tmp, n);
        assert(file.good());
        return tmp;
    }

    void write_info(int tmp, int n) {
        if (n > info_len) return;
        file.seekp(static_cast<int>(sizeof(int) * (n - 1)));
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        assert(file.good());
    }

    int write(Leaf<T> &t) {
        int index = get_info(1);
        if (index == 0) {
            index += sizeof(int) * info_len;
        }
        file.seekp(index);
        file.write(reinterpret_cast<char *>(&t.current_size), sizeof(int));
        file.write(reinterpret_cast<char *>(&t.maxsize), sizeof(int));
        file.write(reinterpret_cast<char *>(t.value.data()), t.maxsize * sizeof(Pair<T, bool>));
        write_info(index + 2 * sizeof(int) + sizeof(Pair<T, bool>) * t.maxsize, 1);
        assert(file.good());
        return index;
    }
    int write(sjtu::vector<T> &t) {
        Leaf<T> tt;
        tt.maxsize=t.capacity();
        tt.current_size=t.size();
        for (auto x:t) {
            tt.value.push_back({x,true});
        }
        assert(file.good());
        return write(tt);
    }

    void update(Leaf<T> &t, int index) {
        file.seekp(index);
        file.write(reinterpret_cast<char *>(&t.current_size), sizeof(int));
        file.write(reinterpret_cast<char *>(&t.maxsize), sizeof(int));
        file.write(reinterpret_cast<char *>(t.value.data()), t.value.size() * sizeof(Pair<T, bool>));
    }

    Leaf<T> read(const int index) {
        Leaf<T> tmp;
        file.seekg(index);
        file.read(reinterpret_cast<char *>(&tmp.current_size), sizeof(int));
        file.read(reinterpret_cast<char *>(&tmp.maxsize), sizeof(int));
        tmp.value.resize(tmp.maxsize);
        file.read(reinterpret_cast<char *>(tmp.value.data()), tmp.maxsize * sizeof(Pair<T, bool>));
        return tmp;
    }
};

template<class T1, class T2>
class BPT {
    MemoryRiver<TreeNode<T1> > node_file;
    MemoryRiver_Leaf<T2> leaf_file;

    static int son_number(const TreeNode<T1> &t) {
        for (int i = 0; i < MAXN; i++) {
            if (t.pointer_son[i] == -1) {
                return i;
            }
        }
        return MAXN;
    }

    void Split(const int now_node, TreeNode<T1> &a) {
        TreeNode<T1> new_node;
        const int new_index = node_file.write(new_node);
        for (int i = 0; i < MAXN / 2; i++) {
            new_node.pointer_son[i] = a.pointer_son[i + MAXN / 2];
            auto tmp = node_file.read(new_node.pointer_son[i]);
            tmp.pointer_father = new_index;
            node_file.update(tmp, new_node.pointer_son[i]);
            a.pointer_son[i + MAXN / 2] = -1;
            strcpy(new_node.reference[i], a.reference[i + MAXN / 2]);
        } //把a作为原本的节点，再开一个新节点，两个节点作为兄弟一起连到父亲上边
        if (a.pointer_father == -1) {
            //这个需要分裂的节点已然是根
            TreeNode<T1> root;
            strcpy(root.reference[0], a.reference[MAXN / 2 - 1]); //比如四个叉树，分裂的节点顶多三个索引，四个节点儿子，把第二个索引拿出来，也就是下标为1
            const int index = node_file.write(root);
            a.pointer_father = new_node.pointer_father = index;
            root.pointer_son[0] = now_node;
            root.pointer_son[1] = new_index;
            node_file.update(root, index);
            node_file.update(a, now_node);
            node_file.update(new_node, new_index);
            node_file.write_info(index, 3);
            node_file.write_info(node_file.get_info(2) + 1, 2);
            return;
        }
        auto father = node_file.read(a.pointer_father);
        new_node.pointer_father = a.pointer_father;
        node_file.update(a, now_node);
        node_file.update(new_node, new_index);
        const int num_son = son_number(father);
        for (int i = 0; i < num_son; i++) {
            if (father.pointer_son[i] == now_node) {
                //塞在i+1的位置！
                for (int j = num_son - 1; j >= i + 1; --j) {
                    father.pointer_son[j + 1] = father.pointer_son[j]; //in case of RE,I set my array size to MAXN+1
                    strcpy(father.reference[j], father.reference[j - 1]);
                }
                father.pointer_son[i + 1] = new_index;
                strcpy(father.reference[i], a.reference[MAXN / 2 - 1]);
                break;
            }
        }
        if (num_son + 1 == MAXN) {
            Split(a.pointer_father, father);
        } else {
            node_file.update(father, a.pointer_father);
        }
    }

    int search_to_leaf(T1 index) {
        int depth = node_file.get_info(2), now_node = node_file.get_info(3);
        while (depth > 1) {
            --depth;
            TreeNode<T1> a = node_file.read(now_node);
            const int num = son_number(a);
            for (int i = 0; i < num; ++i) {
                if (i == num - 1 || strcmp(a.reference[i], index) > 0) {
                    now_node = a.pointer_son[i];
                    break;
                }
            }
        }
        return now_node;
    }

public:
    BPT() = default;

    void initialise(const string &s1, const string &s2) {
        node_file.initialise(s1);
        leaf_file.initialise(s2);
    }

    bool empty() {
        if (!node_file.get_info(2)) {
            return true;
        }
        return false;
    }

    //第一位存index，第二位存depth，第三位root
    void insert(T1 index, const T2 value) {
        if (const int depth = node_file.get_info(2); !depth) {
            TreeNode<T1> root;
            Leaf<T2> a(value, true);
            root.pointer_son[0] = leaf_file.write(a);
            strcpy(root.reference[0], index);
            node_file.write_info(node_file.write(root), 3); //write the address of the root into position 3
            node_file.write_info(1, 2); //change depth into 1
            return;
        }
        const int now_node = search_to_leaf(index);
        TreeNode<T1> a = node_file.read(now_node);
        const int num = son_number(a);
        int pos = 0;
        for (int i = 0; i <= num; ++i) {
            const int state = i < num ? strcmp(a.reference[i], index) : 1;
            if (!state) {
                if (Leaf<T2> tmp = leaf_file.read(a.pointer_son[i]); tmp.current_size + 1 == tmp.maxsize) {
                    tmp.maxsize <<= 1;
                    tmp.value.resize(tmp.maxsize);
                    tmp.value[tmp.current_size++] = {value, true};
                    a.pointer_son[i] = leaf_file.write(tmp);
                    node_file.update(a, now_node);
                } else {
                    tmp.value[tmp.current_size++] = {value, true};
                    leaf_file.update(tmp, a.pointer_son[i]);
                }
                return;
            } // 原本就有
            if (state > 0) {
                pos = i;
                break;
            } // 原本没有
        }
        for (int i = num - 1; i >= pos; --i) {
            strcpy(a.reference[i + 1], a.reference[i]);
            a.pointer_son[i + 1] = a.pointer_son[i];
        }
        Leaf<T2> new_leaf(value, true);
        strcpy(a.reference[pos], index);
        a.pointer_son[pos] = leaf_file.write(new_leaf);
        node_file.update(a, now_node);
        if (num != MAXN - 1) {
            return;
        }
        TreeNode<T1> new_node;
        for (int i = 0; i < MAXN / 2; i++) {
            new_node.pointer_son[i] = a.pointer_son[i + MAXN / 2];
            a.pointer_son[i + MAXN / 2] = -1;
            strcpy(new_node.reference[i], a.reference[i + MAXN / 2]);
        }
        node_file.update(a, now_node);
        if (a.pointer_father == -1) {
            TreeNode<T1> root;
            strcpy(root.reference[0], new_node.reference[0]);
            const int root_node = node_file.write(root);
            a.pointer_father = new_node.pointer_father = root_node;
            root.pointer_son[0] = now_node;
            root.pointer_son[1] = node_file.write(new_node);
            node_file.update(a, now_node);
            node_file.update(root, root_node);
            node_file.write_info(root_node, 3);
            node_file.write_info(2, 2);
            return;
        }
        auto father = node_file.read(a.pointer_father);
        new_node.pointer_father = a.pointer_father;
        const int num_son = son_number(father);
        for (int i = 0; i < num_son; i++) {
            if (father.pointer_son[i] == now_node) {
                //塞在i+1的位置！
                for (int j = num_son - 1; j >= i + 1; --j) {
                    father.pointer_son[j + 1] = father.pointer_son[j];
                    strcpy(father.reference[j], father.reference[j - 1]);
                }
                father.pointer_son[i + 1] = node_file.write(new_node);
                strcpy(father.reference[i], new_node.reference[0]);
                break;
            }
        }
        if (num_son + 1 == MAXN) {
            Split(a.pointer_father, father);
        } else {
            node_file.update(father, a.pointer_father);
        }
    }

    sjtu::vector<T2> search(T1 index) {
        if (const int depth = node_file.get_info(2); !depth)
            return {};
        const int now_node = search_to_leaf(index);
        const auto a = node_file.read(now_node);
        const int num = son_number(a);
        for (int i = 0; i < num; ++i) {
            const int state = strcmp(a.reference[i], index);
            if (!state) {
                Leaf<T2> tmp = leaf_file.read(a.pointer_son[i]);
                sjtu::set<T2> S;
                for (int j = 0; j < tmp.current_size; j++) {
                    if (const auto [fi, se] = tmp.value[j]; se) {
                        S.insert(fi);
                    } else {
                        auto it = S.find(fi); // 使用 find 获取迭代器
                        if (it != S.end()) {
                            // 检查元素是否存在
                            S.erase(it); // 现在传入的是迭代器，类型匹配
                        }
                    }
                }
                sjtu::vector<T2> res;
                for (const auto t: S)
                    res.push_back(t);
                if (res.size() != tmp.value.size()) {
                    //更新value
                    tmp.current_size = static_cast<int>(res.size());
                    for (int j = 0; j < static_cast<int>(res.size()); ++j) {
                        tmp.value[j] = make_pair(res[j], true);
                    }
                }
                return res;
            }
            if (state > 0)
                return {};
        }
        return {};
    }

    void Val_delete(T1 index, const T2 value) {
        if (const int depth = node_file.get_info(2); !depth)
            return;
        const int now_node = search_to_leaf(index);
        auto a = node_file.read(now_node);
        const int num = son_number(a);
        for (int i = 0; i <= num; ++i) {
            const int state = i < num ? strcmp(a.reference[i], index) : 1;
            if (!state) {
                if (Leaf<T2> tmp = leaf_file.read(a.pointer_son[i]); tmp.current_size + 1 == tmp.maxsize) {
                    tmp.maxsize <<= 1;
                    tmp.value.resize(tmp.maxsize);
                    tmp.value[tmp.current_size++] = {value, false};
                    a.pointer_son[i] = leaf_file.write(tmp);
                    node_file.update(a, now_node);
                } else {
                    tmp.value[tmp.current_size++] = {value, false};
                    leaf_file.update(tmp, a.pointer_son[i]);
                }
                return;
            } // 原本就有
            if (state > 0) {
                return;
            } // 原本没有
        }
    }

    void Val_delete(T1 index) {
        if (const int depth = node_file.get_info(2); !depth)
            return;
        const int now_node = search_to_leaf(index);
        auto a = node_file.read(now_node);
        const int num = son_number(a);
        for (int i = 0; i <= num; ++i) {
            const int state = i < num ? strcmp(a.reference[i], index) : 1;
            if (!state) {
                Leaf<T2> tmp = leaf_file.read(a.pointer_son[i]);
                tmp.current_size = 0; //清空索引下的vector
                leaf_file.update(tmp,a.pointer_son[i]);
                return;
            } // 原本就有
            if (state > 0) {
                return;
            } // 原本没有
        }
    }

    void shift(Pair<T1, T2> p1, Pair<T1, T2> p2) {
        Val_delete(p1.first, p1.second);
        insert(p2.first, p2.second);
    }

    void update(sjtu::vector<T2> r,T1 index) {
        if (const int depth = node_file.get_info(2); !depth)
            return ;
        const int now_node = search_to_leaf(index);
        auto a = node_file.read(now_node);
        const int num = son_number(a);
        for (int i = 0; i < num; ++i) {
            const int state = strcmp(a.reference[i], index);
            if (!state) {
                Leaf<T2> tmp = leaf_file.read(a.pointer_son[i]);
                a.pointer_son[i]=leaf_file.write(r);
                node_file.update(a,now_node);
            }
            if (state > 0)
                return ;
        }
    }

};

template<class T, int info_len = 2>
class MemoryRiver_vector {
    //第一个数字为index:现在的最后一个Leaf类存到了哪个位置(取尾位置);
private:
    fstream file;
    string file_name;

public:
    MemoryRiver_vector() = default;

    explicit MemoryRiver_vector(string file_name) : file_name(std::move(file_name)) {
    }

    ~MemoryRiver_vector() {
        file.close();
    }

    void initialise(const string &FN = "") {
        if (!FN.empty()) file_name = FN;
        fstream check_file;
        check_file.open(file_name, std::ios::in|std::ios::out);
        if (!check_file.good()) {
            // if constexpr (true){
            check_file.close();
            check_file.open(file_name, std::ios::out);
            int tmp = 0;
            for (int i = 0; i < info_len; ++i)
                check_file.write(reinterpret_cast<char *>(&tmp), sizeof(int));

        }
        check_file.close();
        file.open(file_name, std::ios::in | std::ios::out);
        std::cerr << "I, " << file_name << ", AM ALSO FUCKING INITIALIZED." << std::endl;
        assert(file.good());
    }

    void get_info(int &tmp, const int n) {
        if (n > info_len) return;
        file.seekg(static_cast<int>((n - 1) * sizeof(int)));
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
    }

    int get_info(const int n) {
        int tmp;
        get_info(tmp, n);
        return tmp;
    }

    void write_info(int tmp, const int n) {
        if (n > info_len) return;
        file.seekp(static_cast<int>(sizeof(int) * (n - 1)));
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
    }

    int size() {
        return get_info(2);
    }

    bool empty() {
        return get_info(2) == 0;
    }

    sjtu::vector<T> get_vector() {
        sjtu::vector<T> tmp;
        for (int i = 0; i < this->size(); i++) {
            tmp.push_back(read(i));
        }
        return tmp;
    }

    int write(T t) {
        int index = get_info(1);
        if (index == 0) {
            index += sizeof(int) * info_len;
        }
        const int n = get_info(2);
        file.seekp(index);
        file.write(reinterpret_cast<char *>(&t), sizeof(T));
        write_info(index + sizeof(T), 1);
        write_info(n + 1, 2);
        return index;
    }

    void update(T t, const int n) {
        //0-base
        file.seekp(sizeof(int) * info_len + n * sizeof(T));
        file.write(reinterpret_cast<char *>(&t), sizeof(T));
    }

    T read(const int n) {
        //0-base
        T tmp;
        file.seekg(sizeof(int) * info_len + n * sizeof(T));
        file.read(reinterpret_cast<char *>(&tmp), sizeof(T));
        return tmp;
    }
};
#include <algorithm>
#include <iterator>
namespace sjtu {

    template<typename Iterator, typename Compare>
    void quick_sort(Iterator first, Iterator last, Compare comp) {
        const int INSERTION_THRESHOLD = 16;
        auto size = std::distance(first, last);

        // 小数组使用插入排序
        if (size < INSERTION_THRESHOLD) {
            for (auto i = first; i != last; ++i)
                std::rotate(std::upper_bound(first, i, *i, comp), i, std::next(i));
            return;
        }

        // 三数取中选择基准
        auto mid = first + size / 2;
        auto last_elem = std::prev(last);

        // 三数排序
        if (comp(*mid, *first)) std::iter_swap(first, mid);
        if (comp(*last_elem, *mid)) std::iter_swap(mid, last_elem);
        if (comp(*mid, *first)) std::iter_swap(first, mid);

        // 基准值放在倒数第二个位置
        std::iter_swap(mid, last_elem);
        auto pivot = *last_elem;

        // 三路划分 (Dutch National Flag算法)
        auto left = first;     // 小于区域的右边界
        auto right = last_elem; // 大于区域的左边界

        for (auto i = first; i < right;) {
            if (comp(*i, pivot)) {
                std::iter_swap(i++, left++);
            } else if (comp(pivot, *i)) {
                std::iter_swap(i, --right);
            } else {
                ++i; // 相等元素留在中间
            }
        }

        // 将基准值放到正确位置
        std::iter_swap(right, last_elem);

        // 递归排序子数组
        quick_sort(first, left, comp);
        quick_sort(right + 1, last, comp);
    }

    // 默认比较器版本
    template<typename Iterator>
    void quick_sort(Iterator first, Iterator last) {
        quick_sort(first, last, std::less<>{});
    }


    template<typename Key, typename Hash = std::hash<Key> >
    class unordered_set {
    private:
        // 链表节点结构
        struct Node {
            Key key;
            Node *next;

            Node(): key(0), next(nullptr) {
            }

            // 拷贝构造
            Node(const Key &k) : key(k), next(nullptr) {
            }

            // 移动构造
            Node(Key &&k) : key(std::move(k)), next(nullptr) {
            }
        };

        // 哈希表容器
        vector<Node *> buckets;
        size_t element_count;
        float max_load;
        Hash hasher;

        // 获取桶索引
        size_t get_bucket_index(const Key &key) const {
            return hasher(key) % buckets.size();
        }

        // 重新哈希
        void rehash(size_t new_size) {
            vector<Node *> new_buckets(new_size, nullptr);

            for (size_t i = 0; i < buckets.size(); ++i) {
                Node *current = buckets[i];
                while (current != nullptr) {
                    Node *next = current->next;
                    size_t index = hasher(current->key) % new_size;
                    current->next = new_buckets[index];
                    new_buckets[index] = current;
                    current = next;
                }
            }

            buckets.swap(new_buckets);
        }

    public:
        // 迭代器类
        class iterator {
        private:
            Node *current_node;
            const vector<Node *> *buckets_ptr;
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
            iterator(Node *node = nullptr, const vector<Node *> *buckets = nullptr, size_t index = 0)
                : current_node(node), buckets_ptr(buckets), bucket_index(index) {
            }

            iterator &operator++() {
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

            bool operator==(const iterator &other) const {
                return current_node == other.current_node;
            }

            bool operator!=(const iterator &other) const {
                return !(*this == other);
            }

            const Key &operator*() const {
                return current_node->key;
            }

            const Key *operator->() const {
                return &(current_node->key);
            }
        };

        // 构造函数
        explicit unordered_set(size_t initial_size = 16, float load_factor = 1.0f)
            : buckets(initial_size, nullptr), element_count(0), max_load(load_factor) {
        }

        // 析构函数
        ~unordered_set() {
            clear();
        }

        // 拷贝构造函数
        unordered_set(const unordered_set &other)
            : buckets(other.buckets.size(), nullptr), element_count(0),
              max_load(other.max_load), hasher(other.hasher) {
            for (size_t i = 0; i < other.buckets.size(); ++i) {
                Node *current = other.buckets[i];
                while (current != nullptr) {
                    insert(current->key);
                    current = current->next;
                }
            }
        }

        // 赋值运算符
        unordered_set &operator=(const unordered_set &other) {
            if (this != &other) {
                clear();
                buckets.resize(other.buckets.size(), nullptr);
                max_load = other.max_load;
                hasher = other.hasher;

                for (size_t i = 0; i < other.buckets.size(); ++i) {
                    Node *current = other.buckets[i];
                    while (current != nullptr) {
                        insert(current->key);
                        current = current->next;
                    }
                }
            }
            return *this;
        }

        // 迭代器
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

        // 容量
        bool empty() const {
            return element_count == 0;
        }

        size_t size() const {
            return element_count;
        }

        // 修改器
        // 拷贝插入
        Pair<iterator, bool> insert(const Key &key) {
            if (load_factor() >= max_load) {
                reserve(buckets.size() * 2);
            }

            size_t index = get_bucket_index(key);
            Node *current = buckets[index];

            while (current != nullptr) {
                if (current->key == key) {
                    return {iterator(current, &buckets, index), false};
                }
                current = current->next;
            }

            Node *new_node = new Node(key); // 使用拷贝构造
            new_node->next = buckets[index];
            buckets[index] = new_node;
            ++element_count;

            return {iterator(new_node, &buckets, index), true};
        }

        // 移动插入
        Pair<iterator, bool> insert(Key &&key) {
            if (load_factor() >= max_load) {
                reserve(buckets.size() * 2);
            }

            size_t index = get_bucket_index(key);
            Node *current = buckets[index];

            while (current != nullptr) {
                if (current->key == key) {
                    return {iterator(current, &buckets, index), false};
                }
                current = current->next;
            }

            Node *new_node = new Node(std::move(key)); // 使用移动构造
            new_node->next = buckets[index];
            buckets[index] = new_node;
            ++element_count;

            return {iterator(new_node, &buckets, index), true};
        }

        size_t erase(const Key &key) {
            size_t index = get_bucket_index(key);
            Node *current = buckets[index];
            Node *prev = nullptr;

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
            for (size_t i = 0; i < buckets.size(); ++i) {
                while (buckets[i] != nullptr) {
                    Node *temp = buckets[i];
                    buckets[i] = buckets[i]->next;
                    delete temp;
                }
            }
            element_count = 0;
        }

        // 查找
        iterator find(const Key &key) const {
            size_t index = get_bucket_index(key);
            Node *current = buckets[index];

            while (current != nullptr) {
                if (current->key == key) {
                    return iterator(current, &buckets, index);
                }
                current = current->next;
            }

            return end();
        }

        size_t count(const Key &key) const {
            return find(key) != end() ? 1 : 0;
        }

        // 哈希策略
        float load_factor() const {
            return static_cast<float>(element_count) / buckets.size();
        }

        float max_load_factor() const {
            return max_load;
        }

        void max_load_factor(float new_factor) {
            max_load = new_factor;
            if (load_factor() > max_load) {
                reserve(buckets.size() * 2);
            }
        }

        void reserve(size_t count) {
            if (count > buckets.size()) {
                rehash(count);
            }
        }
    };
} // namespace sjtu

template<class T>
class char_more {
private:
    char a[61]{};
    int the_size;

public:
    explicit char_more(const string &str): the_size(sizeof(T) / sizeof(char)) {
        if (str.size() > the_size) {
            throw std::runtime_error("");
        }
        for (int i = 0; i < str.size(); i++) {
            a[i] = str[i];
        }
    }

    char_more(): the_size(sizeof(T) / sizeof(char)) {
    }

    string get_char() {
        T tmp;
        strncpy(tmp, a, sizeof(T) / sizeof(char));
        return string(tmp);
    }

    [[nodiscard]] int get_size() const {
        return the_size;
    }
};

struct user_info {
    char password[31]{};
    char name[16]{};
    char mail[31]{};
    int privilege{};

    user_info() = default;

    user_info(char password[31], char name[16], char mail[31], const int privilege) {
        strcpy(this->password, password);
        strcpy(this->name, name);
        strcpy(this->mail, mail);
        this->privilege = privilege;
    }
};

struct login_info {
    char username[21]{};
    bool log{};

    login_info() = default;

    login_info(char username[21], const bool &log) {
        strcpy(this->username, username);
        this->log = log;
    }
};

struct CharArrayHash {
    size_t operator()(const char (&arr)[21]) const {
        size_t hash = 0;
        for (size_t i = 0; i < 21 && arr[i] != '\0'; ++i) {
            hash = (hash * 31) + arr[i];
        }
        return hash;
    }
};

struct train_info {
    int num{};
    char station[35][31]{};
    int seatnum{};
    int price[35]{};
    char starttimes[6]{};
    int traveltimes[35]{};
    int stopovertimes[35]{};
    char saledate[2][6]{};
    char type{};
    int remain_seat[99][35]{}; //某一天的某站
    bool release = false;
};

struct transfer_info {
    char from[31]{};
    char to[31]{};
    int I{}, J{};
    char id[21]{};
    int address{}; //可以直接到train_storage查詢到火車的信息，不需要敬禮BPT
    int price{};
    char saledate[2][6]{};
    int setup_hour{}, setup_minute{};
    int arrival_hour{}, arrival_minute{};
    int arrival_nextday{}, setup_nextday{};
    transfer_info(char from[31], char to[31], int I, int J,
        char id[21], const int &address, const int &price,
        char start_sell[6], char end_sell[6], int setup_hour,
        int arrival_hour, int setup_minute,int arrival_minute,
        int arrival_nextday, int setup_nextday): I(I), J(J),address(address),
                                                                                                                                                                                                                                                                      price(price), setup_hour(setup_hour),
                                                                                                                                                                                                                                                                      arrival_hour(arrival_hour),
                                                                                                                                                                                                                                                                      setup_minute(setup_minute),
                                                                                                                                                                                                                                                                      arrival_minute(arrival_minute),
                                                                                                                                                                                                                                                                      arrival_nextday(arrival_nextday),
                                                                                                                                                                                                                                                                      setup_nextday(setup_nextday) {
        strcpy(this->from, from), strcpy(this->to, to), strcpy(this->id, id), strcpy(saledate[0], start_sell), strcpy(saledate[1], end_sell);
    }

    transfer_info() = default;

    transfer_info &operator=(const transfer_info &other) {
        if (this == &other) {
            return *this;
        }
        I = other.I;
        J = other.J;
        strcpy(id, other.id);
        address = other.address;
        price = other.price;
        strcpy(saledate[0], other.saledate[0]);
        strcpy(saledate[1], other.saledate[1]);
        setup_hour = other.setup_hour;
        setup_minute = other.setup_minute;
        arrival_hour = other.arrival_hour;
        arrival_minute = other.arrival_minute;
        arrival_nextday = other.arrival_nextday;
        setup_nextday = other.setup_nextday;
        return *this;
    }
};

struct char_31 {
    char a[31]{};
    char_31()=default;
    explicit char_31(const char b[31]) {
        strcpy(a, b);
    }

    bool operator<(const char b[31]) const {
        return strcmp(a, b) < 0;
    }
    bool operator<(const char_31 &b) const {
        return strcmp(a, b.a) < 0;
    }

    bool operator==(const char_31 &b) const {
        return strcmp(a, b.a) == 0;
    }

    bool operator==(const char b[31]) const {
        return strcmp(a, b) == 0;
    }
};

struct queue {
    int queue_order;
    int order;
    int address;
    int day;
    char id[21]{};
    char from[31]{}, to[31]{};
    char time1[13]{},time2[13]{};
    char username[21]{};
    int buy_num;
    int price;
    int state;
    bool operator<(const queue &other) {
        if (order < other.order) {
            return true;
        }
        if (order == other.order) {
            if (state<other.state) {
                return true;
            }
        }
        return false;
    }
    bool operator<(const queue &other)const  {
        if (order < other.order) {
            return true;
        }
        if (order == other.order) {
            if (state<other.state) {
                return true;
            }
        }
        return false;
    }
    queue() =default;
    queue(int queue_order,int order,int address,int day,char id[21],
    char from[31],char to[31],char time1[13],char time2[13],char username[21],
    int buy_num,int price,int state):queue_order(queue_order),order(order),address(address),day(day),buy_num(buy_num),price(price),state(state) {
        strcpy(this->id,id),strcpy(this->from,from),strcpy(this->to,to),strcpy(this->time1,time1),strcpy(this->time2,time2),strcpy(this->username,username);
    }
};

struct queue_info{
    int order{};
    char username[21]{};
    int pos{};
    queue_info()=default;
    queue_info(int order,char username[21],int pos):order(order),pos(pos) {
        strcpy(this->username,username);
    }
    bool operator<(const queue_info&other)const {
        if (order<other.order) {
            return true;
        }
        if (order==other.order) {
            if (strcmp(username,other.username)<0) {
                return true;
            }
        }
        return false;
    }
};
inline BPT<char[21], int> user_file; //存儲用戶名到存儲地址的映射
inline MemoryRiver_vector<user_info> user_storage; //利用地址存儲特定用户的info
inline sjtu::unordered_set<std::string> login_state; //存儲有哪些處於登陸狀態
inline BPT<char[21], int> train_file; //存儲火车ID到存儲地址的映射
inline MemoryRiver_vector<train_info> train_storage; //利用地址存儲特定火车的静态info
inline BPT<char[31], char_31> from_to_file; //from到to的存儲
inline BPT<char[63], int> transfer_file; //两站(前後信息拼接起來)到換乘信息對應的地址
inline MemoryRiver_vector<transfer_info> transfer_storage; //換乘信息的存儲位置
inline BPT<char[21], queue> queue_file; //用戶名到排隊信息
inline BPT<char[31], queue_info> queue_storage; //車次與日期到排隊信息
inline BPT<char[31],int> train_queue_file;//等车排队到第几位了
#pragma once

#endif
