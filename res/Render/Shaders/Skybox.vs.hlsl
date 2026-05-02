#include "Common.hlsli"

struct ObjectData
{
    float4x4 world;
    Material material;

    uint indexCount;
    uint startIndexLocation;
    int baseVertexLocation;
    uint padding4;
};

cbuffer SkyBoxData : register(b0)
{
    ObjectData objData;
    int isLockOnCamera;
    
    double padding;
}

cbuffer PassData : register(b1)
{
    float4x4 viewProj;
    float4x4 invView;
    float4x4 view;
    float4x4 proj;
    float4x4 invProj;
    float4x4 invViewProj;

    float3 eyePos;
    float padding2;
}

VertexOut main(VertexIn vin)
{
    VertexOut vout;

    vout.posW = vin.posL;
    
    float4 posW = mul(float4(vin.posL, 1.0f), objData.world);

    if (isLockOnCamera == 1)
    {
        posW.xyz += eyePos;
    }

    vout.posH = mul(posW, viewProj).xyww;
    return vout;
}
