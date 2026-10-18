# pragma once
#include "Tile.hpp"
#include "Scene.hpp"
#include "SceneManager.hpp"

class Goal : public Tile { // GameObjectを継承している前提
private:
    float x = 0.f, y = 0.f;
    const float SIZE = 50.f;

public:
    Goal(float posX, float posY, std::string filename) : Tile(posX, posY, filename) {
        SetTag("Goal");
    }

    void Update(double deltaTime) override
    {
        auto player = GetScene()->FindObjectWithTag("Player"); // プレイヤーオブジェクトを取得
        if (player) {
            // アドレス（ポインタ）を渡して座標を比較させる
            if (GetComponent<ColliderComponent>()->CheckCollision(player->GetComponent<ColliderComponent>())) {
                SceneManager::GetInstance().ChangeScene(SceneType::SCENETYPE_RESULT);
                AudioManager::GetInstance()->PlaySound("Resource/sounds/SE/ClearSound.m4a");
                return;
            }
        }
        Tile::Update(deltaTime);    
    }
};