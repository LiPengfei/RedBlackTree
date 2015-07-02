#pragma once

#include "../DataStruct/RBTreeNode.h"
#include <iostream>
#include <queue>
using namespace std;

class RBTree {
public:
    RBTree() { }
    ~RBTree() { delete m_head; }

    RBTree(const RBTree &rhs) = delete;
    RBTree & operator= (const RBTree &rhs) = delete;

public:
    void DeleteMin() {
        m_head = RBTNode::DeleteMin(m_head);
        if (m_head) m_head->m_color = RBTNode::Color::Black;
    }


    void DeleteMax() {
        m_head = RBTNode::DeleteMax(m_head);
        if (m_head) m_head->m_color = RBTNode::Color::Black;
    }


    void Insert(const int &val) {
        m_head = RBTNode::Insert(m_head, val);
        m_head->m_color = RBTNode::Color::Black;
    }

    void Delete(const int &val) {
        m_head = RBTNode::Delete(m_head, val);
        if (m_head) m_head->m_color = RBTNode::Color::Black;
    }

    void Print() {
        cout << "=========================================" << endl;
        queue<RBTNode*> a;
        if (m_head) a.push(m_head);
        while (!a.empty()) {
            RBTNode *node = a.front();
            a.pop();
            cout << (unsigned int)node->m_color << "\t" << node->GetValue() << endl;
            if (node->m_left) a.push(node->m_left);
            if (node->m_right) a.push(node->m_right);
        }
    }

private:
    RBTNode *m_head = nullptr;
};

