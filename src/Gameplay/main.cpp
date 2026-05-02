#include <windows.h>
#include <Engine.h>
#include <Shapes.h>

DECLARE_SCRIPT(ObjectRotatorScript, ScriptFlag::Update)

void Update() {
	m_pOwner->transform.LocalRotate({ 0.0f, 0.01f, 0.0f });
	m_pOwner->transform.LocalTranslate({ 0.0f, 0.01f, 0.0f });

}

END_SCRIPT

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
    gce::Console::Init();
    gce::GameManager::Create();

    gce::Scene& scene = gce::Scene::Create();

	gce::Texture& texture = *new gce::Texture("res/Exemple/TexturesTest.jpg");

	gce::GameObject& gameObject = gce::GameObject::Create(scene);
	gameObject.SetName("Transparency");
	gce::MeshRenderer& meshRenderer = *gameObject.AddComponent<gce::MeshRenderer>();
    meshRenderer.SetGeometry(gce::SHAPES.CUBE);
	meshRenderer.SetAlbedoTexture(&texture);
	ObjectRotatorScript& rotatorScript = *gameObject.AddScript<ObjectRotatorScript>();

	gce::GameObject& cameraObject = gce::GameObject::Create(scene);
	cameraObject.SetName("MainCamera");
	cameraObject.transform.SetWorldPosition({ 0.0f, 0.0f, -5.0f });
	gce::Camera& camera = *cameraObject.AddComponent<gce::Camera>();
	camera.SetMainCamera();
	camera.SetType(gce::PERSPECTIVE);
	camera.perspective.fov = XM_PIDIV4;
	camera.perspective.nearPlane = 0.001f;
	camera.perspective.farPlane = 500.0f;
	camera.perspective.aspectRatio = 16.0f / 9.0f;
	camera.perspective.up = { 0.0f, 1.0f, 0.0f };

	gce::GameObject& directionalLightObject = gce::GameObject::Create(scene);
	directionalLightObject.SetName("DirectionalLight");
	gce::Light& directionalLight = *directionalLightObject.AddComponent<gce::Light>();
	directionalLight.DefaultDirectionLight();

    gce::WindowParam params;
    params.title = L"GCE Engine Window";
    params.width = 1280;
    params.height = 720;
    params.isFullScreen = true;
    params.isSplitScreen = false;
    params.screenDisposition = gce::SplitScreenDisposition::SQUARE_4_PLAYERS;

    gce::GameManager::Run(params);

    gce::GameManager::Destroy();
    gce::Console::UnInit();
    return 0;
}