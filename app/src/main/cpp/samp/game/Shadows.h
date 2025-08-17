/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once


#include "RW/RenderWare.h"
#include "Vector.h"
#include "PtrList.h"
#include "PolyBunch.h"
#include "RealTimeShadow.h"

class CEntity;
class CPhysical;
class CVehicleGTA;
class CPedGTA;

/*
 Shadow rectangle:
         Front
 +---------+---------+
 |         |         |
 |         |         |
 |         | Posn    |
 +---------+---------+ Side
 |         |         |
 |         |         |
 |         |         |
 +---------+---------+

 Posn - world coordinates (x,y,z)
 Front - local coordinates (x,y) relatively to center
 Side - local coordinates (x,y) relatively to center
*/

enum eShadowType : uint8 {
    SHADOW_NONE     = 0,
    SHADOW_DEFAULT  = 1,
    SHADOW_ADDITIVE = 2,
    SHADOW_INVCOLOR = 3,
    SHADOW_OIL_1    = 4,
    SHADOW_OIL_2    = 5, // Oil on fire
    SHADOW_OIL_3    = 6,
    SHADOW_OIL_4    = 7,
    SHADOW_OIL_5    = 8
};

enum eShadowTextureType {
    SHADOW_TEX_CAR = 1,
    SHADOW_TEX_PED = 2,
    SHADOW_TEX_EXPLOSION = 3,
    SHADOW_TEX_HELI = 4,
    SHADOW_TEX_HEADLIGHTS = 5,
    SHADOW_TEX_BLOOD = 6
};

// bruh, OG name
enum VEH_SHD_TYPE {
    VEH_SHD_CAR = 0,
    VEH_SHD_BIKE = 1,
    VEH_SHD_HELI = 2,
    VEH_SHD_PLANE = 3,
    VEH_SHD_RC = 4,
    VEH_SHD_BIG_PLANE = 5 // AT400; ANDROM
};

class CRegisteredShadow {
public:
    CVector          m_vecPosn;
    CVector2D        m_Front;
    CVector2D        m_Side;
    float            m_fZDistance;
    float            m_fScale;
    RwTexture*       m_pTexture;
    CRealTimeShadow* m_pRTShadow;
    uint16           m_nIntensity;
    eShadowType      m_nType; // TODO: Check if this is the correct type...
    uint8            m_nRed;
    uint8            m_nGreen;
    uint8            m_nBlue;

    uint8 m_bDrawOnWater : 1;
    uint8 m_bAlreadyRenderedInBatch : 1;          /// Whenever it has been rendered. Reset each frame, used for batching
    uint8 m_bDrawOnBuildings : 1;
};
VALIDATE_SIZE(CRegisteredShadow, (VER_x32 ? 0x34 : 0x40));

class CPermanentShadow {
public:
    CVector    m_vecPosn;
    float      m_fFrontX;
    float      m_fFrontY;
    float      m_fSideX;
    float      m_fSideY;
    float      m_fZDistance;
    float      m_fScale;
    uint32     m_nTimeCreated;
    uint32     m_nTimeDuration;
    RwTexture* m_pTexture;
    uint16     m_nIntensity;
    eShadowType m_nType;
    uint8      m_nRed;
    uint8      m_nGreen;
    uint8      m_nBlue;

    uint8 m_bDrawOnWater : 1;
    uint8 m_bIgnoreMapObjects : 1;
    uint8 m_bDrawOnBuildings : 1;

    void Init() {
        m_nType = SHADOW_NONE;
    }
};
VALIDATE_SIZE(CPermanentShadow, (VER_x32 ? 0x38 : 0x40));

class CStaticShadow {
public:
    uint32      m_nId;
    CPolyBunch* m_pPolyBunch;
    uint32      m_nTimeCreated;
    CVector     m_vecPosn;
    float       m_fFrontX;
    float       m_fFrontY;
    float       m_fSideX;
    float       m_fSideY;
    float       m_fZDistance;
    float       m_fScale;
    RwTexture*  m_pTexture;
    uint16      m_nIntensity;
    eShadowType m_nType;
    uint8       m_nRed;
    uint8       m_nGreen;
    uint8       m_nBlue;
    bool        m_bJustCreated;
    bool        m_bRendered;
    bool        m_bTemporaryShadow; // delete after 5000ms
    union {
        struct {
            uint8 m_nDayIntensity : 4;
            uint8 m_nNightIntensity : 4;
        };
        uint8 m_nDayNightIntensity;
    };

public:
    static void InjectHooks();

    void Init() {
        m_nId = 0;
        m_pPolyBunch = nullptr;
    }
    void Free();
};
VALIDATE_SIZE(CStaticShadow, (VER_x32 ? 0x40 : 0x50));

struct _ProjectionParam {
    RwV3d    at;           // Camera at vector
    RwMatrix invMatrix;    // Transforms to shadow camera space
    RwUInt8  shadowValue;  // Shadow opacity value
    RwBool   fade;         // Shadow fades with distance
    RwUInt32 numIm3DBatch; // Number of buffer flushes. Unused
    RwMatrix entityMatrix;
};
VALIDATE_SIZE(_ProjectionParam, 0x98);

constexpr uint32 MAX_STORED_SHADOWS      = 48;
constexpr uint32 MAX_PERMANENT_SHADOWS   = 48;
constexpr uint32 MAX_STATIC_SHADOWS      = 48;
constexpr uint32 MAX_SHADOW_POLY_BUNCHES = 360;

class CShadows {
public:
    static inline uint16 ShadowsStoredToBeRendered;
    static inline CPolyBunch* pEmptyBunchList;
    static inline std::array<CPolyBunch, MAX_SHADOW_POLY_BUNCHES> aPolyBunches;
    static inline std::array<CStaticShadow, MAX_STATIC_SHADOWS> aStaticShadows;
    static inline std::array<CPermanentShadow, MAX_PERMANENT_SHADOWS> aPermanentShadows;
    static inline std::array<CRegisteredShadow, MAX_STORED_SHADOWS> asShadowsStored;

    static inline RwTexture* gpShadowHeadLightsTexLong;

public:
    static void InjectHooks();

    static void Init();
    static void Shutdown();
    static void TidyUpShadows();

    static void AddPermanentShadow(uint8 type, RwTexture* texture, CVector* posn, float topX, float topY, float rightX, float rightY, int16 intensity, uint8 red, uint8 greeb, uint8 blue, float drawDistance, uint32 time, float upDistance);
    static void UpdatePermanentShadows();

    static void StoreShadowToBeRendered(uint8 type, const CVector& posn, float frontX, float frontY, float sideX, float sideY, int16 intensity, uint8 red, uint8 green, uint8 blue);
    static void StoreShadowToBeRendered(uint8 type, RwTexture* texture, const CVector& posn, float topX, float topY, float rightX, float rightY, int16 intensity, uint8 red, uint8 green, uint8 blue, float zDistance, bool drawOnWater, float scale, CRealTimeShadow* realTimeShadow, bool drawOnBuildings);
    // NOTSA
    static void StoreShadowToBeRendered(eShadowType type, RwTexture* tex, const CVector& posn, CVector2D top, CVector2D right, int16 intensity, uint8 red, uint8 green, uint8 blue, float zDistance, bool drawOnWater, float scale, CRealTimeShadow* realTimeShadow, bool drawOnBuildings);
    static void SetRenderModeForShadowType(eShadowType type);
    static void RemoveOilInArea(float x1, float x2, float y1, float y2);
    static void GunShotSetsOilOnFire(const CVector& shotOrigin, const CVector& shotTarget);
    static void PrintDebugPoly(CVector* a, CVector* b, CVector* c);
    static void CalcPedShadowValues(CVector sunPosn, float& displacementX, float& displacementY, float& frontX, float& frontY, float& sideX, float& sideY);
    static void AffectColourWithLighting(eShadowType shadowType,
                                         uint8 dayNightIntensity,
                                         uint8 red, uint8 green, uint8 blue,
                                         uint8& outRed, uint8& outGreen, uint8& outBlue
    );
    static void StoreShadowForPedObject(CPedGTA* ped, float displacementX, float displacementY, float frontX, float frontY, float sideX, float sideY);
    static void StoreRealTimeShadow(CPhysical* physical, float displacementX, float displacementY, float frontX, float frontY, float sideX, float sideY);
    /*!
    * @addr 0x707F40
    */
    static void UpdateStaticShadows();
    static void RenderExtraPlayerShadows();

    /*!
    * @addr 0x708300
    * @brief Render all active static shadows
    */
    static void RenderStaticShadows();

    static void CastShadowEntityXY(CEntity* entity, float conrerAX, float cornerAY, float cornerBX, float cornerBY, CVector* posn, float frontX, float frontY, float sideX, float sideY, int16 intensity, uint8 red, uint8 green, uint8 blue, float zDistance, float scale, CPolyBunch** ppPolyBunch, uint8* pDayNightIntensity, int32 shadowType);
    static void CastShadowEntityXYZ(CEntity* entity, CVector* posn, float frontX, float frontY, float sideX, float sideY, int16 intensity, uint8 red, uint8 green, uint8 blue, float zDistance, float scale, CPolyBunch** ppPolyBunch, CRealTimeShadow* realTimeShadow);
    static void CastPlayerShadowSectorList(CPtrList& ptrList, float conrerAX, float cornerAY, float cornerBX, float cornerBY, CVector* posn, float frontX, float frontY, float sideX, float sideY, int16 intensity, uint8 red, uint8 green, uint8 blue, float zDistance, float scale, CPolyBunch** ppPolyBunch, uint8* pDayNightIntensity, int32 shadowType);
    static void CastShadowSectorList(CPtrList& ptrList, float conrerAX, float cornerAY, float cornerBX, float cornerBY, CVector* posn, float frontX, float frontY, float sideX, float sideY, int16 intensity, uint8 red, uint8 green, uint8 blue, float zDistance, float scale, CPolyBunch** ppPolyBunch, uint8* pDayNightIntensity, int32 shadowType);
    static void CastRealTimeShadowSectorList(CPtrList& ptrList, float conrerAX, float cornerAY, float cornerBX, float cornerBY, CVector* posn, float frontX, float frontY, float sideX, float sideY, int16 intensity, uint8 red, uint8 green, uint8 blue, float zDistance, float scale, CPolyBunch** ppPolyBunch, CRealTimeShadow* realTimeShadow, uint8* pDayNightIntensity);
    static void RenderStoredShadows();
    static void GeneratePolysForStaticShadow(int16 staticShadowIndex);
    static bool StoreStaticShadow(uint32 id, eShadowType type, RwTexture* texture, const CVector* posn, float frontX, float frontY, float sideX, float sideY, int16 intensity, uint8 red, uint8 green, uint8 blue, float zDistane, float scale, float drawDistance, bool temporaryShadow, float upDistance);
    static void StoreShadowForVehicle(CVehicleGTA* vehicle, VEH_SHD_TYPE vehShadowType);
    static void StoreCarLightShadow(CVehicleGTA* vehicle, int32 id, RwTexture* texture, CVector* posn, float frontX, float frontY, float sideX, float sideY, uint8 red, uint8 green, uint8 blue, float maxViewAngle);
    static void StoreShadowForPole(CEntity* entity, float offsetX, float offsetY, float offsetZ, float poleHeight, float poleWidth, uint32 localId);
    static void RenderIndicatorShadow(uint32 id, eShadowType, RwTexture* texture, const CVector& posn, float frontX, float frontY, float sideX, float sideY, int16 intensity);
};

#ifdef _MSC_VER
CVector* ShadowRenderTriangleCB(CVector* normal, CVector* trianglePos, _ProjectionParam* param);
#endif

constexpr float MAX_DISTANCE_PED_SHADOWS = 15.0f; // 0x8D5240 - TODO: Rename to `MAX_DISTANCE_SHADOWS`
constexpr float MAX_DISTANCE_PED_SHADOWS_SQR = MAX_DISTANCE_PED_SHADOWS * MAX_DISTANCE_PED_SHADOWS; // 0xC4B6B0 - TODO: Rename to `MAX_DISTANCE_PED_SHADOWS_SQ`

static inline RwTexture* gpShadowCarTex;
static inline RwTexture* gpShadowPedTex;
static inline RwTexture* gpShadowHeliTex;
static inline RwTexture* gpShadowBikeTex;
static inline RwTexture* gpShadowBaronTex;
static inline RwTexture* gpShadowExplosionTex;
static inline RwTexture* gpShadowHeadLightsTex; // near
static inline RwTexture* gpShadowHeadLightsTex2; // single
static inline RwTexture* gpBloodPoolTex;
static inline RwTexture* gpHandManTex;
static inline RwTexture* gpCrackedGlassTex;
static inline RwTexture* gpPostShadowTex;

static inline std::array<RwImVertexIndex, 24> g_ShadowVertices;
