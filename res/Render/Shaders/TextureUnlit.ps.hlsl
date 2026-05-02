#include "Common.hlsli"

cbuffer ObjectData : register(b0)
{
    float4x4 world;
    Material material;
}
Texture2D textures[] : register(t0);
cbuffer TexID : register(b3) { uint textureID; };
sampler textureSampler : register(s0);

float4 main(VertexOut vIn) : SV_Target
{
    return textures[material.albedoTextureID].Sample(textureSampler, vIn.uv);
}