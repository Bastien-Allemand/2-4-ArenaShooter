#include "Common.hlsli"

// ===================================================================
// UTILS METHODS
// ===================================================================

//float3 TonemapACES(float3 x)
//{
//    const float a = 2.51;
//    const float b = 0.03;
//    const float c = 2.43;
//    const float d = 0.59;
//    const float e = 0.14;
//    return saturate((x * (a * x + b)) / (x * (c * x + d) + e));
//}

   //float attenuation = 1.0 / (distance * distance);
    //attenuation *= (light.range * light.range) / (distance * distance + light.range * light.range);


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
    float padding2;
    float padding3;
    float padding4;
}

Texture2D textures[] : register(t0);
cbuffer TextureIDBuffer : register(b3)
{
    uint textureID;
}
sampler textureSampler : register(s0);

float3 SampleNormal(float2 uv, float3 normal, float3 tangent)
{
    if (material.useTextureNormal == 1)
    {
        float3 normalMap = textures[material.normalTextureID].SampleLevel(textureSampler, uv, 0.0f).rgb;
        normalMap = normalMap * 2.0 - 1.0;

        float3 N = normalize(normal);
        float3 T = normalize(tangent);
        T = normalize(T - dot(T, N) * N);
        float3 B = cross(N, T);
        float3x3 TBN = float3x3(T, B, N);
 
        return normalize(mul(normalMap, TBN));
    }
    else
        return normalize(normal);
}

float3 CombineNormal(float3 baseNormal, float3 normalMapSample, float3 tangent)
{
    if (material.useTextureNormal == 1)
    {
        float3 normalMap = normalMapSample * 2.0 - 1.0;
        
        float3 N = normalize(baseNormal);
        float3 T = normalize(tangent);
        T = normalize(T - dot(T, N) * N);
        float3 B = cross(N, T);
        float3x3 TBN = float3x3(T, B, N);
        
        float3 worldNormal = normalize(mul(normalMap, TBN));
        
        return normalize(lerp(baseNormal, worldNormal, 0.3f));
    }
    else
    {
        return normalize(baseNormal);
    }
}

float SampleMetalness(float2 uv)
{
    if (material.useTextureMetalness == 1)
        return textures[material.metalnessTextureID].Sample(textureSampler, uv).r;
    else
        return 0.0f;
}

float SampleRoughness(float2 uv)
{
    if (material.useTextureRoughness == 1)
        return textures[material.roughnessTextureID].Sample(textureSampler, uv).r;
    else
        return 0.1f;
}

float3 SampleAmbient(float2 uv)
{
    if (material.useTextureAmbient == 1)
        return textures[material.ambientTextureID].Sample(textureSampler, uv).rgb;
    else
        return float3(1.0f, 1.0f, 1.0f);
}

float3 SampleDisplacement(float2 uv, float mipLevel)
{
    if (material.useDisplacementTexture == 1)
        return textures[material.displacementTextureID].SampleLevel(textureSampler, uv, mipLevel).r * 0.3f;
    else
        return 0.5f;
}

float3 SampleAlbedo(float2 uv)
{
    if (material.useTextureAlbedo == 1)
        return textures[material.albedoTextureID].Sample(textureSampler, uv);
    else
        return float3(0.5f, 0.5f, 0.5f);
}


float3 SchlickFresnelAdvanced(float3 F0, float3 halfDir, float3 viewDir)
{
    float cosTheta = saturate(dot(halfDir, viewDir));
    float fresnel = pow(1.0 - cosTheta, 5.0);
    return F0 + (1.0 - F0) * fresnel;
}

float DistributionGGXAnisotropic(float3 normal, float3 halfwayDir, float roughness, float anisotropy)
{
    float alpha = roughness * roughness;
    float alphaX = alpha * (1.0 + anisotropy);
    float alphaY = alpha * (1.0 - anisotropy);
    
    float NdotH = max(dot(normal, halfwayDir), 0.0);
    float NdotH2 = NdotH * NdotH;
    
    float denom = NdotH2 * (alphaX * alphaY - 1.0) + 1.0;
    denom = 3.14159265359 * denom * denom;
    
    return (alphaX * alphaY) / denom;
}
float DistributionGGX(float3 normal, float3 halfwayDir, float roughness)
{
    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;
    float NdotH = max(dot(normal, halfwayDir), 0.0);
    float NdotH2 = NdotH * NdotH;
    
    float denom = NdotH2 * (alpha2 - 1.0) + 1.0;
    denom = 3.14159265359 * denom * denom;
    
    return alpha2 / max(denom, 0.0001);
}

float GeometrySchlickGGX(float cosTheta, float k)
{
    return cosTheta / (cosTheta * (1.0 - k) + k);
}

float GeometrySmithAdvanced(float3 normal, float3 viewDir, float3 lightDir, float roughness)
{
    float alpha = roughness * roughness;
    float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;;
    
    float NdotV = max(dot(normal, viewDir), 1e-5);
    float NdotL = max(dot(normal, lightDir), 1e-5);
    
    return GeometrySchlickGGX(NdotV, k)
           * GeometrySchlickGGX(NdotL, k);
}

float3 SubsurfaceScattering(float3 normal, float3 lightDir, float3 viewDir, float3 lightColor, float subsurfaceAmount)
{
    float3 scatterDir = lightDir + normal * subsurfaceAmount;
    float scatter = pow(saturate(dot(viewDir, -scatterDir)), 12.0) * subsurfaceAmount;
    return lightColor * scatter * 0.5;
}

float3 CalculateRimLighting(float3 normal, float3 viewDir, float3 lightColor, float power, float intensity, float3 lightDir)
{
    float rim = 1.0 - saturate(dot(normal, viewDir));
    rim = pow(rim, power);
    
    float NdotL = max(dot(normal, lightDir), 0.0);
    float lightInfluence = saturate(NdotL + 0.3);
    
    rim *= lightInfluence;
    return lightColor * rim * intensity;
}

float3 VolumetricLighting(float3 worldPos, float3 lightPos, float3 lightColor, float intensity)
{
    float3 toLight = lightPos - worldPos;
    float distance = length(toLight);
    float attenuation = 1.0 / (1.0 + distance * distance * 0.01);
    
    // Halo lumi re
    float halo = exp(-distance * 0.1) * intensity;
    return lightColor * halo * attenuation;
}

float3 VolumetricDirectional(float3 viewDir, float3 lightDir, float intensity, LightData light)
{
    float scatter = pow(saturate(dot(viewDir, -lightDir)), 8.0);
    return light.color.rgb * scatter * intensity;
}

float CalculateAttenuation(float distance, float range)
{
    float linearAttenuation = 1.0 / max(distance, 0.01);
    float quadraticAttenuation = 1.0 / max(distance * distance, 0.01);
    
    float falloff = saturate(1.0 - pow(distance / range, 4.0));
    falloff = falloff * falloff;
    
    float finalAttenuation = lerp(linearAttenuation, quadraticAttenuation, 0.7) * falloff;
    
    return finalAttenuation;
}

// ===================================================================
// DIRECTIONNAL LIGHT ADVANCED
// ===================================================================

float3 CalculateAdvancedDirectionnalTexture(float3 worldPosition, float3 basenormal, float3 tangent, float2 uv, float3 sampleAlbedo, float3 sampleAmbient, float sampleRoughness, float sampleMetallic, LightData light)
{
    float3 normal = CombineNormal(basenormal, textures[material.normalTextureID].Sample(textureSampler, uv).rgb, tangent);
     
    float3 lightDir = normalize(-light.direction);
    float3 viewDir = normalize(eyePos.xyz - worldPosition);
    float3 halfwayDir = normalize(lightDir + viewDir);
    
    // ===HALF-LAMBERT ===
    float NdotL_raw = dot(normal, lightDir);
    float NdotL_halfLambert = NdotL_raw * 0.5 + 0.5;
    float NdotL_standard = max(NdotL_raw, 0.0);
    
  
    float NdotL_final = lerp(NdotL_halfLambert, NdotL_standard, 0.7);
    
    float NdotV = max(dot(normal, viewDir), 0.0001);
    float NdotH = max(dot(normal, halfwayDir), 0.0);
    float VdotH = max(dot(viewDir, halfwayDir), 0.0);
    
    // === PROPRI T S MAT RIAU  === //
    float3 F0 = lerp(float3(0.04, 0.04, 0.04), sampleAlbedo, sampleMetallic);
    float safeRoughness = max(sampleRoughness, 0.045);
    
    // === COMPOSANTS PBR  === //
    float D = DistributionGGX(normal, halfwayDir, safeRoughness);
    float G = GeometrySmithAdvanced(normal, viewDir, lightDir, safeRoughness);
    float3 F = SchlickFresnelAdvanced(F0, halfwayDir, viewDir);
    
    // === CONSERVATION D' NERGIE === //
    float3 kS = F;
    float3 kD = (1.0 - kS) * (1.0 - sampleMetallic);
    
    // === CALCULS D' CLAIRAGE === //
    float3 diffuse = kD * sampleAlbedo / 3.14159265359;
    float3 specular = (D * F * G) / max(4.0 * NdotV * NdotL_standard, 0.0001);
    specular = min(specular, float3(1.0, 1.0, 1.0));
    
    // === MASQUES POUR ADOUCIR LE SP CULAIRE === //
    float specularFade = smoothstep(0.3, 1.0, NdotV);
    float lightingMask = smoothstep(0.3, 1.0, NdotL_final);
    specular *= specularFade * lightingMask;
    
    // === EFFETS ADDITIONNELS  === //
    float3 rimLight = CalculateRimLighting(normal, viewDir, light.rimLightColor, light.rimLightIntensity, 0.05, lightDir);
    rimLight *= lightingMask;
    
    float3 subsurface = SubsurfaceScattering(normal, lightDir, viewDir, light.color.rgb, material.subsurface * 0.3);
    subsurface *= NdotL_final;
   
    // === AMBIENT === //
    // float ao = sampleAmbient.r;
    // float3 ambient = sampleAlbedo * ambientIntensity * ao;
        
    // === RADIANCE AVEC HALF-LAMBERT === // 
    float3 radiance = light.color.rgb * light.strength * light.intensity;
    float3 Lo = (diffuse + specular) * radiance * NdotL_final;
        
    // === R SULTAT FINAL === //
    float3 finalColor = Lo + rimLight + subsurface;
   
    return finalColor;
}

// ===================================================================
// POINT LIGHT ADVANCED
// ===================================================================

float3 CalculateAdvancedPointLightTexture(float3 worldPosition, float3 basenormal, float3 tangent, float2 uv, float3 sampleAlbedo, float3 sampleAmbient, float sampleRoughness, float sampleMetallic, LightData light)
{
    float3 normal = CombineNormal(basenormal, textures[material.normalTextureID].SampleLevel(textureSampler, uv, 0.0f).rgb, tangent);
    
    float3 lightDir = light.position - worldPosition;
    float distance = length(lightDir);
    lightDir = normalize(lightDir);
    
    float attenuation = CalculateAttenuation(distance, light.range);
    
    float3 viewDir = normalize(eyePos.xyz - worldPosition);
    float3 halfwayDir = normalize(lightDir + viewDir);
    
   // float NdotL = max(dot(normal, lightDir),0.0f);
    float NdotV = max(dot(normal, viewDir), 0.0);
    
    float NdotL_raw = dot(normal, lightDir);
    
    
    float NdotL_halfLambert = NdotL_raw * 0.5 + 0.5;
    float NdotL = max(NdotL_raw, 0.0f);
    
    float transitionFactor = pow(NdotL_halfLambert, 2.0);
    
   
    float lightContribution = transitionFactor;
    
    float3 F0 = lerp(float3(0.04, 0.04, 0.04), sampleAlbedo, sampleMetallic);
    //F0 *= sampleSpecular;
    
    float D = DistributionGGX(normal, halfwayDir, sampleRoughness);
    float G = GeometrySmithAdvanced(normal, viewDir, lightDir, sampleRoughness);
    float3 F = SchlickFresnelAdvanced(F0, halfwayDir, viewDir);
    
    // === CALCULS D' CLAIRAGE === //
    float3 kS = F;
    float3 kD = (1.0 - kS) * (1.0 - sampleMetallic);
    
    float3 diffuse = kD * sampleAlbedo / 3.14159265359;
    float3 specular = (D * F * G) / max(4.0 * NdotV * NdotL, 0.001);
    
    
    // === EFFETS ADDITIONNELS === //
    float3 rimLight = CalculateRimLighting(normal, viewDir, light.rimLightColor, light.rimLightIntensity, 0.8, lightDir) * lightContribution;
    float3 subsurface = SubsurfaceScattering(normal, lightDir, viewDir, light.color.rgb, material.subsurface) * lightContribution;
    float3 volumetric = VolumetricLighting(worldPosition, light.position, light.color.rgb, 0.3) * lightContribution;
    
    // === AMBIENT === //
    // float ao = sampleAmbient.r;
    // float3 ambient = sampleAlbedo * ambientIntensity * ao;
    
    // if (NdotL_raw <= -0.8)
    // {
    //     return ambient;
    // }
    
    // === RADIANCE DE BASE === // 
    float3 radiance = light.color.rgb * light.strength * light.intensity * attenuation;
    float3 Lo = (diffuse + specular) * radiance * NdotL * lightContribution;
    
    // === R SULTAT FINAL === //
    float3 finalColor = Lo + rimLight + subsurface + volumetric;
    
    return finalColor;
}

// ===================================================================
// SPOT LIGHT ADVANCED
// ===================================================================

float3 CalculateAdvancedSpotLightTexture(float3 worldPosition, float3 basenormal, float3 tangent, float2 uv, float3 sampleAlbedo, float3 sampleAmbient, float sampleRoughness, float sampleMetallic, LightData light)
{
    float3 normal = CombineNormal(basenormal, textures[material.normalTextureID].SampleLevel(textureSampler, uv, 0.0f).rgb, tangent);
 
    float3 lightDir = normalize(light.position - worldPosition);
    float3 spotDir = normalize(-light.direction);
    
    float innerCone = cos(radians(light.penumbraAngle));
    float outerCone = cos(radians(light.spotAngle));
    
    float cosAngle = dot(lightDir, spotDir);
    float spotAttenuation = smoothstep(innerCone, outerCone, cosAngle);
    
    float distance = length(light.position - worldPosition);
    float distanceAttenuation = CalculateAttenuation(distance, light.range);
    
    //if (spotAttenuation <= 0.1)
    //{
    //    float ao = sampleAmbient.r;
    //    return sampleAlbedo * 0.1f * ao;
    //}
    
 
    float3 viewDir = normalize(eyePos.xyz - worldPosition);
    float3 halfwayDir = normalize(lightDir + viewDir);
    
    //float NdotL = max(dot(normal, lightDir), 0.0);
    float NdotV = max(dot(normal, viewDir), 0.0);
    
    float NdotL_raw = dot(normal, lightDir);
    
    float NdotL_halfLambert = NdotL_raw * 0.5 + 0.5;
    float NdotL = max(NdotL_raw, 0.0f);
    
    float backfaceContribution = 0.2;
    float frontfaceContribution = 1.0;
    
    float lightContribution = lerp(backfaceContribution, frontfaceContribution,
                                   smoothstep(-0.3, 0.3, NdotL_raw));
    
 
    float3 F0 = lerp(float3(0.04, 0.04, 0.04), sampleAlbedo, sampleMetallic);
    //F0 *= sampleSpecular;
    
    float D = DistributionGGX(normal, halfwayDir, sampleRoughness);
    float G = GeometrySmithAdvanced(normal, viewDir, lightDir, sampleRoughness);
    float3 F = SchlickFresnelAdvanced(F0, halfwayDir, viewDir);
    
      // === EFFETS ADDITIONNELS === //
    float3 rimLight = CalculateRimLighting(normal, viewDir, light.rimLightColor, light.rimLightIntensity, 0.8, lightDir) * lightContribution;
    float3 subsurface = SubsurfaceScattering(normal, lightDir, viewDir, light.color.rgb, material.subsurface) * lightContribution;
    float3 volumetric = VolumetricLighting(worldPosition, light.position, light.color.rgb, 0.3) * lightContribution;
    
    float3 kS = F;
    float3 kD = (1.0 - kS) * (1.0 - sampleMetallic);
    
    float3 diffuse = kD * sampleAlbedo / 3.14159265359;
    float3 specular = (D * F * G) / max(4.0 * NdotV * NdotL, 0.001);
       
    // === AMBIENT === //
    // float ao = sampleAmbient.r;
    // float3 ambient = sampleAlbedo * ambientIntensity * ao;
     
    // === RADIANCE DE BASE === // 
    float3 radiance = light.color.rgb * light.strength * light.intensity * spotAttenuation * distanceAttenuation;
    float3 Lo = (diffuse + specular) * radiance * NdotL * lightContribution;
    
    // === R SULTAT FINAL === //
    float3 finalColor = Lo + rimLight + subsurface + volumetric;
    
    return finalColor;
}

float4 main(VertexOut vOut) : SV_Target
{
    float3 tangent = normalize(vOut.tangent);
    float2 uv = vOut.uv;
    float3 normal = normalize(vOut.normal);
    
    float3 albedo = SampleAlbedo(uv);
    float3 ambient = SampleAmbient(uv);
    float roughness = SampleRoughness(uv);
    float metallic = SampleMetalness(uv);

    float3 finalColor = float3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < MAX_LIGHTS; i++)
    {
        LightData lights = light[i];
        
        if (lights.type < 0 || lights.intensity <= 0.0f)
            continue;
        
        if (lights.type == 0) // Directional
        {
            finalColor += CalculateAdvancedDirectionnalTexture(
                    vOut.posW, normal, tangent, uv,
                    albedo, ambient, roughness, metallic, lights);
        }
        else if (lights.type == 1)// Point
        {
            float distance = length(lights.position - vOut.posW);
            if (distance <= lights.range)
            {
            
                finalColor += CalculateAdvancedPointLightTexture(
                    vOut.posW, normal, tangent, uv,
                    albedo, ambient, roughness, metallic, lights);
            }
        }
        else if (lights.type == 2)// Spot
        {
            float distance = length(lights.position - vOut.posW);
            if (distance <= lights.range)
            {
                finalColor += CalculateAdvancedSpotLightTexture(
                    vOut.posW, normal, tangent, uv,
                    albedo, ambient, roughness, metallic, lights);
            }
        }
    }
    
    float ao = ambient.r;
    float3 globalAmbient = albedo * ambientIntensity * ao;
    finalColor += globalAmbient;

    finalColor = finalColor / (finalColor + 1.0f);

    return float4(finalColor, 1.0f);
};