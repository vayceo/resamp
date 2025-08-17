#include "PtrList.h"

void CPtrList::InjectHooks() {

}

CPtrList::CPtrList() {
    m_node = nullptr;
}

// Get elements count
// 0x5521B0
uint32 CPtrList::CountElements() const {
    uint32 counter;
    CPtrNode* currNode = GetNode();
    for (counter = 0; currNode; ++counter) {
        currNode = currNode->GetNext();
    }
    return counter;
}

// Check if data is a member of list
// 0x5521D0
bool CPtrList::IsMemberOfList(void* data) const {
    CPtrNode* currNode = GetNode();
    while (currNode) {
        if (currNode->m_item == data) {
            return true;
        }
        currNode = currNode->m_next;
    }
    return false;
}
