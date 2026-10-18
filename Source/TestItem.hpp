#pragma once
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "StateComponent.hpp"

// テスト用のアイテム：物理コンポーネント・当たり判定が適用される

class TestItem : public GameObject {
private:
    bool mb_IsHeld = false;
    GameObject* mp_Holder = nullptr;

public:
    TestItem(float x, float y) : GameObject() {
        SetTag("Prop");
        SetPosition({ x, y });

        auto col = AddComponent<ColliderComponent>(GetTransform(), Vector2f(32.f, 32.f));
        auto phys = AddComponent<PhysicsComponent>(75.0f);

        AddComponent<StateComponent>();
    }

    void Update(double deltaTime) override {
        if (mb_IsHeld && mp_Holder) {
            Vector2f holderPos = mp_Holder->GetPosition();
            SetPosition({ holderPos.x, holderPos.y - 40.f });

            auto phys = GetComponent<PhysicsComponent>();
            if (phys) phys->SetVelocity({ 0, 0 });
        }

        Vector2f displacement = GetComponent<PhysicsComponent>()->GetDisplacement(deltaTime);
        GetTransform()->Translate(displacement.x, displacement.y);

        GameObject::Update(deltaTime);
    }

    void PickUp(GameObject* holder) {
        mb_IsHeld = true;
        mp_Holder = holder;
        SetCollidable(false);
    }

    void Throw(const Vector2f& force) {
        mb_IsHeld = false;
        mp_Holder = nullptr;
        SetCollidable(true);

        auto phys = GetComponent<PhysicsComponent>();
        if (phys) {
            phys->ApplyForce(force);
        }
    }

    bool IsHeld() const { return mb_IsHeld; }

    void OnCollision(GameObject* other) {
        if ((GetComponent<StateComponent>()->GetState() == ActorState::Thrown) && !other->CompareTag("Player")) {
            // SpawnExplosionEffect(); // effect
            if (other->GetComponent<StateComponent>()->GetState() != ActorState::Held) this->Destroy();
        }
    }
};