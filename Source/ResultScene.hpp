#pragma once

# include "Game.hpp"
# include "Scene.hpp"
# include "BackgroundComponent.hpp"
# include "Camera.hpp"

class ResultScene : public Scene
{
public:
    void Initialize() override;
    void Update(double deltaTime) override;
    void Draw() override;
    void Finalize() override;

private:
    std::unique_ptr<Camera> m_Camera;
    Player* mp_Player = nullptr;
};

void ResultScene::Initialize() {
    // create background
    int bgHandle = ResourceManager::GetInstance().LoadGraphic("Resource/images/ClearScreen.png");
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

    AudioManager::GetInstance()->StopMusic();
    AudioManager::GetInstance()->PlaySound("Resource/sounds/BGM/ClearSound.m4a.mp3");
}

void ResultScene::Update(double deltaTime) {
    Scene::Update(deltaTime);
    if (InputMapping::GetInstance().IsJustPressed(InputAction::CONFIRM)) {
        SceneManager::GetInstance().ChangeScene(SceneType::SCENETYPE_TITLE);
    }
}

void ResultScene::Draw() {
    for (const auto& obj : objects) {
        obj->Draw(*m_Camera);
    }

    // 描画する直前に、システム全体の標準フォントサイズを32に変更
    SetFontSize(96);

    // 通常の描画関数を使う
    DrawFormatString(660, 540, GetColor(255, 255, 255), "input count: %d", ProgressManager::GetInstance().data.inputCount);

    // 描画が終わったら、元のサイズ（デフォルトは16）に戻しておく
    SetFontSize(16);
}

void ResultScene::Finalize() {
}