/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/

#include "Matrix.h"
#include "../vendor/armhook/patch.h"
//uint8_t* CMatrix::EulerIndices1 = (uint8_t*)0x866D9C;
//uint8_t* CMatrix::EulerIndices2 = (uint8_t*)0x866D94;
int32_t numMatrices;
CMatrix gDummyMatrix;


void CMatrix::InjectHooks()
{
    CHook::Write(g_libGTASA + (VER_x32 ? 0x00677C70 : 0x84D910), &numMatrices);
}

CMatrix::CMatrix(const CMatrix& matrix) {
    CMatrix::CopyOnlyMatrix(matrix);
}

// like previous + attach
CMatrix::CMatrix(RwMatrix* matrix, bool temporary) {
    CMatrix::Attach(matrix, temporary);
}

// destructor detaches matrix if attached
CMatrix::~CMatrix()
{
    CMatrix::Detach();
}

void CMatrix::Attach(RwMatrix* matrix, bool bOwnsMatrix)
{
    CMatrix::Detach();

    m_pAttachMatrix = matrix;
    m_bOwnsAttachedMatrix = bOwnsMatrix;
    CMatrix::Update();
}

void CMatrix::Detach()
{
    if (m_bOwnsAttachedMatrix && m_pAttachMatrix)
        RwMatrixDestroy(m_pAttachMatrix);

    m_pAttachMatrix = nullptr;
}

// copy base RwMatrix to another matrix
void CMatrix::CopyOnlyMatrix(const CMatrix& matrix)
{
    memcpy(this, &matrix, sizeof(RwMatrix));
}

// update RwMatrix with attaching matrix. This doesn't check if attaching matrix is present, so use it only if you know it is present.
// Using UpdateRW() is more safe since it perform this check.
void CMatrix::Update()
{
    CMatrix::UpdateMatrix(m_pAttachMatrix);
}

// update RwMatrix with attaching matrix.
void CMatrix::UpdateRW()
{
    if (!m_pAttachMatrix)
        return;

    CMatrix::UpdateRwMatrix(m_pAttachMatrix);
}

// update RwMatrix with this matrix
void CMatrix::UpdateRwMatrix(RwMatrix* matrix) const
{
    *RwMatrixGetRight(matrix) = m_right;
    *RwMatrixGetUp(matrix) = m_forward;
    *RwMatrixGetAt(matrix) = m_up;
    *RwMatrixGetPos(matrix) = m_pos;

    RwMatrixUpdate(matrix);
}

void CMatrix::UpdateMatrix(RwMatrix* rwMatrix)
{
    m_right = *RwMatrixGetRight(rwMatrix);
    m_forward = *RwMatrixGetUp(rwMatrix);
    m_up = *RwMatrixGetAt(rwMatrix);
    m_pos = *RwMatrixGetPos(rwMatrix);
}

void CMatrix::SetUnity()
{
    CMatrix::ResetOrientation();
    m_pos.Set(0.0F, 0.0F, 0.0F);
}

void CMatrix::ResetOrientation()
{
    m_right.Set  (1.0F, 0.0F, 0.0F);
    m_forward.Set(0.0F, 1.0F, 0.0F);
    m_up.Set     (0.0F, 0.0F, 1.0F);
}

void CMatrix::SetScale(float scale)
{
    m_right.Set  (scale, 0.0F,  0.0F);
    m_forward.Set(0.0F,  scale, 0.0F);
    m_up.Set     (0.0F,  0.0F,  scale);
    m_pos.Set    (0.0F,  0.0F,  0.0F);
}

// scale on three axes
void CMatrix::SetScale(float x, float y, float z)
{
    m_right.Set  (x,     0.0F,  0.0F);
    m_forward.Set(0.0F,  y,     0.0F);
    m_up.Set     (0.0F,  0.0F,  z   );
    m_pos.Set    (0.0F,  0.0F,  0.0F);
}

void CMatrix::SetTranslateOnly(CVector translation)
{
    m_pos = translation;
}

// like previous + reset orientation
void CMatrix::SetTranslate(CVector translation)
{
    CMatrix::ResetOrientation();
    CMatrix::SetTranslateOnly(translation);
}

void CMatrix::SetRotateXOnly(float angle)
{
    auto fSin = sin(angle);
    auto fCos = cos(angle);

    m_right.Set  (1.0F,  0.0F,  0.0F);
    m_forward.Set(0.0F,  fCos,  fSin);
    m_up.Set     (0.0F, -fSin,  fCos);
}

void CMatrix::SetRotateYOnly(float angle)
{
    auto fSin = sin(angle);
    auto fCos = cos(angle);

    m_right.Set  (fCos,  0.0F, -fSin);
    m_forward.Set(0.0F,  1.0F,  0.0F);
    m_up.Set     (fSin,  0.0F,  fCos);
}

void CMatrix::SetRotateZOnly(float angle)
{
    auto fSin = sin(angle);
    auto fCos = cos(angle);

    m_right.Set  ( fCos, fSin, 0.0F);
    m_forward.Set(-fSin, fCos, 0.0F);
    m_up.Set     ( 0.0F, 0.0F, 1.0F);
}

void CMatrix::SetRotateX(float angle)
{
    CMatrix::SetRotateXOnly(angle);
    m_pos.Set(0.0F, 0.0F, 0.0F);
}

void CMatrix::SetRotateY(float angle)
{
    CMatrix::SetRotateYOnly(angle);
    m_pos.Set(0.0F, 0.0F, 0.0F);
}

void CMatrix::SetRotateZ(float angle)
{
    CMatrix::SetRotateZOnly(angle);
    m_pos.Set(0.0F, 0.0F, 0.0F);
}

// set rotate on 3 axes
void CMatrix::SetRotate(float x, float y, float z)
{
    auto fSinX = sin(x);
    auto fCosX = cos(x);
    auto fSinY = sin(y);
    auto fCosY = cos(y);
    auto fSinZ = sin(z);
    auto fCosZ = cos(z);

    m_right.Set  ( fCosZ*fCosY-(fSinZ*fSinX)*fSinY,  fCosZ*fSinX*fSinY+fSinZ*fCosY,  -(fSinY*fCosX));
    m_forward.Set(-(fSinZ*fCosX),                    fCosZ*fCosX,                     fSinX);
    m_up.Set     ( fCosZ*fSinY+fSinZ*fSinX*fCosY,    fSinZ*fSinY-fCosZ*fSinX*fCosY,   fCosY*fCosX);
    m_pos.Set(0.0F, 0.0F, 0.0F);
}

void CMatrix::RotateX(float angle)
{
    auto rotMat = CMatrix();
    rotMat.SetRotateX(angle);
    m_right =   rotMat * m_right;
    m_forward = rotMat * m_forward;
    m_up =      rotMat * m_up;
    m_pos =     rotMat * m_pos;
}

void CMatrix::RotateY(float angle)
{
    auto rotMat = CMatrix();
    rotMat.SetRotateY(angle);
    m_right =   rotMat * m_right;
    m_forward = rotMat * m_forward;
    m_up =      rotMat * m_up;
    m_pos =     rotMat * m_pos;
}

void CMatrix::RotateZ(float angle)
{
    auto rotMat = CMatrix();
    rotMat.SetRotateZ(angle);
    m_right =   rotMat * m_right;
    m_forward = rotMat * m_forward;
    m_up =      rotMat * m_up;
    m_pos =     rotMat * m_pos;
}

// rotate on 3 axes
void CMatrix::Rotate(CVector rotation)
{
    auto rotMat = CMatrix();
    rotMat.SetRotate(rotation.x, rotation.y, rotation.z);
    m_right =   rotMat * m_right;
    m_forward = rotMat * m_forward;
    m_up =      rotMat * m_up;
    m_pos =     rotMat * m_pos;
}

void CMatrix::Reorthogonalise()
{
    auto vecCross = CrossProduct(m_right, m_forward);
    vecCross.Normalise();

    auto vecCross2 = CrossProduct(m_forward, vecCross);
    vecCross2.Normalise();

    auto vecCross3 = CrossProduct(vecCross, vecCross2);

    m_right = vecCross2;
    m_forward = vecCross3;
    m_up = vecCross;
}

// similar to UpdateRW(RwMatrixTag *)
void CMatrix::CopyToRwMatrix(RwMatrix* matrix) const
{
    UpdateRwMatrix(matrix);
    RwMatrixUpdate(matrix);
}

//void CMatrix::SetRotate(const CQuaternion& quat)
//{
//    auto vecImag2 = quat.imag + quat.imag;
//    auto x2x = vecImag2.x * quat.imag.x;
//    auto y2x = vecImag2.y * quat.imag.x;
//    auto z2x = vecImag2.z * quat.imag.x;
//
//    auto y2y = vecImag2.y * quat.imag.y;
//    auto z2y = vecImag2.z * quat.imag.y;
//    auto z2z = vecImag2.z * quat.imag.z;
//
//    auto x2r = vecImag2.x * quat.real;
//    auto y2r = vecImag2.y * quat.real;
//    auto z2r = vecImag2.z * quat.real;
//
//    m_right.Set  (1.0F-(z2z+y2y),   z2r+y2x,        z2x-y2r);
//    m_forward.Set(y2x-z2r,          1.0F-(z2z+x2x), x2r+z2y);
//    m_up.Set     (y2r+z2x,          z2y-x2r,        1.0F-(y2y+x2x));
//}

void CMatrix::Scale(float scale) {
    ScaleXYZ(scale, scale, scale);
}

void CMatrix::ScaleXYZ(float x, float y, float z) {
    m_right   *= x;
    m_forward *= y;
    m_up      *= z;
}

void CMatrix::Scale(const CVector& scale){
    ScaleXYZ(scale.x, scale.y, scale.z);
}

void CMatrix::ForceUpVector(CVector vecUp) {
    m_right   = CrossProduct(m_forward, vecUp);
    m_forward = CrossProduct(vecUp, m_right);
    m_up      = vecUp;
}

void CMatrix::ConvertToEulerAngles(float* pX, float* pY, float* pZ, uint32_t uiFlags)
{
    float fArr[3][3];

    fArr[0][0] = m_right.x;
    fArr[0][1] = m_right.y;
    fArr[0][2] = m_right.z;

    fArr[1][0] = m_forward.x;
    fArr[1][1] = m_forward.y;
    fArr[1][2] = m_forward.z;

    fArr[2][0] = m_up.x;
    fArr[2][1] = m_up.y;
    fArr[2][2] = m_up.z;

    /* Original indices deciding logic, i replaced it with clearer one
    auto iInd1 = CMatrix::EulerIndices1[(uiFlags >> 3) & 0x3];
    auto iInd2 = CMatrix::EulerIndices2[iInd1 + ((uiFlags & 0x4) != 0)];
    auto iInd3 = CMatrix::EulerIndices2[iInd1 - ((uiFlags & 0x4) != 0) + 1]; */
    int8_t iInd1 = 0, iInd2 = 1, iInd3 = 2;
    switch (uiFlags & eMatrixEulerFlags::_ORDER_MASK) {
        case ORDER_XYZ:
            iInd1 = 0, iInd2 = 1, iInd3 = 2;
            break;
        case ORDER_XZY:
            iInd1 = 0, iInd2 = 2, iInd3 = 1;
            break;
        case ORDER_YZX:
            iInd1 = 1, iInd2 = 2, iInd3 = 0;
            break;
        case ORDER_YXZ:
            iInd1 = 1, iInd2 = 0, iInd3 = 2;
            break;
        case ORDER_ZXY:
            iInd1 = 2, iInd2 = 0, iInd3 = 1;
            break;
        case ORDER_ZYX:
            iInd1 = 2, iInd2 = 1, iInd3 = 0;
            break;
    }

    if (uiFlags & eMatrixEulerFlags::EULER_ANGLES) {
        auto r13 = fArr[iInd1][iInd3];
        auto r12 = fArr[iInd1][iInd2];
        auto cy = sqrt(r12 * r12 + r13 * r13);
        if (cy > 0.0000019073486) { // Some epsilon?
            *pX = atan2(r12, r13);
            *pY = atan2(cy, fArr[iInd1][iInd1]);
            *pZ = atan2(fArr[iInd2][iInd3], -fArr[iInd3][iInd1]);
        }
        else {
            *pX = atan2(-fArr[iInd2][iInd3], fArr[iInd2][iInd2]);
            *pY = atan2(cy, fArr[iInd1][iInd1]);
            *pZ = 0.0F;
        }
    }
    else {
        auto r21 = fArr[iInd2][iInd1];
        auto r11 = fArr[iInd1][iInd1];
        auto cy = sqrt(r11 * r11 + r21 * r21);
        if (cy > 0.0000019073486) { // Some epsilon?
            *pX = atan2(fArr[iInd3][iInd2], fArr[iInd3][iInd3]);
            *pY = atan2(-fArr[iInd3][iInd1], cy);
            *pZ = atan2(r21, r11);
        }
        else {
            *pX = atan2(-fArr[iInd2][iInd3], fArr[iInd2][iInd2]);
            *pY = atan2(-fArr[iInd3][iInd1], cy);
            *pZ = 0.0F;
        }
    }

    if (uiFlags & eMatrixEulerFlags::SWAP_XZ)
        std::swap(*pX, *pZ);

    if (uiFlags & eMatrixEulerFlags::_ORDER_NEEDS_SWAP) {
        *pX = -*pX;
        *pY = -*pY;
        *pZ = -*pZ;
    }
}

void CMatrix::ConvertFromEulerAngles(float x, float y, float z, uint32_t uiFlags)
{
    /* Original indices deciding logic, i replaced it with clearer one
    auto iInd1 = CMatrix::EulerIndices1[(uiFlags >> 3) & 0x3];
    auto iInd2 = CMatrix::EulerIndices2[iInd1 + ((uiFlags & 0x4) != 0)];
    auto iInd3 = CMatrix::EulerIndices2[iInd1 - ((uiFlags & 0x4) != 0) + 1]; */
    int8_t iInd1 = 0, iInd2 = 1, iInd3 = 2;
    switch (uiFlags & eMatrixEulerFlags::_ORDER_MASK) {
        case ORDER_XYZ:
            iInd1 = 0, iInd2 = 1, iInd3 = 2;
            break;
        case ORDER_XZY:
            iInd1 = 0, iInd2 = 2, iInd3 = 1;
            break;
        case ORDER_YZX:
            iInd1 = 1, iInd2 = 2, iInd3 = 0;
            break;
        case ORDER_YXZ:
            iInd1 = 1, iInd2 = 0, iInd3 = 2;
            break;
        case ORDER_ZXY:
            iInd1 = 2, iInd2 = 0, iInd3 = 1;
            break;
        case ORDER_ZYX:
            iInd1 = 2, iInd2 = 1, iInd3 = 0;
            break;
    }

    float fArr[3][3];

    if (uiFlags & eMatrixEulerFlags::SWAP_XZ)
        std::swap(x, z);

    if (uiFlags & eMatrixEulerFlags::_ORDER_NEEDS_SWAP) {
        x = -x;
        y = -y;
        z = -z;
    }

    auto fSinX = sin(x);
    auto fCosX = cos(x);
    auto fSinY = sin(y);
    auto fCosY = cos(y);
    auto fSinZ = sin(z);
    auto fCosZ = cos(z);

    if (uiFlags & eMatrixEulerFlags::EULER_ANGLES) {
        fArr[iInd1][iInd1] = fCosY;
        fArr[iInd1][iInd2] = fSinX*fSinY;
        fArr[iInd1][iInd3] = fCosX*fSinY;

        fArr[iInd2][iInd1] =   fSinY*fSinZ;
        fArr[iInd2][iInd2] =   fCosX*fCosY  - fCosY*fSinX*fSinZ;
        fArr[iInd2][iInd3] = -(fSinX*fCosZ) -(fCosX*fCosY*fSinZ);

        fArr[iInd3][iInd1] = -(fCosZ*fSinY);
        fArr[iInd3][iInd2] =   fCosX*fSinZ  + fCosY*fCosZ*fSinX;
        fArr[iInd3][iInd3] = -(fSinX*fSinZ) + fCosX*fCosY*fCosZ;
    }
    else { // Use Tait-Bryan angles
        fArr[iInd1][iInd1] =   fCosY*fCosZ;
        fArr[iInd1][iInd2] = -(fCosX*fSinZ) + fCosZ*fSinX*fSinY;
        fArr[iInd1][iInd3] =   fSinX*fSinZ  + fCosX*fCosZ*fSinY;

        fArr[iInd2][iInd1] =   fCosY*fSinZ;
        fArr[iInd2][iInd2] =   fCosX*fCosZ  + fSinX*fSinY*fSinZ;
        fArr[iInd2][iInd3] = -(fCosZ*fSinX) + fCosX*fSinY*fSinZ;

        fArr[iInd3][iInd1] = -fSinY;
        fArr[iInd3][iInd2] =  fCosY*fSinX;
        fArr[iInd3][iInd3] =  fCosX*fCosY;
    }

    m_right.Set  (fArr[0][0], fArr[0][1], fArr[0][2]);
    m_forward.Set(fArr[1][0], fArr[1][1], fArr[1][2]);
    m_up.Set     (fArr[2][0], fArr[2][1], fArr[2][2]);
}

void CMatrix::operator=(const CMatrix& rvalue)
{
    CMatrix::CopyOnlyMatrix(rvalue);
    CMatrix::UpdateRW();
}

void CMatrix::operator+=(const CMatrix& rvalue)
{
    m_right += rvalue.m_right;
    m_forward += rvalue.m_forward;
    m_up += rvalue.m_up;
    m_pos += rvalue.m_pos;
}

void CMatrix::operator*=(const CMatrix& rvalue)
{
    *this = (*this * rvalue);
}

CMatrix operator*(const CMatrix& a, const CMatrix& b)
{
    auto result = CMatrix();
    result.m_right =   a.m_right * b.m_right.x   + a.m_forward * b.m_right.y   + a.m_up * b.m_right.z;
    result.m_forward = a.m_right * b.m_forward.x + a.m_forward * b.m_forward.y + a.m_up * b.m_forward.z;
    result.m_up =      a.m_right * b.m_up.x      + a.m_forward * b.m_up.y      + a.m_up * b.m_up.z;
    result.m_pos =     a.m_right * b.m_pos.x     + a.m_forward * b.m_pos.y     + a.m_up * b.m_pos.z + a.m_pos;
    return result;
}

CVector operator*(const CMatrix& a, const CVector& b)
{
    CVector result;
    result.x = a.m_pos.x + a.m_right.x * b.x + a.m_forward.x * b.y + a.m_up.x * b.z;
    result.y = a.m_pos.y + a.m_right.y * b.x + a.m_forward.y * b.y + a.m_up.y * b.z;
    result.z = a.m_pos.z + a.m_right.z * b.x + a.m_forward.z * b.y + a.m_up.z * b.z;
    return result;
}
CMatrix operator+(const CMatrix& a, const CMatrix& b)
{
    CMatrix result;
    result.m_right =   a.m_right + b.m_right;
    result.m_forward = a.m_forward + b.m_forward;
    result.m_up =      a.m_up + b.m_up;
    result.m_pos =     a.m_pos + b.m_pos;
    return result;
}

CMatrix& Invert(CMatrix& in, CMatrix& out)
{
    out.GetPosition().Set(0.0F, 0.0F, 0.0F);

    out.GetRight().Set(in.GetRight().x, in.GetForward().x, in.GetUp().x);
    out.GetForward().Set(in.GetRight().y, in.GetForward().y, in.GetUp().y);
    out.GetUp().Set(in.GetRight().z, in.GetForward().z, in.GetUp().z);

    out.GetPosition() += in.GetPosition().x * out.GetRight();
    out.GetPosition() += in.GetPosition().y * out.GetForward();
    out.GetPosition() += in.GetPosition().z * out.GetUp();
    out.GetPosition() *= -1.0F;

    return out;
}

CMatrix Invert(const CMatrix& in)
{
    CMatrix result;
    Invert(const_cast<CMatrix&>(in), result); // const cast necessary because it's fucked - but it wont be modified.
    return result;
}

CMatrix Lerp(CMatrix from, CMatrix to, float t) {
    from.ScaleAll(1.0f - t);
    to.ScaleAll(t);
    return from + to;
}
