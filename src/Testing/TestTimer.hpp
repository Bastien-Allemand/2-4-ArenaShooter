#ifndef TEST_TIMER_HPP_INCLUDED
#define TEST_TIMER_HPP_INCLUDED

#include <Render.h>
#include <Engine.h>
#include "define.h"
using namespace gce;


BEGIN_TEST( Timer )

 //    GameManager::Create();
 //    Scene& scene = Scene::Create();
 //    Geometry* const pCube = GeometryFactory::CreateCubeGeo();
 //    D12PipelineObject* const pDefaultPSO = new D12PipelineObject(GameManager::s_pInstance->vertexShader, GameManager::s_pInstance->pixelShader, GameManager::s_pInstance->hullShader, GameManager::s_pInstance->domainShader, GameManager::s_pInstance->rootSignature);;
 //
 //    GameObject& third = GameObject::Create(scene, [](GameObject* pGameObject) {
 //
 //        // Listner for non static methods
 //        if (GetKeyDown(Keyboard::SPACEBAR) && pGameObject->GetComponent<MeshRenderer>()->IsActive()) 
 //        {
	// 		// Clear all timers before adding new ones
 //            GameManager::GetTimerSystem().Clear();
 //
	// 		// Add a timer to fade out the mesh renderer after 2 seconds
 //            GameManager::GetTimerSystem().AddTimer(2.0f, &FadeOut, pGameObject);
 //
 //            // Add a timer to print text after 1 second, indepedently of the first one.
 //            // Timers are not queued or chained — they run concurrently.
 //            GameManager::GetTimerSystem().AddTimer(1.0f, &PrintTestText);
 //            
 //            // Add a timer to print text after 2 seconds
 //            GameManager::GetTimerSystem().AddTimer(2.0f, &PrintTimerEndingText);
 //        }
	// 	else if (GetKey(Keyboard::P) && !pGameObject->GetComponent<MeshRenderer>()->IsActive()) {
 //            GameManager::GetTimerSystem().Clear();
 //            GameManager::GetTimerSystem().AddTimer(2.0f, &FadeIn, pGameObject);
 //            GameManager::GetTimerSystem().AddTimer(2.0f, &PrintTimerEndingText);
 //        } });
 //
 //    
 //    MeshRenderer& meshRenderer3 = third.AddComponent<MeshRenderer>();
 //    meshRenderer3.pGeometry = pCube;
	// meshRenderer3.pPSO = pDefaultPSO;
 //
 //    GameObject& second = GameObject::Create(scene, [](GameObject* pGameObject) {
 //
 //        });
 //    second.transform.SetLocalPosition({ 2, 0, 0 });
 //
 //    MeshRenderer& meshRenderer2 = second.AddComponent<MeshRenderer>();
 //    meshRenderer2.pGeometry = pCube;
	// meshRenderer2.pPSO = pDefaultPSO;
 //
 //    WindowParam windowParam;
 //    GameManager::Run(windowParam);
 //
 //    delete pCube;
	// delete pDefaultPSO;
 //
 //    GameManager::Destroy();

END_TEST_MAIN_FUNCTION( Timer )

inline static bool s_cubeHasDisappeared = false;

static void FadeOut( GameObject* const pThis )
{
	if ( pThis == nullptr ) return;
	pThis->GetComponent<MeshRenderer>()->SetActive( false );
	s_cubeHasDisappeared = true;
}

static void FadeIn( GameObject* const pThis )
{
	if ( pThis == nullptr ) return;
	pThis->GetComponent<MeshRenderer>()->SetActive( true );
	s_cubeHasDisappeared = false;
}

static void PrintTestText() { Console::Log( "Test timer ended successfully." ); }

static void PrintTimerEndingText() { Console::Log( s_cubeHasDisappeared ? "The Cube has disappeared." : "The Cube has (re)appeared." ); }

END_TEST( Timer )


#endif