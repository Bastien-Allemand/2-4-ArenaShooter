#include <Console.h>
#include "Exemple.h"

// INCLUDES NECESSAIRES DE BASE !!!

#include <windows.h>
#include <Render.h>
#include <Engine.h>

// Exemple Initialisation de Base

// le namespace est utilis� dans tout le GC-Engine
// si vous n'utilisez pas using il faudra faire gce:: devant tous les �lements du GC-Engine
// vous avez le choix 
using namespace gce;

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	//// Initialisation du Moteur : 
	GameManager::Create();

	// Console utilis�e pour le debug
	Console::Init();

	// Creation d'un scene
	Scene& scene1 = Scene::Create();

	//// Creation d'un objet pour la cam�ra
	GameObject& cameraObject = GameObject::Create(scene1);
	cameraObject.transform.LocalTranslate({ 0, 0, -10 });
	// ajout du component Camera sur l'objet puis initialisation des propri�t�s de la camera
	Camera* pCamera = cameraObject.AddComponent<Camera>();

	pCamera->SetMainCamera();
	pCamera->SetType(PERSPECTIVE);
	// Fov de base ici pi/4
	pCamera->perspective.fov = XM_PIDIV4;
	// point le plus proche que la camera verra
	pCamera->perspective.nearPlane = 0.001f; 
	// point le plus �loign� que la camera verra
	pCamera->perspective.farPlane = 500.0f; 
	// aspect ratio set en fonction de la largeur et hauteur de la fen�tre
	pCamera->perspective.aspectRatio = 1000.0f/800.0f;
	// ici on d�finis que l'axe Y va g�rer la hauteur 
	pCamera->perspective.up = { 0.0f, 1.0f, 0.0f };

	//Creation d'un objet sans texture
	GameObject& testObject = GameObject::Create(scene1);

	MeshRenderer* pMeshRenderer = testObject.AddComponent<MeshRenderer>();
	// SHAPES contient plusieurs shapes de base (je vous laisse regarder) pour faire des formes de bases
	pMeshRenderer->SetGeometry(SHAPES.CUBE);

	Texture* pNewTexture = new Texture("res/Exemple/TexturesTest.jpg");

	// AlbedoTextureID est la texture � set de base les autres sont pour r�gler des effets en plus de material
	// si vous savez ce que c'est et vous en servir utilisez les sinon ne vous compliquez pas la vie et utilisez juste albedo
	// ATTENTION lorsque vous voulez utiliser une texture sur un MeshRenderer il faut utiliser useTexture = 1 pour l'activer et useTexture = 0 pour la d�sactiver
	pMeshRenderer->SetAlbedoTexture(pNewTexture);

	testObject.transform.SetWorldPosition({-2.0f,3.0f,0.0f});

	// creation d'un objet avec texture
	GameObject& texturedObject = GameObject::Create(scene1);
	texturedObject.transform.SetWorldPosition({ 2.0f,3.0f,0.0f });
	MeshRenderer* pMeshRenderer2 = texturedObject.AddComponent<MeshRenderer>();
	pMeshRenderer2->SetGeometry(SHAPES.CUBE);

	// load de la texture
	//Texture* pNewTexture = new Texture("res/Exemple/TexturesTest.jpg");
	// AlbedoTextureID est la texture � set de base les autres sont pour r�gler des effets en plus de material
	// si vous savez ce que c'est et vous en servir utilisez les sinon ne vous compliquez pas la vie et utilisez juste albedo
	// ATTENTION lorsque vous voulez utiliser une texture sur un MeshRenderer il faut utiliser useTexture = 1 pour l'activer et useTexture = 0 pour la d�sactiver
	pMeshRenderer2->SetAlbedoTexture(pNewTexture);

	// creation d'un objet avec geometry custom
	GameObject& shapeCustom = GameObject::Create(scene1);
	shapeCustom.transform.SetWorldPosition({ 0.0f,3.0f,0.0f });
	MeshRenderer* pMeshRenderer3 = shapeCustom.AddComponent<MeshRenderer>();
	// Pensez � inverser l'axe Y des uv quand vous loadez des OBJ (sinon �a casse vos textures)
	pMeshRenderer3->SetGeometry(GeometryFactory::LoadGeometry("res/Exemple/SUZANNE.obj"));
	pMeshRenderer3->SetAlbedoTexture(pNewTexture);

	// Exemple pr�cis de fonctionnalit�s
	//Exemple::Scripts(&scene1);
	//Exemple::Inputs(&scene1);
	//Exemple::UseAudio(&scene1);
	//Exemple::ParentChild(&scene1);


	// Propei�t�s de la fen�tre
	WindowParam windowParam;
	windowParam.title = L"Window Name";
	windowParam.width = 1000;
	windowParam.height = 800;

	GameManager::Run(windowParam);


	// Pour pas surcharger le main je vais ajouter plusieurs exemples ici :

	// Destruction du GameManager
	GameManager::Destroy();

	return 0;
}

