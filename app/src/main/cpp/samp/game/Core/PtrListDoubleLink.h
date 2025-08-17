/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "PtrList.h"
#include "PtrNodeDoubleLink.h"

class CPtrListDoubleLink : public CPtrList {
public:
    CPtrListDoubleLink() { m_node->m_item = nullptr; }
    ~CPtrListDoubleLink() { Flush(); }

    static void InjectHooks();

    void Flush();
    CPtrNodeDoubleLink* AddItem(void* item);
    void DeleteItem(void* item);
    void DeleteNode(CPtrNodeDoubleLink* node); //Most likely inlined in the final exe, this code is used in multiple places

    CPtrNodeDoubleLink* GetNode() const { return reinterpret_cast<CPtrNodeDoubleLink*>(m_node); }
};

VALIDATE_SIZE(CPtrListDoubleLink, (VER_x32 ? 0x4 : 0x8));
