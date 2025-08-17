#include "AnimBlendClumpData.h"
#include "game/MemoryMgr.h"

void CAnimBlendClumpData::InjectHooks() {

}

// 0x4CF0E0
CAnimBlendClumpData::CAnimBlendClumpData() {
    m_NumFrames   = 0;
    m_PedPosition = nullptr;
    m_Frames      = nullptr;
}

// 0x4CF100
CAnimBlendClumpData::~CAnimBlendClumpData() {
    m_Associations.Remove();
    if (m_Frames) {
        CMemoryMgr::FreeAlign(&m_Frames);
    }
}

// 0x4CF140
void CAnimBlendClumpData::SetNumberOfBones(int32 numBones) {
    if (m_Frames) {
        CMemoryMgr::FreeAlign(&m_Frames);
    }
    m_NumFrames = numBones;
    m_Frames = reinterpret_cast<AnimBlendFrameData*>(CMemoryMgr::MallocAlign(sizeof(AnimBlendFrameData) * numBones, 64));
}

// 0x4CF190
void CAnimBlendClumpData::ForAllFrames(void (*callback)(AnimBlendFrameData*, void*), void* data) {
    for (auto& frame : std::span{ m_Frames, m_NumFrames }) {
        callback(&frame, data);
    }
}

// 0x4CF1D0
void CAnimBlendClumpData::LoadFramesIntoSPR() {
    // NOP
}

// 0x4CF1E0
void CAnimBlendClumpData::ForAllFramesInSPR(void (*callback)(AnimBlendFrameData*, void*), void* data, uint32 a3) {
    // NOP
}
