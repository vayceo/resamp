
#include "ColLine.h"

void CColLine::InjectHooks() {

}

// 0x40EF50
CColLine::CColLine(const CVector& start, const CVector& end) {
    m_vecStart = start;
    m_vecEnd = end;
}

// 0x40EF10
void CColLine::Set(const CVector& start, const CVector& end) {
    m_vecStart = start;
    m_vecEnd = end;
}

// NOTSA
float CColLine::DistTo(CVector pt) const {
   // return CCollision::DistToLine(m_vecStart, m_vecEnd, pt);
}

// NOTSA
void CColLine::DrawWireFrame(CRGBA color, const CMatrix& transform) const {
//    const auto colorARGB = color.ToInt();
//    CLines::RenderLineNoClipping(transform * m_vecStart, transform * m_vecEnd, colorARGB, colorARGB);
}
