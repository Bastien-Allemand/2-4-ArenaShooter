#ifndef ENGINE_GAME_MANAGER_INL_INCLUDED
#define ENGINE_GAME_MANAGER_INL_INCLUDED

namespace gce {


inline void GameManager::Create() { if (s_pInstance != nullptr) return; s_pInstance = new GameManager();}
inline void GameManager::Run(WindowParam& param) { if ( s_pInstance == nullptr ) return; s_pInstance->GameLoop(param); }
inline void GameManager::Destroy() { delete s_pInstance; }

inline float32 GameManager::DeltaTime() { return s_pInstance->m_deltaTime; }
inline float32 GameManager::FixedDeltaTime() { return s_pInstance->m_fixedDeltaTime; }
inline float32 GameManager::FPS() { return s_pInstance->m_fps; }
inline Window* GameManager::GetWindow() { return s_pInstance->m_pWindow; }
inline WindowParam& GameManager::GetWindowParam() { return s_pInstance->m_windowParam; }
inline RenderSystem& GameManager::GetRenderSystem() { return s_pInstance->m_renderSystem; }
inline LifespanSystem& GameManager::GetLifespanSystem() { return s_pInstance->m_lifespanSystem; }
inline ScriptSystem& GameManager::GetScriptSystem() { return s_pInstance->m_scriptSystem; }
inline PhysicSystem& GameManager::GetPhysicSystem() { return s_pInstance->m_physicSystem; }
inline StatesSystem& GameManager::GetStatesSystem() { return s_pInstance->m_statesSystem; }
inline UiSystem& GameManager::GetUiSystem() { return s_pInstance->m_uiSystem; }
inline Vector<Camera*>& GameManager::GetMainCameras() { return s_pInstance->m_mainCameras; }

}

#endif