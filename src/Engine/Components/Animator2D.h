#ifndef ENGINE_ANIMATOR_2D_H_INCLUDED
#define ENGINE_ANIMATOR_2D_H_INCLUDED

#include "define.h"
#include "Base.h"

namespace gce {

class Texture;


DECLARE_COMPONENT_REQUIRE( Animator2D, ComponentBase, MeshRendererMask )
{
public:
	void AddAnimation(String const& filePath);
	void SetLooping(bool looping) { m_looping = looping; }

	// Méthodes pour contrôler la vitesse d'animation
	void SetFrameRate(float32 framesPerSecond);
	float32 GetFrameRate() const { return m_frameRate; }

private:
	bool m_looping = true;
	Vector<Texture*> m_textures;
	uint8 m_currentTexture = 0;
	float32 m_frameRate = 10.0f;
	float32 m_timeBetweenFrames = 0.1f; // Temps entre les images (1/m_frameRate)
	float32 m_timer = 0.0f;
	bool m_isPlaying = true;


	friend class RenderSystem;
};


}

#endif