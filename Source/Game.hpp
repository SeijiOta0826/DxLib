#pragma once
# include "DxLib.h"

# include "Global.hpp"
# include "Vector.hpp"

# include "InputManager.hpp"
# include "GraphicsManager.hpp"
# include "ResourceManager.hpp"
# include "SceneManager.hpp"

# include "TitleScene.hpp"
# include "Stage1.hpp"
# include "ResultScene.hpp"
# include "GameOverScene.hpp"

# include <chrono>

static constexpr double TARGET_FPS = 60.0;
static constexpr double FIXED_TIMESTEP = 1.0 / TARGET_FPS;

// ゲームループを管理するクラス
// 新しいシーンを作った時ここで登録する必要がある

class Game {
public:
    Game();   // ctor
    ~Game();  // dtor

    void Run();

    void UpdateLogic(double deltaTime);
    void UpdateRender(double interpolationAlpha);

    void Draw();

	bool IsRunning() const { return mb_IsRunning; }
    

private:
	bool mb_IsRunning;
    bool mb_initSuccess;

    //SceneManager m_SceneManager;
    // SceneManager::GetInstance().
    
    std::chrono::steady_clock::time_point m_LastFrameTime;
    double md_Accumulator;
};


Game::Game() : mb_IsRunning(false), mb_initSuccess(false), md_Accumulator(0.0)
{
    //--------------------------------------------------------------//
    //--------------------------------------------------------------//
    // DxLib setup                                                  //
    ChangeWindowMode(TRUE);                                         //
    SetGraphMode(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, 32);  //
    SetMainWindowText("2DTest");                                    //
                                                                    //
    if (DxLib_Init() == -1)                                         //
    {                                                               //
        return;                                                     //
    }                                                               //
                                                                    //
    SetDrawScreen(DX_SCREEN_BACK);                                  //
    mb_initSuccess = true;                                          //
    mb_IsRunning = true;                                            //
                                                                    //
    m_LastFrameTime = std::chrono::steady_clock::now();             //
    //--------------------------------------------------------------//
    //--------------------------------------------------------------//


    GraphicsManager::GetInstance().Initialize(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT);
    
    // -----------------------------------------------------------
    // TODO : register scenes（ここで新しく作ったシーンを登録する）
    // test scene

    SceneManager::GetInstance().RegisterScene(SceneType::SCENETYPE_TITLE,
        []() { return std::make_unique<TitleScene>(); });
    SceneManager::GetInstance().RegisterScene(SceneType::SCENETYPE_GAME_STAGE1,
        []() { return std::make_unique<Stage1>(); });

    SceneManager::GetInstance().RegisterScene(SceneType::SCENETYPE_GAMEOVER,
        []() { return std::make_unique<GameOverScene>(); });
    SceneManager::GetInstance().RegisterScene(SceneType::SCENETYPE_RESULT,
        []() { return std::make_unique<ResultScene>(); });
    // -----------------------------------------------------------

    // set
    SceneManager::GetInstance().ChangeScene(SceneType::SCENETYPE_TITLE);
}

Game::~Game() {
    SceneManager::GetInstance().Finalize();
    DxLib_End();
}

// game loop
void Game::Run()
{
    while (mb_IsRunning && ProcessMessage() == 0) {
        auto currentTimePoint = std::chrono::steady_clock::now();
        double deltaTime = std::chrono::duration<double>(currentTimePoint - m_LastFrameTime).count();
        m_LastFrameTime = currentTimePoint;

        md_Accumulator += deltaTime;

        while (md_Accumulator >= FIXED_TIMESTEP) {
            UpdateLogic(FIXED_TIMESTEP);
            md_Accumulator -= FIXED_TIMESTEP;
        }

        double interpolationAlpha = md_Accumulator / FIXED_TIMESTEP;

        UpdateRender(interpolationAlpha);
        Draw();
        ScreenFlip();
    }
}

void Game::UpdateLogic(double deltaTime)
{
    InputManager::GetInstance().Update();
    InputMapping::GetInstance().Update();

    ResourceManager::GetInstance().Update();

    SceneManager::GetInstance().Update(deltaTime);

    if (InputManager::GetInstance().IsKeyTriggered(KEY_INPUT_ESCAPE))
    {
        mb_IsRunning = false;
    }
}

void Game::UpdateRender(double interpolationAlpha) {
}

void Game::Draw()
{
    ClearDrawScreen();
    SceneManager::GetInstance().Draw();
}