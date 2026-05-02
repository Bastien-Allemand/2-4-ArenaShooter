#include "Exemple.h"

#include <Render.h>
#include <Engine.h>
#include <Audio.h>

#include <Script.h>
#include <string>
using namespace gce;

// Utilisation des script pour ajouter des fonctionnalité
DECLARE_SCRIPT(TestAudio, ScriptFlag::Start | ScriptFlag::Update)

void Start()
{
	// load et unload des sons
	// il faut utiliser des WString pour l'Audio et ajouter WRES_PATH pour que les path soit valides
	std::wstring soundPath = WRES_PATH L"res/Exemple/DROSHEKALIMO.mp3";
	std::wstring musicPath = WRES_PATH L"res/Exemple/Music.mp3";
	AudioUse::LoadSound("Son1", soundPath.c_str());
	AudioUse::LoadSound("Music1", musicPath.c_str());

	// Mettre les sons dans des catégories pour pouvoir modifier tous les sons de cette catégorie
	AudioUse::SetAudioCategory("Son1", gce::Category::SFX);

	// Play sound : soundName, loopSound, listener position
	AudioUse::Play("Son1");
	AudioUse::Play("Music1", true);

	AudioUse::SetMasterVolume(50); // Volume général : maximum 2^24
}

void Update()
{
	// gérer les sons
	if (GetKeyDown(Keyboard::E))
		AudioUse::Pause("Music1");
	if (GetKeyDown(Keyboard::R))
		AudioUse::Resume("Music1");
	if (GetKeyDown(Keyboard::T))
		AudioUse::Stop("Music1");
	if (GetKeyDown(Keyboard::Y))
		AudioUse::Play("Music1");

	// Gérer le son
	if (GetKeyDown(Keyboard::G))
		AudioUse::SetVolumeOfCategory(Category::SFX, 30);
	if (GetKeyDown(Keyboard::H))
		AudioUse::SetVolumeOfSound("Music1", 10);
}
END_SCRIPT

void Exemple::UseAudio(Scene* pScene)
{
	GameObject& AudioPlay = GameObject::Create(*pScene);
	AudioPlay.AddScript<TestAudio>();
}
