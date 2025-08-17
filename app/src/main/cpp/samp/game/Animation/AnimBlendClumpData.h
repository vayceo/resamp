/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "AnimBlendFrameData.h"
#include "AnimBlendAssociation.h"

class CAnimBlendClumpData {
public:
    CAnimBlendLink      m_Associations;
    uint32              m_NumFrames;
    CVector*            m_PedPosition;
    AnimBlendFrameData* m_Frames;

public:
    static void InjectHooks();

    CAnimBlendClumpData();
    ~CAnimBlendClumpData();
    CAnimBlendClumpData* Constructor() { CAnimBlendClumpData(); return this; }
    CAnimBlendClumpData* Destructor()  { this->CAnimBlendClumpData::~CAnimBlendClumpData(); return this; }

    void ForAllFrames(void (*callback)(AnimBlendFrameData*, void*), void* data);
    void ForAllFramesInSPR(void (*callback)(AnimBlendFrameData*, void*), void* data, uint32 a3);
    void LoadFramesIntoSPR();
    void SetNumberOfBones(int32 numBones);
};

VALIDATE_SIZE(CAnimBlendClumpData, (VER_x32 ? 0x14 : 0x28));
