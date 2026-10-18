// // Deprecated: Functional logic has been moved to the Scene class.

/*
#pragma once

#include "ObjectPool.hpp"
#include "GameObject.hpp"

#include <vector>
#include <memory>
#include <algorithm>
#include <list>

#include "Player.hpp"

// TODO : change it to singleton pattern
class ObjectManager
{
private:
    std::vector<GameObject*> m_DrawList;

    std::unique_ptr<Player> m_Player;

    // ObjectPool<Enemy> m_EnemyPool;

    // ObjectPool<Bullet> m_BulletPool;

public:
    void Initialize() { }

    void Update(double deltaTime)
    {
        if (m_Player && m_Player->IsActive())
        {
            m_Player->Update(deltaTime);
        }

        // m_EnemyPool.UpdateAll();
    }

    void Draw()
    {
        if (m_Player && m_Player->IsActive())
        {
            m_Player->Draw();
        }
    }

    Player* GetPlayer() { return m_Player.get(); }
};
*/
