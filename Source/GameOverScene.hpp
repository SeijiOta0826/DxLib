#pragma once
# include "Game.hpp"
# include "Scene.hpp"
# include "BackgroundComponent.hpp"
# include "Camera.hpp"

class GameOverScene : public Scene
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

void GameOverScene::Initialize(){
    // create background
    int bgHandle = ResourceManager::GetInstance().LoadGraphic("Resource/images/GameOverScreen.png");
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

	ProgressManager::GetInstance().data.currentHp = 99; // プレイヤーデータをリセット
    AudioManager::GetInstance()->StopMusic();
	AudioManager::GetInstance()->PlayMusic("Resource/sounds/BGM/GameOverScene.mp3", true);
}

void GameOverScene::Update(double deltaTime) {
    Scene::Update(deltaTime);
    if (InputMapping::GetInstance().IsJustPressed(InputAction::CONFIRM)) {
        SceneManager::GetInstance().ChangeScene(SceneType::SCENETYPE_TITLE);
    }
}

void GameOverScene::Draw() {
    for (const auto& obj : objects) {
        obj->Draw(*m_Camera);
    }
}

void GameOverScene::Finalize() {
}