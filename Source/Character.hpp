#pragma once

# include "GameObject.hpp"
# include "Vector.hpp"

// Player, Enemy ‚È‚Ç‚ÌŠî’êƒNƒ‰ƒX
class Character : public GameObject
{
protected:
    int mn_HP;           
    // StateMachine m_StateMachine;

public:
    Character(float x, float y)
        : GameObject(), mn_HP(3){
        SetPosition(Vector2f(x, y));
    }

    virtual ~Character() {}

    void Update(double deltaTime) override
    {
        if (!mb_IsActive) return;
        GameObject::Update(deltaTime);
    }

    void Draw(const Camera& camera) override
    {
        if (!mb_IsActive) return;
        GameObject::Draw(camera);

    }

    virtual void TakeDamage(int damage)
    {
        mn_HP -= damage;
        if (mn_HP <= 0)
        {
            mb_IsActive = false;
        }
    }
};