/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "../Enums/eSurfaceType.h"
#include "ColPoint.h"
#include "../CompressedVector.h"
#include "game/Core/Rect.h"
#include "game/StoredCollPoly.h"
#include "ColTrianglePlane.h"
#include "../rgba.h"

class CColTriangle {
public:
    CColTriangle() = default;
    CColTriangle(int32 a, int32 b, int32 c, eSurfaceType material, tColLighting light) :
            vA(a),
            vB(b),
            vC(c),
            m_nMaterial(material),
            m_nLight(light)
    {
    }

    /*!
    * @addr notsa
    * @brief Render the triangle in the 3D world (Be sure to call from a place where 3D stuff is rendered, if called from elsewhere you won't see the lines!)
    *
    * @param color     Color of the lines used
    * @param vertices  Pointer to the array of vertices where this triangles vertices are (CColData::m_pVertices usually)
    * @param transform Transformation matrix to be used
    */
    void DrawWireFrame(CRGBA color, const CollisionVector* vertices, const CMatrix& transform) const;
    auto GetPlane(const CollisionVector* vertices) const -> CColTrianglePlane;
    auto GetPoly(const CollisionVector* verts) const -> CStoredCollPoly;

    static auto GetBoundingRect(const CVector& a, const CVector& b, const CVector& c) -> CRect;

    auto GetSurfaceType() const { return m_nMaterial; }
public:
    union {
        struct {
            int32 vA; // vertex index in vertices array
            int32 vB; // vertex index in vertices array
            int32 vC; // vertex index in vertices array
        };
        int32 m_vertIndices[3];
    };
    eSurfaceType m_nMaterial;
    tColLighting m_nLight;
};
VALIDATE_SIZE(CColTriangle, 0x10);
