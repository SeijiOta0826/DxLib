#pragma once

#include "Tile.hpp"
#include "Vector.hpp" // Vector2fを使うため
#include <cmath>
#include <cstdlib>

// ========================================================
// ① 前方宣言エリア（クラスの中身は知らないけど、ポインタは使うよと教える）
// ========================================================
class Player;
class ColliderComponent;
class PhysicsComponent;

// ========================================================
// ② クラスの設計図エリア（関数の「中身」は絶対にここに書かない！）
// ========================================================
class Icicle : public Tile {
private:
    enum class State { Idle, Warning, Falling, Broken };
    State m_state = State::Idle;

    // ①で前方宣言したので、エラーにならずにポインタを登録できる
    Player* m_targetPlayer;

    float m_triggerDistX = 50.0f;
    float m_warningTime = 0.5f;
    float m_timer = 0.0f;
    float m_velocityY = 0.0f;
    float m_gravity = 50.0f;
    float m_maxSpeed = 100.0f;
    Vector2f m_initialPos;

    bool isActive;     // 表示するか

public:
    // ここは宣言のみ
    Icicle(float x, float y, std::string filename, Player* player);
    virtual void Update(double deltaTime) override;
    virtual void Draw(const Camera& camera) override;

private:
    // ここも宣言のみ
    void UpdateIdle();
    void UpdateWarning(double deltaTime);
    void UpdateFalling(double deltaTime);
    void UpdateBroken(double deltaTime); // ← これを追加
};


// ========================================================
// ③ 実体を操作するためのインクルードエリア（循環参照を防ぐため、クラス定義の「後」に置く）
// ========================================================
#include "Player.hpp"
#include "ColliderComponent.hpp"
#include "PhysicsComponent.hpp"


// ========================================================
// ④ 関数の実装エリア（必ず inline を付けて、 クラス名:: を書く）
// ========================================================

inline Icicle::Icicle(float x, float y, std::string filename, Player* player)
    : Tile(x, y, filename)
    , m_targetPlayer(player)
{
    SetTag("Icicle");
    m_initialPos = Vector2f(x, y);

    isActive = true;

    if (auto phys = GetComponent<PhysicsComponent>()) {
        phys->SetEnabled(false);
    }
}

inline void Icicle::Update(double deltaTime) {
    if (!mb_IsActive) return;

    switch (m_state) {
    case State::Idle:    UpdateIdle(); break;
    case State::Warning: UpdateWarning(deltaTime); break;
    case State::Falling: UpdateFalling(deltaTime); break;
    case State::Broken:  UpdateBroken(deltaTime); break;
    }
    Tile::Update(deltaTime);
}

inline void Icicle::UpdateIdle() {
    if (m_targetPlayer == nullptr) return;

    Vector2f myPos = GetPosition();
    Vector2f playerPos = m_targetPlayer->GetPosition();
    isActive = true;

    if (playerPos.y > myPos.y && std::abs(playerPos.x - myPos.x) < m_triggerDistX) {
        m_state = State::Warning;
        m_timer = m_warningTime;
    }
}

inline void Icicle::UpdateWarning(double deltaTime) {
    m_timer -= static_cast<float>(deltaTime);
    float shakeX = (rand() % 5) - 2.0f;
    SetPosition(Vector2f(m_initialPos.x + shakeX, m_initialPos.y));

    if (m_timer <= 0.0f) {
        SetPosition(m_initialPos);
        m_state = State::Falling;
    }
}

inline void Icicle::UpdateFalling(double deltaTime) {
    float dt = static_cast<float>(deltaTime);

    m_velocityY += m_gravity * dt;
    if (m_velocityY > m_maxSpeed) m_velocityY = m_maxSpeed;

    Vector2f pos = GetPosition();
    pos.y += m_velocityY * dt;
    SetPosition(pos);

    // ③で完全な情報が読み込まれているので、ここで GetComponent を使っても波線が出ない
    auto myCollider = GetComponent<ColliderComponent>();
    auto playerCollider = m_targetPlayer->GetComponent<ColliderComponent>();

    if ((myCollider && playerCollider && myCollider->CheckCollision(playerCollider))
        || this->GetTransform()->mv_position.y > 1000.f) {
        m_state = State::Broken;
        if ((myCollider && playerCollider && myCollider->CheckCollision(playerCollider)))
        {
            ProgressManager::GetInstance().data.currentHp -= 10;
			AudioManager::GetInstance()->PlaySound("Resource/sounds/SE/被ダメ.mp3");
            m_targetPlayer->DamageEffectStart();
        }
        return;
    }
}

inline void Icicle::UpdateBroken(double deltaTime) {
    // 1. 座標と速度を初期値に戻す
    SetPosition(m_initialPos);
    m_velocityY = 0.0f;
    // 2. 状態を「待機」に戻す
    m_state = State::Idle;
    isActive = false;
}

void Icicle::Draw(const Camera& camera)
{
    if (!mb_IsActive || !isActive) return;
    Tile::Draw(camera);
}
