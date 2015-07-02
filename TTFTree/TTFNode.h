#pragma once
#include "../Common/common.h"

template<typename Elem> class TTFTree;

template <typename Elem>
class TTFNode {

    friend class TTFTree<Elem>;

private:
    enum ElementIndex : unsigned int {
        FIRST_ELEMENT,
        SECOND_ELEMENT,
        THIRD_ELEMENT,
    };

    enum ChildIndex : unsigned int {
        FIRST_CHILD,
        SECOND_CHILD,
        THIRD_CHILD,
        FORTH_CHILD,
    };

private:
    TTFNode(const Elem *element) : m_firstElement(new Elem(*element)), m_index(ms_index++) { }
    TTFNode() : m_index(ms_index++) { }
    ~TTFNode();

private:
    Elem *Insert(const Elem &element){

        Elem *newElement = new Elem(element);
        _Insert(newElement);
        return newElement;
    }

    void Delete(const Elem &element) {
        _Delete(element);
    }
    
    // check the foundElement in this node. If found, return true, the (Element) inElement and the
    // (Node)elementMayIn the foundElement In. else return false, the (Element) nullptr, elementMayIn
    // (the nextNode need to check).
    bool CheckElement(const Elem &foundElement,
                         Elem **inElement,
                         TTFNode<Elem> **ElementMayIn);

private:
    bool IsEmpty() const {
        if (m_firstElement == nullptr
            && m_secondElement == nullptr
            && m_thirdElement == nullptr) {

            assert(m_firstChild == nullptr
                   && m_secondChild == nullptr
                   && m_thirdChild == nullptr
                   && m_forthChild == nullptr);

            return true;
        }
        return false;
    }

    bool Is2Node() const {
        if (m_firstElement != nullptr
            && m_secondElement == nullptr
            && m_thirdElement == nullptr) {

            assert(m_thirdChild == nullptr
                   && m_forthChild == nullptr);

            return true;
        }
        return false;
    }

    bool Is3Node() const {
        if (m_firstElement != nullptr
            && m_secondElement != nullptr
            && m_thirdElement == nullptr) {

            assert(m_forthChild == nullptr);

            return true;
        }
        return false;
    }

    bool Is4Node() const {
        if (m_firstElement != nullptr
            && m_secondElement != nullptr
            && m_thirdElement != nullptr) {

            return true;
        }
        return false;
    }

    bool _IsLeaf() const {
        if (m_firstChild == nullptr
            && m_secondChild == nullptr
            && m_thirdChild == nullptr
            && m_forthChild == nullptr
            && !IsEmpty()) {
            return true;
        }
        return false;
    }

    bool IsFirstChild() const{
        if (m_parent != nullptr) {
            return m_parent->m_firstChild == this;
        }

        return false;
    }

    bool IsSecondChild() const {
        if (m_parent != nullptr) {
            return m_parent->m_secondChild == this;
        }

        return false;
    }

    bool IsThirdChild() const {
        if (m_parent != nullptr) {
            return m_parent->m_thirdChild == this;
        }

        return false;
    }

    bool IsForthChild() const {
        if (m_parent != nullptr) {
            return m_parent->m_forthChild == this;
        }

        return false;
    }

private:
    bool HasFirstChild() const {
        return m_firstChild != nullptr;
    }
    
    bool HasSecondChild() const {
        return m_secondChild != nullptr;
    }

    bool HasThirdChild() const {
        return m_thirdChild != nullptr;
    }

    bool HasForthChild() const {
        return m_forthChild != nullptr;
    }

    bool HasFirstElement() const {
        return m_firstElement != nullptr;
    }

    bool HasSecondElement() const {
        return m_secondElement != nullptr;
    }

    bool HasThirdElement() const {
        return m_thirdElement != nullptr;
    }

private:
    // element get
    Elem *GetFirstElement() const { return m_firstElement; }
    Elem *GetSecondElement() const { return m_secondElement; }
    Elem *GetThirdElement() const { return m_thirdElement; }
    void SetFirstElement(Elem *val) { m_firstElement = val; }
    void SetSecondElement(Elem *val) { m_secondElement = val; }
    void SetThirdElement(Elem *val) { m_thirdElement = val; }

    // child get
    TTFNode<Elem> *GetFirstChild() const { return m_firstChild; }
    TTFNode<Elem> *GetSecondChild() const { return m_secondChild; }
    TTFNode<Elem> *GetThirdChild() const { return m_thirdChild; }
    TTFNode<Elem> *GetForthChild() const { return m_forthChild; }
    void SetFirstChild(TTFNode<Elem> *val) {
        m_firstChild = val;
        if (val != nullptr) val->m_parent = this;
    }
    void SetSecondChild(TTFNode<Elem> *val) {
        m_secondChild = val;
        if (val != nullptr) val->m_parent = this;
    }
    void SetThirdChild(TTFNode<Elem> *val) {
        m_thirdChild = val;
        if (val != nullptr) val->m_parent = this;
    }
    void SetForthChild(TTFNode<Elem> *val) {
        m_forthChild = val;
        if (val != nullptr) val->m_parent = this;
    }

    // sibling get;
    TTFNode<Elem> *GetLeftSibling() const;
    TTFNode<Elem> *GetRightSibling() const;

    // parent only get
    TTFNode<Elem> *GetParent() const { return m_parent; }
    unsigned int GetIndex() const { return m_index; }

private:
    void _Insert(Elem *element);
    void _Delete(const Elem &element);

    void _UpLeftRightChild(TTFNode<Elem> *leftChild, TTFNode<Elem> *rightChild);
    void _DownToChild(ElementIndex element_index, TTFNode<Elem> *node);

    void _2ToEmpty();

    void _CombineLeft();
    void _CombineRight();

    void _LeftRotate();
    void _RightRotate();
    
private:
    Elem *m_firstElement = nullptr;
    Elem *m_secondElement = nullptr;
    Elem *m_thirdElement = nullptr;

    TTFNode<Elem> *m_firstChild = nullptr;
    TTFNode<Elem> *m_secondChild = nullptr;
    TTFNode<Elem> *m_thirdChild = nullptr;
    TTFNode<Elem> *m_forthChild = nullptr;

    TTFNode<Elem> *m_parent = nullptr;
    unsigned int m_index = 0;
    static unsigned int ms_index;

};


#include "TTFNode.cpp" 

