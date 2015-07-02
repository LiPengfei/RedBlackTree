#pragma once
#include "TTFNode.h"

#include <iostream>
#include <queue>

template<typename Elem>
class TTFTree {
public:
    TTFTree() { };

    ~TTFTree() {
        delete m_head;
        m_head = nullptr;
    };

public:
    Elem *Search(const Elem &foundElement) {
        Elem *retElem = nullptr;
        _Search(foundElement, foundElement, nullptr);
        return retElem;
    }

    /*---------------------------------------------------------------------
    return element in the tree. if found return the found else create a
    new one return point to it.

    ... param: element   in:need to insert
    ---------------------------------------------------------------------*/
	Elem *Insert(const Elem &element);

	bool Delete(const Elem &element);
//     bool DeleteMin();
//     bool DeleteMax();

public:
    bool IsEmpty() {
        return m_head == nullptr || m_head->IsEmpty();
    }

    void DebugPrint();

private:
    /*---------------------------------------------------------------------
    helper function for Insert and Search

    return true if found the element, retElem point to the found target.
    retNode set to point to the node contain the element.

    ... param: foundElement   in: need to search
    ... param: retElem        inout: in nullptr do nothing else set to point
    to the element in tree.
    ... param: retNode        inout: in nullptr do nothing else set to node
    contains the element.
    ---------------------------------------------------------------------*/
    bool _Search(const Elem &foundElement, Elem **retElem, TTFNode<Elem> **retNode);

private:

    //when check for insert. if we meet 4-node, split it.
    static bool _CheckForInsert(TTFNode<Elem> *node,
                               const Elem &foundElement,
                               Elem **inElement,
                               TTFNode<Elem> **ElementMayIn);

    static void _Split4Node(TTFNode<Elem> *node);


    // when check for insert. if we meet 2-node(expect for 独子结点), change it to 3/4 node.
    // method : if sibling is not 2-node, rotate it closest value up, parent down. then 2->3 node.
    //          if sibling is 2-node, parent is not 2-node (对于任何非独子结点，这是不变量）down. 
    //          sibling + parent + self -> 4 node.
    //          if sibling is 2-node, parent is 2-node(only when parent is head). combine them 
    //          to parent.
    // NOTE: node may point to a invaild postion after this function call.
    //
    // reutrn ture if found and it is in leaf node
    // return false if not found, and *inElement = nullptr
    // return false if found but not in leaf node, *elementInOrNextcheck prepare to find successor.
    static bool _CheckForDelete(TTFNode<Elem> *node,
                               const Elem &foundElement,
                               Elem **inElement,
                               TTFNode<Elem> **ElementMayIn);

    static TTFNode<Elem> *_Large2Node(TTFNode<Elem> *node);

    static bool _BorrowFromRight(TTFNode<Elem> **node);

    static bool _BorrowFromLeft(TTFNode<Elem> **node);

    static TTFNode<Elem> *_GetSuccessor(TTFNode<Elem> *node);


private:
    TTFNode<Elem> *m_head = nullptr;
};


#include "TTFTree.cpp"

