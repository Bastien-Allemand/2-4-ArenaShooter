// VS

#include "Common.hlsli"

cbuffer ObjectData : register(b0)
{
    float4x4 world;
    Material material;
}

cbuffer PassData : register(b1)
{
    float4x4 viewProj;
    float4x4 invView;
    float4x4 View;
    float4x4 Proj;
    float4x4 InvProj;
    float4x4 InvViewProj;
    float3 eyePos;
}

cbuffer LightPassData : register(b2)
{
    LightData light[MAX_LIGHTS];

    float maxTessDistance;
    float minTessDistance;
    float minTessFactor;
    float maxTessFactor;
    
    float ambientIntensity;
    float padding;
    float padding1;
    float padding2;
}

VertexOut main(VertexIn vIn)
{
    float4x4 worldViewProj = mul(world, viewProj);
    
    VertexOut vOut;
    vOut.posH = mul(float4(vIn.posL, 1.0f), worldViewProj);
    float4 worldPos = mul(float4(vIn.posL, 1.0f), world);
    vOut.posW = worldPos.xyz;
    vOut.uv = vIn.uv;
     
    float3 distance = (invView[3].xyz - vOut.posW);
    
    float tessNear = maxTessFactor;
    float tessFar = minTessFactor;
    
    float tessAlpha = saturate((distance - minTessDistance) / (maxTessDistance - minTessDistance));
    vOut.tessFactor = lerp(tessNear, tessFar, tessAlpha);
    
    if (length(vIn.normal) < 0.001f)
    {
        vOut.normal = float3(0,0, 1);
    }
    else
    {
        float3x3 worldMatrix = (float3x3) world;
        
        vOut.normal = normalize(mul(vIn.normal, worldMatrix));
    }
    
    if (length(vIn.tangent) < 0.001f)
    {
        vOut.tangent = float3(1, 0, 0);
    }
    else
    {
        vOut.tangent = normalize(mul(vIn.tangent, (float3x3) world));
    }
    
    return vOut;
}

