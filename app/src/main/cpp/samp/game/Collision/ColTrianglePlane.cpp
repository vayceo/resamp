
#include "ColTrianglePlane.h"
#include "ColTriangle.h"

void CColTrianglePlane::InjectHooks()
{

}

CColTrianglePlane::CColTrianglePlane(const CColTriangle& tri, const CollisionVector* vertices) :
        CColTrianglePlane{
                vertices[tri.vA],
                vertices[tri.vB],
                vertices[tri.vC]
        }
{
}

CColTrianglePlane::CColTrianglePlane(const CStoredCollPoly& poly) :
        CColTrianglePlane{
                poly.verts[0],
                poly.verts[1],
                poly.verts[2]
        }
{
}

CColTrianglePlane::CColTrianglePlane(const CVector& vA, const CVector& vB, const CVector& vC) {
    const auto normal = (vC - vA).Cross(vB - vA).Normalized();

    m_normal       = normal;
    m_normalOffset = vA.Dot(normal);

    m_orientation = [&, this] {
        const auto nx = std::abs(normal.x);
        const auto ny = std::abs(normal.y);
        const auto nz = std::abs(normal.z);

        using enum Orientation;
        if (nx > ny && nx > nz) {
            return normal.x <= 0.0f ? NEG_X : POS_X;
        }
        if (ny > nz) {
            return normal.y <= 0.0f ? NEG_Y : POS_Y;
        }
        return normal.z <= 0.0f ? NEG_Z : POS_Z;
    }();
}

void CColTrianglePlane::GetNormal(CVector& out) {
    out = GetNormal();
}

void CColTrianglePlane::Set(const CollisionVector* vertices, const CColTriangle& triangle) {
    *this = CColTrianglePlane{ triangle, vertices };
}
