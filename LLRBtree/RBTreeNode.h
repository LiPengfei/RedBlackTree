#pragma once
#include "../Common/common.h"

class RBTNode {
    friend class RBTree;

private:
    enum class Color : unsigned int {
        Red = 1,
        Black = 2,
    };

public:
    RBTNode(const int &val)
        : m_element(new int(val)),
        m_left(nullptr),
        m_right(nullptr) { }


    virtual ~RBTNode() {
        delete m_element;
        delete m_left;
        delete m_right;
    }


    RBTNode(const RBTNode &rhs) = delete;
    RBTNode &operator=(const RBTNode &rhs) = delete;

private:
    int GetValue() const {
        return *m_element;
    }

private:
    static RBTNode *Insert(RBTNode *node, const int &val) {
        if (node==nullptr) {
            return new RBTNode(val);
        }

        // 对于 head 来讲完全没有问题
        if (_IsRed(node->m_left) && _IsRed(node->m_right)) {
            node->_FlipThree();
        }

        if (val < node->GetValue()) {
            node->m_left = Insert(node->m_left, val);
        }
        else if (val == node->GetValue()) {
            assert(false);
        }
        else {
            node->m_right = Insert(node->m_right, val);
        }

        // 子树发生变化之后调整自身。
        if (_IsRed(node->m_right)) {
            node = node->_LeftRotate();
        }

        if (_IsRed(node->m_left) && _IsRed(node->m_left->m_left)) {
            node = node->_RightRotate();
        }

        return node;
    }


    static RBTNode *DeleteMax(RBTNode *node) {
        if (_IsRed(node->m_left) && !_IsRed(node->m_right)) {
            node = node->_RightRotate();
        }

        if (node->m_right == nullptr) {
            delete node;
            return nullptr;
        }

        if (!_IsRed(node->m_right) && !_IsRed(node->m_right->m_left)) {
            node = _MoveRedRight(node);
        }

        node->m_right = DeleteMax(node->m_right);

        return _Fixup(node);
    }


    static RBTNode *DeleteMin(RBTNode *node) {
        if (node->m_left == nullptr) {
            delete node;
            return nullptr;
        }

        // node->m_left 标志了一个 2-3-4 node, 并且是个 2-node。
        if (!_IsRed(node->m_left) && !_IsRed(node->m_left->m_left)) {
            node = _MoveRedLeft(node);
        }

        node->m_left = DeleteMin(node->m_left);

        return _Fixup(node);
    }


    static RBTNode *Delete(RBTNode *node, const int &val) {
        if (val < node->GetValue()) {
            if (!_IsRed(node->m_left) && (!_IsRed(node->m_left->m_left))) {
                node = _MoveRedLeft(node);
            }
            node->m_left = Delete(node->m_left, val);
        }
        else {
            if (_IsRed(node->m_left) && !_IsRed(node->m_right)) {
                node = node->_RightRotate();
            }

            if (val == node->GetValue() && node->m_right == nullptr) {
                delete node;
                return nullptr;
            }

            if (!_IsRed(node->m_right) && !_IsRed(node->m_right->m_left)) {
                node = _MoveRedRight(node);
            }

            if (val == node->GetValue()) {
                RBTNode *minNode = _GetMin(node->m_right);
                int *swpTemp = node->m_element;
                node->m_element = minNode->m_element;
                minNode->m_element = swpTemp;
                node->m_right = DeleteMin(node->m_right);
            }
            else {
                node->m_right = Delete(node->m_right, val);
            }
        }

        return _Fixup(node);
    }
private:
    static bool _IsRed(const RBTNode *node) {
        if (node == nullptr) return false;
        return node->m_color == Color::Red;
    }


    static RBTNode *_Fixup(RBTNode *node) {
            if (_IsRed(node->m_right)) {
                node = node->_LeftRotate();
            }

            if (_IsRed(node->m_left) && _IsRed(node->m_left->m_left)) {
                node = node->_RightRotate();
            }

        return node;
    }

    // 因为 LL 特性， 大量减少了需要处理的情况。
    static RBTNode *_MoveRedLeft(RBTNode *node) {
        node->_FlipThree();
        if (_IsRed(node->m_right->m_left)) {
            node->m_right = node->m_right->_RightRotate();
            node = node->_LeftRotate();
            node->_FlipThree();

            // bug fix 右节点为 4-node的情况
            if (_IsRed(node->m_right->m_right))
                node->m_right = node->m_right->_LeftRotate();
        }

        return node;
    }


    static RBTNode *_MoveRedRight(RBTNode *node) {
        node->_FlipThree();

        if (_IsRed(node->m_left->m_left)) {
            // 这个操作不考虑左边是3-node 还是4-node。
            // 3-node 就借一个来， 4-node就借两个来。
            node = node->_RightRotate();
            node->_FlipThree();

            // if 3-node
            if (!_IsRed(node->m_right->m_left)) {
                node->m_right = node->m_right->_LeftRotate();
            }
        }

        return node;
    }


    RBTNode *_LeftRotate() {
        RBTNode *newRoot = this->m_right;
        this->m_right = newRoot->m_left;
        newRoot->m_left = this;
        newRoot->m_color = newRoot->m_left->m_color;
        newRoot->m_left->m_color = Color::Red;
        return newRoot;
    }


    RBTNode *_RightRotate() {
        RBTNode *newRoot = this->m_left;
        this->m_left = newRoot->m_right;
        newRoot->m_right = this;
        newRoot->m_color = newRoot->m_right->m_color;
        newRoot->m_right->m_color = Color::Red;
        return newRoot;
    }


    static RBTNode *_GetMin(RBTNode *node) {
        RBTNode *ret = nullptr;

        while (node != nullptr) {
            ret = node;
            node = node->m_left;
        }

        return ret;
    }


private:
    void _FlipColor() {
        m_color = m_color == Color::Black ? Color::Red : Color::Black;
    }


    void _FlipThree() {
        _FlipColor();
        if (m_left) m_left->_FlipColor();
        if (m_right) m_right->_FlipColor();
    }

private:
    int *m_element = nullptr;
    Color m_color = Color::Red;

    RBTNode *m_left = nullptr;
    RBTNode *m_right = nullptr;
};