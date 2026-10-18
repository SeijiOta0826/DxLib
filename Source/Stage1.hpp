#pragma once
#include "Game.hpp"
#include "Vector.hpp"
#include "Scene.hpp"

#include "Player.hpp"
#include "Tile.hpp"
#include "Goal.hpp"
#include "MovingPlatform.hpp"
#include "Icicle.hpp"
#include "UnlockItem.hpp"
#include "TestItem.hpp"
#include "InputDisplayPanel.hpp"

#include "BackgroundComponent.hpp"
#include "Camera.hpp"

#include "Map.hpp"

#include <vector>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <type_traits>

// テスト用のシーン

class Stage1 : public Scene {
public:
    void Initialize() override;
    void Update(double deltaTime) override;
    void Draw() override;
    void Finalize() override;

    void Stage();
private:
    std::unique_ptr<Camera> m_Camera;
    Player* mp_Player = nullptr;

    MapData mapData; // ポインタではなく実体を持つ
};


void Stage1::Initialize() {
    // create background
    int bgHandle = ResourceManager::GetInstance().LoadGraphic("Resource/images/StarNight.png");
    int bgX, bgY;
    GraphicsManager::GetInstance().GetGraphSize(bgHandle, &bgX, &bgY);
    auto bgObject = std::make_unique<GameObject>();
    bgObject->SetZOrder(-1.f);
    bgObject->AddComponent<BackgroundComponent>(bgHandle, 1.f);
    AddObject(std::move(bgObject));
    SetWorldBounds(Vector2f(0.f, 0.f), Vector2f(bgX, bgY));

    // create camera
    m_Camera = std::make_unique<Camera>(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT);
    m_Camera->SetPosition(m_Camera->WorldToScreen(Vector2f(0.f, 0.f)));
    m_Camera->SetBounds(bgX, bgY);
    m_Camera->SetDeadzone(600, 400);

    // Init時に直接アドレス（参照）を渡して中身を書き換えてもらう
    if (!LoadMapFromCSV("Resource/maps/stage1.csv", mapData)) {
        // エラー処理（ファイルの読み込み失敗など）
        DrawString(20.f, 20.f, "Stage1.csvの読み込みに失敗しました", GetColor(0, 0, 0));
    }

    // create player
     auto player = std::make_unique<Player>(200, 400);
    AddObject(std::move(player));
    mp_Player = dynamic_cast<Player*>(FindObjectWithTag("Player"));

    // create test items
    for (int i = 0; i < 5; ++i) {
        //auto box = std::make_unique<TestItem>(200.f + i * 50.f, 1800.f);
        auto box = std::make_unique<TestItem>(400, 1600.f + i * 50.f);
        AddObject(std::move(box));
    }

	auto inputPanel = std::make_unique<InputDisplayPanel>(Config::SCREEN_WIDTH - 320.f, Config::SCREEN_HEIGHT - 120.f, "Resource/images/BlackPanel.png");
    AddObject(std::move(inputPanel));

    //AudioManager::GetInstance().

    Stage();

    AudioManager::GetInstance()->StopMusic();
	AudioManager::GetInstance()->PlayMusic("Resource/sounds/BGM/GameScene.mp3",true);
    // TODO : create other objects ( enemies, ... )
}

void Stage1::Update(double deltaTime) {
   
    if (mp_Player) {
        Vector2f pos = mp_Player->GetPosition();
        m_Camera->Update(pos, 0.2f);
    }

    Scene::Update(deltaTime);

    if (InputMapping::GetInstance().IsJustPressed(InputAction::BACK)) {
        SceneManager::GetInstance().ChangeScene(SceneType::SCENETYPE_TITLE);
    }

    if (mp_Player->GetPosition().y > 1000.f){
        ProgressManager::GetInstance().data.currentHp -= 10;
        mp_Player->SetPosition(Vector2f(200.f, 400.f));
		AudioManager::GetInstance()->PlaySound("Resource/sounds/SE/被ダメ.mp3");
    }
}

void Stage1::Draw() {
    for (const auto& obj : objects) {
        obj->Draw(*m_Camera);
    }
}

void Stage1::Finalize() {
}

void Stage1::Stage() {
    const float TILE_SIZE = 60.f;

    for (size_t i = 0; i < mapData.tiles.size(); ++i) {
        int tileType = mapData.tiles[i];

        // 空白（例えば0を何もない空間とする場合）はスキップ
        if (tileType == 0) continue;

        // 1次元のインデックスから、2次元の列(col)と行(row)を算出
        int col = i % mapData.width;
        int row = i / mapData.width;

        // 座標の計算
        float posX = col * TILE_SIZE;
        float posY = row * TILE_SIZE;

        switch (tileType) {
        case 1: { // 床タイルの場合
            std::string filename = "Resource/images/Tile.png";
            auto tile = std::make_unique<Tile>(posX, posY, filename);
            AddObject(std::move(tile));
            break;
        }


        case 2: { // Keyタイルの場合
            std::string filename = "Resource/images/Folder.png";
            auto tile = std::make_unique<Tile>(posX, posY, filename);
            AddObject(std::move(tile));
            break;
        }

        case 3: { // ゴールの場合
            auto tile = std::make_unique<Goal>(posX, posY, "Resource/images/GoalFlag.png");
            AddObject(std::move(tile));
            break;
        }

        case 4: { // エレベーターの場合
            auto tile = std::make_unique<MovingPlatform>(posX, posY,
                std::vector<Vector2f>{Vector2f(-200.0f, 0.0f), Vector2f(-200.0f, 350.0f), Vector2f(200.0f, 350.0f), Vector2f(200.0f, 0.0f)},
                "Resource/images/Elevator.png");
            AddObject(std::move(tile));
            break;
        }

        case 5: { // つららの場合
            auto tile = std::make_unique<Icicle>(posX, posY, "Resource/images/Icicle.png", mp_Player);
            AddObject(std::move(tile));
            break;
        }

        case 6: { // エレベーターの場合
            auto tile = std::make_unique<MovingPlatform>(posX + 15, posY,
                std::vector<Vector2f>{Vector2f(0.0f, 0.0f), Vector2f(0.0f, 330.0f), Vector2f(330.0f, 330.0f), Vector2f(330.0f, 0.0f)},
                "Resource/images/Elevator.png");
            AddObject(std::move(tile));
            break;
        }


        case 7: { // エレベーターの場合
            auto tile = std::make_unique<MovingPlatform>(posX + 15, posY,
                std::vector<Vector2f>{Vector2f(0.0f, 0.0f), Vector2f(0.0f, 330.0f), Vector2f(330.0f, 330.0f), Vector2f(330.0f, 0.0f),
                Vector2f(660.f, 0.0f), Vector2f(660.f, 330.0f), Vector2f(990.f, 330.0f), Vector2f(990.f, 0.0f)},
                "Resource/images/Elevator.png");
            AddObject(std::move(tile));

            break;
        }


        case 8: { // エレベーターの場合
            auto tile = std::make_unique<MovingPlatform>(posX + 15, posY,
                std::vector<Vector2f>{Vector2f(0.0f, 150.0f), Vector2f(0.0f, 330.0f), Vector2f(0.0f, -150.0)},
                "Resource/images/Elevator.png");
            AddObject(std::move(tile));

            break;
        }

        case 9: { // エレベーターの場合
            auto tile = std::make_unique<MovingPlatform>(posX + 15, posY,
                std::vector<Vector2f>{Vector2f(0.0f, 330.0f), Vector2f(0.0f, -330.0f), Vector2f(0.0f, 0.0f)},
                "Resource/images/Elevator.png");
            AddObject(std::move(tile));

            break;


        }
              /*        case 5: { // エレベーターの場合
                          auto tile = std::make_unique<Icicle>(posX, posY, mp_Player, "Resource/images/Elevator.png");
                          AddObject(std::move(tile));
                          break;
                      }*/

        case 10: { // 左キーの場合
            auto tile = std::make_unique<UnlockItem>(posX, posY, InputAction::MOVE_LEFT, "Resource/images/プレイヤー「A」.png");
            AddObject(std::move(tile));
            break;
        }

        case 11: { // ジャンプの場合6
            auto tile = std::make_unique<UnlockItem>(posX, posY, InputAction::JUMP, "Resource/images/Space.png");
            AddObject(std::move(tile));
            break;
        }

        case 12: { // 攻撃の場合
            auto tile = std::make_unique<UnlockItem>(posX, posY, InputAction::ATTACK, "Resource/images/プレイヤー「L」.png");
            AddObject(std::move(tile));
            break;
        }

        case 13: { // 投げの場合
            auto tile = std::make_unique<UnlockItem>(posX, posY, InputAction::PICKUP, "Resource/images/プレイヤー「J」.png");
            AddObject(std::move(tile));
            break;




        }

        }
    }
}