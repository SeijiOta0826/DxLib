#pragma once
# include "Component.hpp"

// オブジェクトのステートを管理するクラス
// プレイヤーが持っている時なら Held、投げ出されたら Thrown、など

enum class ActorState {
    Idle,           // 
    InAir,          // 
    Thrown,         // 
    Held,           // 
    PendingDestroy  // 
};

class StateComponent : public Component {
private:
    ActorState m_CurrentState = ActorState::InAir;

public:
    // Getter / Setter
    void SetState(ActorState newState) { m_CurrentState = newState; }
    ActorState GetState() const { return m_CurrentState; }

    bool CanBePickedUp() const {
        return m_CurrentState == ActorState::Idle || m_CurrentState == ActorState::InAir;
    }

    bool IsFlying() const {
        return m_CurrentState == ActorState::Thrown;
    }

    void Initialize() override {}
    void Update(double deltaTime) override {}
};