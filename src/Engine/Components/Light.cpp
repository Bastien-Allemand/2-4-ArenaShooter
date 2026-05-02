#include "pch.h"
#include "Light.h"

#include "GameObject.h"

namespace gce
{
    void Light::Init()
    {
    }

    Light& Light::DefaultPointLight()
    {
        type = LightType::POINT;
        position = { 0.0f, 0.0f, 0.0f };
        color = { 1.0f, 1.0f, 1.0f, 1.0f };
        range = 8.0f;
        intensity = 0.5f;
        strength = { 0.8f, 0.8f, 0.8f };
        rimLightColor = { color.x(), color.y(), color.z() };
        rimLightIntensity = 0.3f;
        return *this;
    }
    
    Light& Light::DefaultDirectionLight()
    {
        type = LightType::DIRECTIONAL;
        direction = { 0.0f, 0.0f, 1.0f };
        color = { 1.0f, 1.0f, 1.0f, 1.0f };
        intensity = 0.7f;
        strength = { 1.0f, 1.0f, 1.0f };
        rimLightColor = { color.x(), color.y(), color.z() };
        rimLightIntensity = 2.0f;
        return *this;
    }
    
    Light& Light::DefaultSpotLight()
    {
        type = LightType::SPOT;
        position = { 0.0f, 0.0f, 0.0f };
        direction = { 0.0f, 0.0f, 1.0f };
        color = { 1.0f, 1.0f, 1.0f, 1.0f };
        spotAngle = 30.0f;
        penumbraAngle = 45.0f;
        range = 2.0f;
        intensity = 0.5f;
        strength = { 0.5f, 0.5f, 0.5f };
        rimLightColor = { color.x(), color.y(), color.z() };
        rimLightIntensity = 0.4f;
        return *this;
    }
    
}