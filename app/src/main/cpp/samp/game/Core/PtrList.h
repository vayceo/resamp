/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "PtrNode.h"

class CPtrList {
public:
    CPtrNode* m_node;

public:
    static void InjectHooks();

    CPtrList();

    uint32 CountElements() const;
    bool IsMemberOfList(void* data) const;

    CPtrNode* GetNode() const { return m_node; }
    bool IsEmpty() const { return !m_node; }
};

VALIDATE_SIZE(CPtrList, (VER_x32 ? 0x4 : 0x8));
