#include "Common.hlsli"

TextureCube textures[] : register(t0);
cbuffer TextureIDBuffer : register(b2)
{
    uint textureID;
}
sampler textureSampler : register(s0);

float4 main(VertexOut pin) : SV_Target
{
    //return float4(1.0f, 0.5f, 0.0f, 1.0f);
    return textures[textureID].Sample(textureSampler, pin.posW);
}

