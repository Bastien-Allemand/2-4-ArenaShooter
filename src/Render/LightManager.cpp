#include "pch.h"
#include "LightManager.h"
#include "FrameResources.h"

namespace gce
{

	LightManager::LightManager()
	{
		m_lastFreeIndexInList = 0;
		m_lightCount = 0;

		for (int i = 0; i < MAX_LIGHTS; i++)
		{
			m_lightPassData.light[i].type = LightType::NONE;
			m_lightPassData.light[i].intensity = 0.0f;
		}
		
	}

	LightManager::~LightManager()
	{
		
	}

	void LightManager::AddLight(LightData& light)
	{
		LightManager& lm = Instance();
		int freeIndex = -1;
		for (int32 i = 0; i < MAX_LIGHTS; i++)
		{
			if (lm.m_lightPassData.light[i].type == LightType::NONE)
			{
				freeIndex = i;
				break;
			}
		}

		if (freeIndex == -1)
			return;

		lm.m_lightPassData.light[freeIndex] = light;
		light.id = freeIndex;
		lm.m_lightCount += 1;
	}

	void LightManager::UpdateLight(LightData& light)
	{
		LightManager& lm = Instance();
		lm.m_lightPassData.light[light.id] = light;
	}

	void LightManager::RemoveLight(int32 lightID)
	{
		if (lightID >= MAX_LIGHTS)
			return;
		LightManager& lm = Instance();

		LightData emptyLight = {};
		emptyLight.type = LightType::NONE;
		emptyLight.intensity = 0.0f;

		lm.m_lightPassData.light[lightID] = emptyLight;
		lm.m_lightCount -= 1;

	}

	void LightManager::SetLightsProperties(float32 minTessDistance, float32 maxTessDistance, float32 minTessFactor, float32 maxTessFactor, float32 ambientIntensity)
	{
		LightManager& lm = Instance();
		lm.m_lightPassData.maxTessDistance = maxTessDistance;
		lm.m_lightPassData.minTessDistance = minTessDistance;
		lm.m_lightPassData.minTessFactor = minTessFactor;
		lm.m_lightPassData.maxTessFactor = maxTessFactor;
		if (ambientIntensity > 1.0f)
			ambientIntensity = 1.0f;
		if (ambientIntensity < 0.1f)
			ambientIntensity = 0.1f;
		lm.m_lightPassData.ambientIntensity = ambientIntensity;
	}

	void LightManager::ClearLights()
	{
		LightManager& lm = Instance();
		lm.m_lightCount = 0;
		LightData emptyLight = {};
		emptyLight.type = LightType::NONE;
		emptyLight.intensity = 0.0f;
		for (uint32 i = 0; i < MAX_LIGHTS; i++)
		{
			lm.m_lightPassData.light[i] = emptyLight;
		}
	}

	LightData& LightManager::GetLight(uint32 index)
	{
		LightManager& lm = Instance();

		if (index >= 0 && index < lm.m_lightCount)
		{
			return lm.m_lightPassData.light[index];
		}
		return lm.m_lightPassData.light[0];
	}

	uint32 LightManager::GetLightCount()
	{
		return Instance().m_lightCount;
	}

	LightData LightManager::CreateDirectionalLight(Vector3f32 const& direction, Vector4 const& color, float32 intensity, float32 strength)
	{
		LightData light = {};
		light.type = LightType::DIRECTIONAL;
		light.direction = direction;
		light.color = color;
		light.intensity = intensity;
		light.strength = Vector3f32(strength, strength, strength);
		light.rimLightColor = Vector3f32(color.GetX(), color.GetY(), color.GetZ());
		light.rimLightIntensity = 2.0f;
		return light;
	}

	LightData LightManager::CreatePointLight(Vector3f32 const& position, Vector4 const& color, float32 range, float32 intensity, float32 strength)
	{
		LightData light = {};
		light.type = LightType::POINT;
		light.position = position;
		light.color = color;
		light.range = range;
		light.intensity = intensity;
		light.strength = Vector3f32(strength, strength, strength);
		light.rimLightColor = Vector3f32(color.GetX(), color.GetY(), color.GetZ());
		light.rimLightIntensity = 0.3f;
		return light;
	}

	LightData LightManager::CreateSpotLight(Vector3f32 const& position, Vector3f32 const& direction, Vector4 const& color, float32 spotAngle, float32 penumbraAngle, float32 range, float32 intensity, float32 strength)
	{
		LightData light = {};
		light.type = LightType::SPOT;
		light.position = position;
		light.direction = direction;
		light.color = color;
		light.spotAngle = spotAngle;
		light.penumbraAngle = penumbraAngle;
		light.range = range;
		light.intensity = intensity;
		light.strength = Vector3f32(strength, strength, strength);
		light.rimLightColor = Vector3f32(color.GetX(), color.GetY(), color.GetZ());
		light.rimLightIntensity = 0.4f;
		return light;
	}
	void LightManager::UpdateLightCB()
	{
		FrameResourcesManager& instance = FrameResourcesManager::Instance();

		for (int32 i = 0; i < FrameResourcesManager::GetNumFrameResources(); ++i)
		{
			instance.m_frameResources[i].m_pLightsCB->CopyData(Instance().m_lightPassData);
		}
	}
}
