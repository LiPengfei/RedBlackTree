/* if find return. if not find, traval the tree until to leaf. In the
path if meet a 4-node tree, split it.
split method: up the middle value to its parent. parent can't be a
4-node because in the path we split erery node which is 4-node.
*/
template<typename Elem>
Elem *TTFTree<Elem>::Insert(const Elem &element) {
    Elem *searchResult = nullptr;
    bool found = _Search(element, &searchResult, nullptr);
    if (found) {
        return searchResult;
    }

    /* else not found */
    if (m_head == nullptr) {
        m_head = new TTFNode<Elem>(&element);

        return m_head->GetFirstElement();
    }

    else { // contains value
        TTFNode<Elem> *currentNode = m_head;
        TTFNode<Elem> *nextNode = m_head;
        bool found = false;

        found = _CheckForInsert(currentNode, element, nullptr, &nextNode);
        while (nextNode != nullptr) {
            assert(found == false);
            currentNode = nextNode;
            found = _CheckForInsert(currentNode, element, nullptr, &nextNode);
        }
        assert(currentNode->_IsLeaf() && !currentNode->Is4Node());

        return currentNode->Insert(element);
    }
}


template<typename Elem>
bool TTFTree<Elem>::Delete(const Elem &element) {
    Elem *retElem = nullptr;
    
    // modify : lipengfei 2015-06-28
    // here do some change, if found and is leaf, and is not 3/4node or head, delete.
    // bool found = _Search(element, &retElem, nullptr); changed to bellow
    TTFNode<Elem> *foundNode = nullptr;
    bool found = _Search(element, &retElem, &foundNode);
    if (found && foundNode->_IsLeaf()
        && (foundNode == m_head || foundNode->Is3Node() || foundNode->Is4Node())) {
        foundNode->Delete(element);
        return true;
    }

    // else traval with the invaiant: current node is not 2-node(except for head);
    if (found) {
        TTFNode<Elem> *currentNode = m_head;
        TTFNode<Elem> *nextNode = m_head;
        bool findInLeaf = false;
        Elem *retElem2 = nullptr;    // used to check

        while (nextNode != nullptr) {
            findInLeaf = _CheckForDelete(currentNode, element, &retElem2, &nextNode);
            if (findInLeaf) {
                assert(retElem == retElem2); // check 
                assert(!nextNode->Is2Node() || nextNode == m_head);

                nextNode->Delete(element);
                return true;
            }
            else if (!findInLeaf && retElem2 != nullptr) {// find but not in leaf
                assert(retElem == retElem2); // check 

                currentNode = _GetSuccessor(nextNode);

                // exchange and delete
                *retElem = *currentNode->GetFirstElement();
                delete currentNode->GetFirstElement();
                currentNode->SetFirstElement(currentNode->GetSecondElement());
                currentNode->SetSecondElement(currentNode->GetThirdElement());
                currentNode->SetThirdElement(nullptr);

                return true;
            }
            else { // not found
                currentNode = nextNode;
            }
        }
        assert(false);
        return false;
    }
    else {
        return false;
    }
}


template<typename Elem>
void TTFTree<Elem>::DebugPrint() {
    if (!IsEmpty()) {
        std::cout << "==========================================" << endl;
        std::queue<TTFNode<Elem> *> que;
        que.push(m_head);
        while (!que.empty()) {
            TTFNode<Elem> *node = que.front();
            que.pop();
            if (node->GetParent()) {
                std::cout << node->GetParent()->GetIndex() << ":";
            }
            std::cout << node->GetIndex() << "\t\t";
            if (node->HasFirstElement()) {
                cout << *node->GetFirstElement() << "\t";
            }
            if (node->HasSecondElement()) {
                cout << *node->GetSecondElement() << "\t";
            }
            if (node->HasThirdElement()) {
                cout << *node->GetThirdElement() << "\t";
            }

            if (node->HasFirstChild()) {
                que.push(node->GetFirstChild());
            }

            if (node->HasSecondChild()) {
                que.push(node->GetSecondChild());
            }

            if (node->HasThirdChild()) {
                que.push(node->GetThirdChild());
            }

            if (node->HasForthChild()) {
                que.push(node->GetForthChild());
            }
            std::cout << endl;
        }
    }
    else {
        std::cout << " empty " << endl;
    }

    system("pause");
}


template<typename Elem>
bool TTFTree<Elem>::_Search(const Elem &foundElement, Elem **retElem, TTFNode<Elem> **retNode) {
    if (IsEmpty()) {
        if (retElem) *retElem = nullptr;
        if (retNode) *retNode = nullptr;
        return false;
    }

    // m_head != nullptr
    TTFNode<Elem> *currentNode = m_head;
    TTFNode<Elem> *nextNode = m_head;
    bool found = false;
    Elem *inElement = nullptr;

    found = currentNode->CheckElement(foundElement, &inElement, &nextNode);
    while (found != true && nextNode != nullptr) {
        currentNode = nextNode;
        found = currentNode->CheckElement(foundElement, &inElement, &nextNode);
    }

    if (found) {
        if (retElem) *retElem = inElement;
        if (retNode) *retNode = currentNode;
        return true;
    }
    else {
        assert(currentNode->_IsLeaf()); // yes it is ? TODO  check.

        if (retElem) *retElem = nullptr;
        if (retNode) *retNode = nullptr;
        return false;
    }
}


template <typename Elem>
bool TTFTree<Elem>::_CheckForInsert(TTFNode<Elem> *node,
                                    const Elem &foundElement,
                                    Elem **inElement,
                                    TTFNode<Elem> **elementMayIn) {
    assert(node != nullptr);

    TTFNode<Elem> *currentNode = node;
    if (currentNode->Is4Node()) {
        _Split4Node(currentNode);
        // after split backward one step
        TTFNode<Elem> *parent = currentNode->GetParent();
        currentNode = parent ? parent : currentNode;
    }

    return currentNode->CheckElement(foundElement, inElement, elementMayIn);
}


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
template <typename Elem>
bool TTFTree<Elem>::_CheckForDelete(TTFNode<Elem> *node,           // see NOTE
                                    const Elem &foundElement,
                                    Elem **inElement,
                                    TTFNode<Elem> **elementInOrNextCheck) {

    assert(node != nullptr);

    /* modify 2-node to 3/4 node when we meet. */
    TTFNode<Elem> *currentNode = node;
    if (currentNode->GetParent() != nullptr && currentNode->Is2Node()) {
        currentNode = _Large2Node(currentNode);
    }

    bool found = currentNode->CheckElement(foundElement, inElement, elementInOrNextCheck);

    if (found && (*elementInOrNextCheck)->_IsLeaf()) { // found and is leaf.
        return true;
    }
    else if (found && !(*elementInOrNextCheck)->_IsLeaf()) { // found but not in leaf foward.
        assert(currentNode == *elementInOrNextCheck);

        /*
        forward set current to next check. and check if next borrow the foundElement.
        */
        if (*(currentNode->GetFirstElement()) == foundElement) {
            currentNode = currentNode->GetSecondChild();
        }
        else if (*(currentNode->GetSecondElement()) == foundElement) {
            currentNode = currentNode->GetThirdChild();
        }
        else if (currentNode->HasThirdElement()
                 && *(currentNode->GetThirdElement()) == foundElement) {
            currentNode = currentNode->GetForthChild();
        }
        else {
            assert(false && "can't find value in this node");
        }

        // 有没有可能目标值被下一个借走。
        if (currentNode->Is2Node()) {
            currentNode = _Large2Node(currentNode);
            bool borrow = currentNode->CheckElement(foundElement, nullptr, nullptr);
            if (borrow) {
                return _CheckForDelete(currentNode, foundElement, inElement, elementInOrNextCheck);
            }
            else {
                // return false, elementPointer, element's right child.
                *elementInOrNextCheck = currentNode;
                return false;
            }
        }
        else {
            *elementInOrNextCheck = currentNode;
            return false;
        }
    }
    else { // not found return false, nullptr, next check.
        return false;
    }
}


template <typename Elem>
TTFNode<Elem> *TTFTree<Elem>::_GetSuccessor(TTFNode<Elem> *node) {
    TTFNode<Elem> *currentNode = node;

    while (!currentNode->_IsLeaf()) {
        currentNode = currentNode->GetFirstChild();
        if (currentNode->Is2Node() && currentNode->GetParent() != nullptr) {
            currentNode = _Large2Node(currentNode);
        }
    }

    return currentNode;
}

template <typename Elem>
void TTFTree<Elem>::_Split4Node(TTFNode<Elem> *node) {
    assert(node != nullptr);

    Elem *firstElement = node->GetFirstElement();
    Elem *secondElement = node->GetSecondElement();
    Elem *thirdElement = node->GetThirdElement();

    TTFNode<Elem> *firstChild = node->GetFirstChild();
    TTFNode<Elem> *secondChild = node->GetSecondChild();
    TTFNode<Elem> *thirdChild = node->GetThirdChild();
    TTFNode<Elem> *forthChild = node->GetForthChild();

    TTFNode<Elem> *parent = node->GetParent();
    if (parent == nullptr) {
        //modify node
        node->SetFirstElement(secondElement);
        node->SetSecondElement(nullptr);
        node->SetThirdElement(nullptr);

        TTFNode<Elem> *newFirstChild = new TTFNode<Elem>();
        newFirstChild->SetFirstElement(firstElement);
        TTFNode<Elem> *newSecondChild = new TTFNode<Elem>();
        newSecondChild->SetFirstElement(thirdElement);

        node->SetFirstChild(newFirstChild);
        node->SetSecondChild(newSecondChild);
        node->SetThirdChild(nullptr);
        node->SetForthChild(nullptr);

        newFirstChild->SetFirstChild(firstChild);
        newFirstChild->SetSecondChild(secondChild);

        newSecondChild->SetFirstChild(thirdChild);
        newSecondChild->SetSecondChild(forthChild);

        assert(node->Is2Node() && newFirstChild->Is2Node() && newSecondChild->Is2Node());
    }
    else { //parent != nullptr
        // insert to parent
        parent->_Insert(secondElement);

        // modify node
        node->SetSecondElement(nullptr);
        node->SetThirdElement(nullptr);

        node->SetThirdChild(nullptr);
        node->SetForthChild(nullptr);

        TTFNode<Elem> *newChild = new TTFNode<Elem>();
        newChild->SetFirstElement(thirdElement);
        newChild->SetFirstChild(thirdChild);
        newChild->SetSecondChild(forthChild);

        if (node->IsFirstChild()) {
            parent->SetForthChild(parent->GetThirdChild());
            parent->SetThirdChild(parent->GetSecondChild());
            parent->SetSecondChild(newChild);
        }
        else if (node->IsSecondChild()) {
            parent->SetForthChild(parent->GetThirdChild());
            parent->SetThirdChild(newChild);
        }
        else if (node->IsThirdChild()) {
            parent->SetForthChild(newChild);
        }
        else {
            assert(false); // 不变量： 分裂结点的父节点不可能为4-node
        }
        assert(parent->Is3Node() || parent->Is4Node());
        assert(node->Is2Node() && newChild->Is2Node());
    }
}


template <typename Elem>
TTFNode<Elem> *TTFTree<Elem>::_Large2Node(TTFNode<Elem> *node) {
    assert(node->Is2Node());
    TTFNode<Elem> *currentNode = node;

    if (_BorrowFromLeft(&currentNode) || _BorrowFromRight(&currentNode)) { // IMPORTANT: may delete node here 
        ;
    }
    else {
        assert(false && "both borrow fails");
    }

    assert(currentNode != nullptr);
    return currentNode;
}



template <typename Elem>
bool TTFTree<Elem>::_BorrowFromLeft(TTFNode<Elem> **node) {
    assert(node != nullptr);

    TTFNode<Elem> *currentNode = *node;
    TTFNode<Elem> *left = currentNode->GetLeftSibling();
    if (left != nullptr) {
        if (left->Is2Node()) {
            TTFNode<Elem> *parent = currentNode->GetParent();
            assert(parent != nullptr);

            if (parent->Is2Node()) { // left is 2-node and parent is 2-node
                assert(parent->GetParent() == nullptr);

                parent->_UpLeftRightChild(left, currentNode);
                *node = parent;     // backaward node;

                return true;
            } // left is 2-node and parent is 2-node end.
            else if (parent->Is3Node() || parent->Is4Node()) { // left is 2-node, parent is 3/4 node.
                currentNode->_CombineLeft();
                return true;
            }
            else { // parent is not 2/3/4 node.
                assert(false && "parent is not nullptr but is a empty ?");
                return false;
            }
        } // left.Is2Node() end
        else if (left->Is3Node() || left->Is4Node()) {
            currentNode->_RightRotate();
            return true;
        }
        else {
            assert(false && "left_sibling not nullptr but is a empty ?");
            return false;
        }
    } // left != nullptr end
    else { // left == nullptr
        assert(currentNode->IsFirstChild());
        return false;
    }

}


template <typename Elem>
bool TTFTree<Elem>::_BorrowFromRight(TTFNode<Elem> **node) {
    assert(node != nullptr);

    TTFNode<Elem> *currentNode = *node;
    TTFNode<Elem> *right = currentNode->GetRightSibling();
    if (right != nullptr) {
        if (right->Is2Node()) {
            TTFNode<Elem> *parent = currentNode->GetParent();
            assert(parent != nullptr);

            if (parent->Is2Node()) { // left is 2-node, parent is 2-node
                assert(parent->GetParent() == nullptr);

                parent->_UpLeftRightChild(currentNode, right);
                *node = parent;     // backaward node;

                return true;
            } // left is 2-node and parent is 2-node end.

            else if (parent->Is3Node() || parent->Is4Node()) { // left is 2-node, parent is 3/4 node.
                currentNode->_CombineRight();
                return true;
            }
            else { // parent is not 2/3/4 node.
                assert(false && "parent is not nullptr but is a empty ?");
                return false;
            }
        } // left.Is2Node() end
        else if (right->Is3Node() || right->Is4Node()) {
            currentNode->_LeftRotate();
            return true;
        }
        else {
            assert(false && "left_sibling not nullptr but is a empty ?");
            return false;
        }
    } // left != nullptr end
    else { // left == nullptr
        assert(!currentNode->IsForthChild());
        return false;
    }
}

