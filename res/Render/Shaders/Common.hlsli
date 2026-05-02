
#define MAX_LIGHTS 16

struct PatchTess
{
    float EdgeTessFactor[3] : SV_TessFactor;
    float InsideTessFactor : SV_InsideTessFactor;
};

struct HullOut
{
    float3 posW : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : UV;
};

struct VertexIn
{
    float3 posL : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : UV;
};

struct VertexOut
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : UV;
    float tessFactor : TESSFACTOR;
};

struct Material
{
    int ambientTextureID;
    float3 FresnelR0;

    int roughnessTextureID;
    float subsurface;
    int normalTextureID;
    int metalnessTextureID;

    int displacementTextureID;
    int albedoTextureID;
    int useTextureAmbient;
    int useTextureNormal;
       
    int useTextureAlbedo;
    int useTextureRoughness;
    int useDisplacementTexture;
    int useTextureMetalness;
};

struct LightData
{
    float3 strength;
    int id;
        
    float4 color;

    float3 direction;
    int NumFramesDirty;

    float3 position;
    float padding;
    
    float3 rimLightColor;
    float range;

    float spotAngle;
    float penumbraAngle;
    float intensity;
    float rimLightIntensity;

    int type;
    float padding1;
    float padding2;
    float padding3;
};

