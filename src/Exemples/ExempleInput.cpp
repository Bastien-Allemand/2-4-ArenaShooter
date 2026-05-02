#include "Exemple.h"

#include <Render.h>
#include <Engine.h>

#include <Script.h>
using namespace gce;

// Utilisation des script pour ajouter des fonctionnalité
DECLARE_SCRIPT(InputExemple, ScriptFlag::Update)

void Update()
{

	// Gestion du clavier
	// l'enum Keyboard gère toutes les touches du clavier
	if (GetKey(Keyboard::A)) // Verifier si une touche est appuyée 1 fois
		std::cout << "Press A once";
	if (GetKeyDown(Keyboard::A)) // Vérifier si on reste appuyé  sur une touche
		std::cout << "Keep pressing A";
	if (GetKeyUp(Keyboard::A)) // Vérifier sur une touche est relachée
		std::cout << "Release A once";

	//Gestion de la souris
	// l'enum Mouse gère toutes les touches du clavier
	if (GetButton(Mouse::LEFT)) // Verifier si une touche est appuyée 1 fois
		std::cout << "Press left-click once";
	if (GetButtonDown(Mouse::LEFT)) // Vérifier si on reste appuyé  sur une touche
		std::cout << "Keep pressing left-click";
	if (GetButtonUp(Mouse::LEFT)) // Vérifier sur une touche est relachée
		std::cout << "Release left-click once";

	// empêcher le cursor de bouger ou non le curseur + IsMouseCursorLocked() pour check si il est lock
	if (GetKey(Keyboard::L))
		LockMouseCursor();
	if (GetKey(Keyboard::U))
		UnlockMouseCursor();

	// Montrer ou cacher le curseur + IsMouseCursorVisible() pour check si il est caché ou non
	if (GetKey(Keyboard::S))
		ShowMouseCursor();
	if (GetKey(Keyboard::H))
		HideMouseCursor();


	// Gestion du Gamepad
	// Même vérification que pour le clavier ou la souris
	// on ajoute l'index de la manette à laquelle on veut vérifier les inputs
	GetButton(Gamepad::Button::A, 0);
	GetButtonUp(Gamepad::Button::A, 0);
	GetButtonDown(Gamepad::Button::A, 0);
	// il est possible de ne pas ajouter d'index pour vérifier si au moins une manette fait l'input
	GetButton(Gamepad::Button::A);
	
	// Vérifier à quel point les gachettes sont enfoncées entre 0.0 et 1.0f
	GetLeftTrigger(0/*id de la manette*/);
	GetRightTrigger(0);

	// Vérifier la position X et Y de chaque stick et renvoie la position entre -1.0f et 1.0f
	GetLeftStickX(0/*id de la manette*/);
	GetLeftStickY(0);
	GetLeftStick(0);
	GetRightStickX(0);
	GetRightStickY(0);
	GetRightStick(0);

	//Vérfication si un gamePad est connecté
	IsGamepadConnected();
	// Vérification si un gamepad spécifique est connecté
	IsGamepadConnected(0/*id de la manette*/);

	// Activer ou désactiver des vibrations sur une manette
	SetGamepadVibration(0.5f /*force de la vibration à gauche entre 0.0f et 1.0f*/,
						0.5f /*force de la vibration à droite entre 0.0f et 1.0f*/, 
						0 /*id de la manette*/);
	StopGamepadVibration(0);


}

END_SCRIPT

void Exemple::Inputs(Scene* pScene)
{
	GameObject& inputObject = GameObject::Create(*pScene);
	inputObject.AddScript<InputExemple>();
}