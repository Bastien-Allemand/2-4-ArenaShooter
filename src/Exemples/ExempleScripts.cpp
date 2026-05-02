#include "Exemple.h"

#include <Render.h>
#include <Engine.h>

#include <Script.h>
using namespace gce;

// MACRO de déclaration de scripts :
// Pour faire un script vous devez mettre
// Nom du script 
// Les flags servent à savoir quelle types de fonctions sont dans les scripts
// pour accumuler plusieurs scripts on ajoute | entre les flags
// ici un script de Start et d'updtate
DECLARE_SCRIPT(ScriptTemplate, ScriptFlag::Start | ScriptFlag::Update)

// Il faut ensuite déclarer vos fonctions dans les scripts :
// fonction appelé au start de k'objet
void Start()
{
	
	// Code Here...
}

//fonction appelé à chaque frame
void Update()
{
	// Code Here...
}

END_SCRIPT
// Chaque flag a une fonction liées, Ex : Script::CollisionEnter -> void CollisionEnter() {}



DECLARE_SCRIPT(TestScript, ScriptFlag::Start | ScriptFlag::Update)

void Start()
{
	std::cout << "Starting In Script \n";
}

void Update()
{
	std::cout << "Updating In Script \n";
}
END_SCRIPT

void Exemple::Scripts(Scene* pScene)
{
	GameObject& gameObject = GameObject::Create(*pScene);
	gameObject.AddScript<TestScript>();
}