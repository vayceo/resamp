/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "../common.h"
#include "../rgba.h"

class CSphere {
public:
    CVector m_vecCenter{};
    float   m_fRadius{};

    constexpr CSphere() = default;
    constexpr CSphere(CVector center, float radius) : m_vecCenter(center), m_fRadius(radius) {}
    [[deprecated]]
    constexpr CSphere(float radius, CVector center) : m_vecCenter(center), m_fRadius(radius) {}

    void Set(float radius, const CVector& center);

    //! Check if point is inside (within) of `*this`
    bool IsPointWithin(const CVector& p) const;

    /*!
    * @addr notsa
    * @brief Render the sphere in the 3D world (Be sure to call from a place where 3D stuff is rendered, if called from elsewhere you won't see the thing!)
    *
    * @param transform Transformation matrix to be used
    * @param color     Color of the lines used
    */
    void DrawWireFrame(CRGBA color, const CMatrix& transform) const;

    //! Get *this but transformed
    auto GetTransformed(const CMatrix& transform) const -> CSphere;

    friend auto TransformObject(const CSphere& sp, const CMatrix& transform) -> CSphere;
};
VALIDATE_SIZE(CSphere, 0x10);
