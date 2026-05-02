#ifndef ENGINE_SCENE_H_INCLUDED
#define ENGINE_SCENE_H_INCLUDED

#include "define.h"

namespace gce {

class GameObject;


class Scene final
{
public:
    static Scene& Create();

    [[nodiscard]] GameObject* GetGameObject(uint32 id) const;

private:
    Scene() = default;
    ~Scene() = default;

    void UpdateMatrix();

    UnorderedMap<uint32, GameObject*> m_gameObjects;

    friend class LifespanSystem;
    friend class GameManager;
};


}

#endif