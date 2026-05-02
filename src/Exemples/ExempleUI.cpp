#include "Exemple.h"

#include <Render.h>
#include <Engine.h>

#include <Script.h>
using namespace gce;


static void TestButton1()
{
	std::cout << "CLICK RED\n";
}


static void TestButton2()
{
	std::cout << "CLICK BRICK\n";
}

void Exemple::UseUI(Scene* pScene)
{
	GameObject& buttonBrick = GameObject::Create(*pScene);
	UiButton* pBrickButton = buttonBrick.AddComponent<UiButton>();

	// Initialisation des brush pour le style du bouton
    BitMapBrush buttonBrush{ "res/Exemple/TexturesTest.jpg" }; // Image

	pBrickButton->AddListener(TestButton2);

	pBrickButton->pBitMapBrush = &buttonBrush;


}