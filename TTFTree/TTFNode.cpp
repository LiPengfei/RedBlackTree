template <typename Elem>
unsigned int TTFNode<Elem>::ms_index = 0;


template <typename Elem>
TTFNode<Elem>::~TTFNode() {
    delete m_firstChild;
    m_firstChild = nullptr;
    delete m_secondChild;
    m_secondChild = nullptr;
    delete m_thirdChild;
    m_thirdChild = nullptr;
    delete m_forthChild;
    m_forthChild = nullptr;

    delete m_firstElement;
    m_firstElement = nullptr;
    delete m_secondElement;
    m_secondElement = nullptr;
    delete m_thirdElement;
    m_thirdElement = nullptr;
}


template <typename Elem>
TTFNode<Elem> *TTFNode<Elem>::GetLeftSibling() const {
    if (IsFirstChild()) {
        return nullptr;
    }
    else if (IsSecondChild()) {
        return GetParent()->GetFirstChild();
    }
    else if (IsThirdChild()) {
        return GetParent()->GetSecondChild();
    }
    else if (IsForthChild()) {
        return GetParent()->GetThirdChild();
    }
    else {
        assert(false);
        return nullptr;
    }
}


template <typename Elem>
TTFNode<Elem> *TTFNode<Elem>::GetRightSibling() const {
    if (IsFirstChild()) {
        return GetParent()->GetSecondChild();
    }
    else if (IsSecondChild()) {
        return GetParent()->GetThirdChild();
    }
    else if (IsThirdChild()) {
        return GetParent()->GetForthChild();
    }
    else if (IsForthChild()) {
        return nullptr;
    }
    else {
        assert(false);
        return nullptr;
    }
}


// check the foundElement in this node. If found, return true, the (Element) inElement and the
// (Node)elementMayIn the foundElement In. else return false, the (Element) nullptr, elementMayIn
// (the nextNode need to check).
template <typename Elem>
bool TTFNode<Elem>::CheckElement(const Elem &foundElement,
                                 Elem **inElement,
                                 TTFNode<Elem> **elementMayIn) {
    if (!IsEmpty()) {
        // check first element start
        Elem *firstElement = GetFirstElement();
        if (foundElement < *firstElement) {
            if (inElement != nullptr) *inElement = nullptr;
            if (elementMayIn != nullptr) *elementMayIn = GetFirstChild();
            return false;
        }
        else if (foundElement == *firstElement) {
            if (inElement != nullptr) *inElement = firstElement;
            if (elementMayIn != nullptr) *elementMayIn = const_cast<TTFNode<Elem> *>(this);
            return true;
        } // check first element end
        else {

            /* if (has second element and less than it) or has no second element maybe in second child
            if has second element and equal, found
            if has second element and bigger, check third element.
            */
            // check second element start. 
            Elem *secondElement = GetSecondElement();
            if ((HasSecondElement() && foundElement < *secondElement)
                || !HasSecondElement()) {
                if (inElement != nullptr) *inElement = nullptr;
                if (elementMayIn != nullptr) *elementMayIn = GetSecondChild();
                return false;
            }
            else if (HasSecondElement() && foundElement == *secondElement) {
                if (inElement != nullptr) *inElement = secondElement;
                if (elementMayIn != nullptr) *elementMayIn = this;
                return true;
            } // check second element end.
            else if (HasSecondElement() && foundElement > *secondElement) {

                // check third element start
                Elem *thridElement = GetThirdElement();
                if ((HasThirdElement() && foundElement < *thridElement)
                    || !HasThirdElement()) {
                    if (inElement != nullptr) *inElement = nullptr;
                    if (elementMayIn != nullptr) *elementMayIn = GetThirdChild();
                    return false;
                }
                else if (HasThirdElement() && foundElement == *thridElement) {
                    if (inElement != nullptr) *inElement = thridElement;
                    if (elementMayIn != nullptr) *elementMayIn = this;
                    return true;
                }
                else if (HasThirdElement() && foundElement > *thridElement) {
                    if (inElement != nullptr) *inElement = nullptr;
                    if (elementMayIn != nullptr) *elementMayIn = GetForthChild();
                    return false;
                } // check third element end
                else {
                    assert(false);
                    return false;
                }
            }
            else {
                assert(false);
                return false;
            }
        }
    }
    else {
        assert(false);
        return false;
    }
}


template <typename Elem>
void TTFNode<Elem>::_Insert(Elem *element) {
    assert(element != nullptr);

    if (IsEmpty()) {
        m_firstElement = element;
    }
    else if (Is2Node()) {
        Elem *firstElement = GetFirstElement();
        if (*element < *firstElement) {
            m_secondElement = firstElement;
            m_firstElement = element;
        }
        else if (*element > *firstElement) {
            m_secondElement = element;
        }
        else {
            assert(false && "_insert23Node equal value");
        }
    }
    else if (Is3Node()) {
        Elem *firstElement = GetFirstElement();
        Elem *secondElement = GetSecondElement();

        if (*element < *firstElement) {
            m_firstElement = element;
            m_secondElement = firstElement;
            m_thirdElement = secondElement;
        }
        else if (*element > *firstElement) {
            if (*element < *secondElement) {
                m_secondElement = element;
                m_thirdElement = secondElement;
            }
            else if (*element > *secondElement) {
                m_thirdElement = element;
            }
            else {
                assert(false && "insert equal value");
                return;
            }
        }
        else {
            assert(false && "insert equal value");
            return;
        }
    }
    else {
        assert(false && "insert 4-node");
        return;
    }
}


template <typename Elem>
void TTFNode<Elem>::_Delete(const Elem &element) {
	assert(_IsLeaf());

    if (*GetFirstElement() == element) {
        delete GetFirstElement();

        SetFirstElement(GetSecondElement());
        SetSecondElement(GetThirdElement());
        SetThirdElement(nullptr);
    }
    else if (HasSecondElement() && *GetSecondElement() == element) {
        delete GetSecondElement();

        SetSecondElement(GetThirdElement());
        SetThirdElement(nullptr);
    }
    else if (HasThirdElement() && *GetThirdElement() == element) {
        delete GetThirdElement();

        SetThirdElement(nullptr);
    }
    else {
        assert(false && "can't find value in this node");
    }
}


template <typename Elem>
void TTFNode<Elem>::_UpLeftRightChild(TTFNode<Elem> *leftChild, TTFNode<Elem> *rightChild) {
    assert(GetParent() == nullptr && Is2Node());
    assert(leftChild->GetParent() == rightChild->GetParent() && leftChild->GetParent() == this);

    SetSecondElement(GetFirstElement());
    SetFirstElement(leftChild->GetFirstElement());
    SetThirdElement(rightChild->GetFirstElement());

    SetFirstChild(leftChild->GetFirstChild());
    SetSecondChild(leftChild->GetSecondChild());
    SetThirdChild(rightChild->GetFirstChild());
    SetForthChild(rightChild->GetSecondChild());

    leftChild->_2ToEmpty();
    rightChild->_2ToEmpty();

    assert(leftChild->IsEmpty() && rightChild->IsEmpty());
    delete leftChild;
    delete rightChild;
}


template <typename Elem>
void TTFNode<Elem>::_DownToChild(ElementIndex element_index, TTFNode<Elem> *node) {
    Elem *forRet = nullptr;
    switch (element_index) {
    case ElementIndex::FIRST_ELEMENT:
        SetFirstElement(GetSecondElement());
        SetSecondElement(GetThirdElement());

        SetFirstChild(node);
        SetSecondChild(GetThirdChild());
        SetThirdChild(GetForthChild());
        break;
    case ElementIndex::SECOND_ELEMENT:
        SetSecondElement(GetThirdElement());

        SetSecondChild(node);
        SetThirdChild(GetForthChild());
        break;
    case ElementIndex::THIRD_ELEMENT:
        SetThirdChild(node);
        break;
    default:
        assert(false && "ElementIndex param error");
        break;
    }

    SetThirdElement(nullptr);
    SetForthChild(nullptr);
}


template <typename Elem>
void TTFNode<Elem>::_2ToEmpty() {
    assert(Is2Node());

    SetFirstElement(nullptr);

    SetFirstChild(nullptr);
    SetSecondChild(nullptr);
}


template <typename Elem>
void TTFNode<Elem>::_CombineLeft() {
    TTFNode<Elem> *left = GetLeftSibling();
    Elem *borrowParent = nullptr;

    /* set parent. */
    TTFNode<Elem> *parent = GetParent();
    if (IsSecondChild()) {
        borrowParent = parent->GetFirstElement();
        parent->_DownToChild(FIRST_ELEMENT, this);
    }
    else if (IsThirdChild()) {
        borrowParent = parent->GetSecondElement();
        parent->_DownToChild(SECOND_ELEMENT, this);
    }
    else if (IsForthChild()) {
        borrowParent = parent->GetThirdElement();
        parent->_DownToChild(THIRD_ELEMENT, this);
    }

    SetThirdElement(GetFirstElement());
    SetSecondElement(borrowParent);
    SetFirstElement(left->GetFirstElement());

    // order can't ajust !!!
    SetThirdChild(GetFirstChild());
    SetForthChild(GetSecondChild());
    SetFirstChild(left->GetFirstChild());
    SetSecondChild(left->GetSecondChild());

    left->_2ToEmpty();
    assert(left->IsEmpty());
    delete left;
}


template <typename Elem>
void TTFNode<Elem>::_CombineRight() {
    TTFNode<Elem> *right = GetRightSibling();
    Elem *borrowParent = nullptr;

    /* set parent. */
    TTFNode<Elem> *parent = GetParent();
    if (IsFirstChild()) {
        parent->_DownToChild(FIRST_ELEMENT, this);
        borrowParent = parent->GetFirstElement();
    }
    else if (IsSecondChild()) {
        parent->_DownToChild(SECOND_ELEMENT, this);
        borrowParent = parent->GetSecondElement();
    }
    else if (IsThirdChild()) {
        parent->_DownToChild(THIRD_ELEMENT, this);
        borrowParent = parent->GetThirdElement();
    }

    SetSecondElement(borrowParent);
    SetThirdElement(right->GetFirstElement());

    SetThirdChild(right->GetFirstChild());
    SetForthChild(right->GetSecondChild());

    right->_2ToEmpty();
    assert(right->IsEmpty());
    delete right;
}


template <typename Elem>
void TTFNode<Elem>::_LeftRotate() {
    TTFNode<Elem> *right = GetRightSibling();
    Elem *needUpElem = right->GetFirstElement();
    TTFNode<Elem> *needMoveChild = right->GetFirstChild();

    /* set right */
    right->SetFirstElement(right->GetSecondElement());
    right->SetSecondElement(right->GetThirdElement());
    right->SetThirdElement(nullptr);

    right->SetFirstChild(right->GetSecondChild());
    right->SetSecondChild(right->GetThirdChild());
    right->SetThirdChild(right->GetForthChild());
    right->SetForthChild(nullptr);

    /* set parent */
    TTFNode<Elem> *parent = GetParent();
    Elem *needDownElem = nullptr;
    if (IsFirstChild()) {
        needDownElem = parent->GetFirstElement();
        parent->SetFirstElement(needUpElem);
    }
    else if (IsSecondChild()) {
        needDownElem = parent->GetSecondElement();
        parent->SetSecondElement(needUpElem);
    }
    else if (IsThirdChild()) {
        needDownElem = parent->GetThirdElement();
        parent->SetThirdElement(needUpElem);
    }
    else {
        assert(false && "only 1/2/3 child can _LeftRotate");
        return;
    }

    /* set self*/
    assert(Is2Node());
    SetSecondElement(needDownElem);
    SetThirdChild(needMoveChild);
}


template <typename Elem>
void TTFNode<Elem>::_RightRotate() {
    TTFNode<Elem> *left = GetLeftSibling();
    Elem *needUpElem = nullptr;
    TTFNode<Elem> *needMoveChild = nullptr;

    /* set left */
    if (left->Is3Node()) {
        needUpElem = left->GetSecondElement();
        needMoveChild = left->GetThirdChild();

        left->SetSecondElement(nullptr);
        left->SetThirdChild(nullptr);
    }
    else if (left->Is4Node()) {
        needUpElem = left->GetThirdElement();
        needMoveChild = left->GetForthChild();

        left->SetThirdElement(nullptr);
        left->SetForthChild(nullptr);
    }
    else {
        assert(false && "only left is 3/4 Node can _LeftRotate");
        return;
    }

    /* set parent */
    TTFNode<Elem> *parent = GetParent();
    Elem *needDownElem = nullptr;
    if (IsSecondChild()) {
        needDownElem = parent->GetFirstElement();
        parent->SetFirstElement(needUpElem);
    }
    else if (IsThirdChild()) {
        needDownElem = parent->GetSecondElement();
        parent->SetSecondElement(needUpElem);
    }
    else if (IsForthChild()) {
        needDownElem = parent->GetThirdElement();
        parent->SetThirdElement(needUpElem);
    }
    else {
        assert(false && "only 2/3/4 child can _LeftRotate");
        return;
    }

    /* set self*/
    SetSecondElement(GetFirstElement());
    SetFirstElement(needDownElem);

    SetThirdChild(GetSecondChild());
    SetSecondChild(GetFirstChild());
    SetFirstChild(needMoveChild);
}