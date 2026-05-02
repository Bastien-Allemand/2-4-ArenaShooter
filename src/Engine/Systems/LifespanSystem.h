#ifndef ENGINE_LIFESPAN_SYSTEM_H_INCLUDED
#define ENGINE_LIFESPAN_SYSTEM_H_INCLUDED

#include "define.h"
#include "Containers/Queue.hpp"

namespace gce {

class GameObject;


class LifespanSystem final
{
    LifespanSystem() = default;
    ~LifespanSystem() = default;

    void HandleCreations();
    void HandleDeletions();

    struct {
        Queue<GameObject*> gameObjects;
    } m_toCreate;

    struct {
        Queue<GameObject*> gameObjects;
    } m_toDelete;

    friend class GameObject;
    friend class GameManager;
};


}

#endif