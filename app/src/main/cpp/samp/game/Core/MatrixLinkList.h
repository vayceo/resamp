#pragma once

#include "MatrixLink.h"
#include "../common.h"

class CMatrixLinkList {
public:
    CMatrixLinkList();

public:
    CMatrixLink  m_head;
    CMatrixLink  m_tail;
    CMatrixLink  m_allocatedListHead;
    CMatrixLink  m_allocatedListTail;
    CMatrixLink  m_freeListHead;
    CMatrixLink  m_freeListTail;
    CMatrixLink* m_pObjects;

public:
    static void InjectHooks();

public:
    void            Init(int32 count);
    void            Shutdown();
    CMatrixLink*    AddToList1();
    CMatrixLink*    AddToList2();
    void            MoveToList1(CMatrixLink* mat);
    void            MoveToList2(CMatrixLink* mat);
    void            MoveToFreeList(CMatrixLink* mat);
    int32           GetNumFree();
    int32           GetNumUsed1();
    int32           GetNumUsed2();

public:
    inline bool IsFull() { return m_freeListHead.m_pNext == &m_freeListTail; }
    inline CMatrixLink* GetOldestLink() { return m_tail.m_pPrev; }
};

VALIDATE_SIZE(CMatrixLinkList, (VER_x32 ? 0x1FC : 0x278));

extern CMatrixLinkList gMatrixList;
