#define NUM_CONTROL_POINTS 3
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

Texture2D textures[] : register(t0);
cbuffer TextureIDBuffer : register(b3)
{
    uint textureID;
}
sampler textureSampler : register(s0);

float GetEdgeDistance(float2 uv)
{
    float2 distToEdge = min(uv, 1.0 - uv);
    return min(distToEdge.x, distToEdge.y);
}


[domain("tri")]
VertexOut main(PatchTess input, float3 bary : SV_DomainLocation, const OutputPatch<HullOut, NUM_CONTROL_POINTS> tri)
{
    VertexOut dout;
    
    float3 posW = bary.x * tri[0].posW + bary.y * tri[1].posW + bary.z * tri[2].posW;
    float3 normal = bary.x * tri[0].normal + bary.y * tri[1].normal + bary.z * tri[2].normal;
    float3 tangent = bary.x * tri[0].tangent + bary.y * tri[1].tangent + bary.z * tri[2].tangent;
    float2 uv = bary.x * tri[0].uv + bary.y * tri[1].uv + bary.z * tri[2].uv;
    
    normal = normalize(normal);
    tangent = normalize(tangent);
    
    
    // === DISPLACEMENT MAPPING === //
    
    if (material.useDisplacementTexture == 1)
    {
        float3 cameraPos = invView[3].xyz;
        float distance = length(cameraPos - posW);
        
        const float MipInterval = 20.0f;
        float mipLevel = clamp((distance - MipInterval) / MipInterval, 0.0f, 6.0f);
        
        float displacementHeight = textures[material.displacementTextureID].SampleLevel(textureSampler, uv, mipLevel).r;
        
        float displacementScale = 0.10f;
        displacementHeight = (displacementHeight - 0.5f) * displacementScale;
        
        float edgeDist = GetEdgeDistance(uv);
        float edgeFactor = smoothstep(0.0, 0.15, edgeDist);

        displacementHeight *= edgeFactor;
        
        posW += normal * displacementHeight;
          
        float2 texelSize = float2(1.0f / 2048.0f, 1.0f / 2048.0f);
        
        float hL = textures[material.displacementTextureID].SampleLevel(textureSampler, uv + float2(-texelSize.x, 0), mipLevel).r;
        float hR = textures[material.displacementTextureID].SampleLevel(textureSampler, uv + float2(texelSize.x, 0), mipLevel).r;
        float hD = textures[material.displacementTextureID].SampleLevel(textureSampler, uv + float2(0, -texelSize.y), mipLevel).r;
        float hU = textures[material.displacementTextureID].SampleLevel(textureSampler, uv + float2(0, texelSize.y), mipLevel).r;
        
        float3 tangentSpaceNormal = normalize(float3((hL - hR) * displacementScale * 20.0f, (hD - hU) * displacementScale * 20.0f, 1.0));

        float3 N = normal;
        float3 T = normalize(tangent - dot(tangent, N) * N);
        float3 B = cross(N, T);
        
        float3x3 TBN = float3x3(T, B, N);
        
        float3 displacementnormal = normalize(mul(tangentSpaceNormal, TBN));
         normal = normalize(lerp(normal, displacementnormal, 0.5f));
    }
    
    dout.posW = posW;
    dout.posH = mul(float4(posW, 1.0f), viewProj);
    dout.normal = normal;
    dout.tangent = tangent;
    dout.uv = uv;
    dout.tessFactor = 0.0f;
    
    return dout;
}