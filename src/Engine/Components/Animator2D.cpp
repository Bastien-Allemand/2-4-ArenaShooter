#include "pch.h"
#include "Animator2D.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "GameManager.h"
#include "Texture.h"

namespace gce {


void Animator2D::AddAnimation(String const& filePath)
{
	Texture* pNewTexture = new Texture(filePath);
	m_textures.PushBack(pNewTexture);
}

void Animator2D::SetFrameRate(float32 framesPerSecond)
{
	if (framesPerSecond <= 0.0f)
	{
		framesPerSecond = 0.1f;
	}

	m_frameRate = framesPerSecond;
	m_timeBetweenFrames = 1.0f / m_frameRate;
}


}