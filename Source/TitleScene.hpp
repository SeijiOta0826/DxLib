#pragma once
#include "Game.hpp"
#include "Scene.hpp"

#include "BackgroundComponent.hpp"
#include "Camera.hpp"

#include "AudioManager.hpp"

#include "InputMapping.hpp"
#include <vector>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <type_traits>


// タイトルシーン
class TitleScene : public Scene {
public:
    void Initialize() override;
    void Update(double deltaTime) override;
    void Draw() override;
    void Finalize() override;

private:
    std::unique_ptr<Camera> m_Camera;
};


void TitleScene::Initialize() {
    // create background
    int bgHandle = ResourceManager::GetInstance().LoadGraphic("Resource/images/Title_background.png");
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
}

void TitleScene::Update(double deltaTime) {
    Scene::Update(deltaTime);

    // 決定（シーン遷移）
    if (InputMapping::GetInstance().IsJustPressed(InputAction::CONFIRM)) {
        SceneManager::GetInstance().ChangeScene(SceneType::SCENETYPE_GAME_STAGE1);
		AudioManager::GetInstance()->PlaySound("Resource/sounds/SE/決定.mp3");
    }
}

void TitleScene::Draw() {
    for (const auto& obj : objects) {
        obj->Draw(*m_Camera);
    }
}

void TitleScene::Finalize() {
}

