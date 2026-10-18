#pragma once

#include "Scene.hpp"

#include <memory>
#include <unordered_map>
#include <functional>

class Camera;

// シーンを管理するクラス
// 新しいタイプのシーンを追加の時 SceneType を更新する

enum class SceneType {
    SCENETYPE_NONE = 0,
    SCENETYPE_TITLE,
    SCENETYPE_GAME_STAGE1,
    SCENETYPE_RESULT,
    SCENETYPE_GAMEOVER,
};

// singleton pattern
class SceneManager {
public:
    static SceneManager& GetInstance() {
        static SceneManager instance;
        return instance;
    }

    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

    void RegisterScene(SceneType type, std::function<std::unique_ptr<Scene>()> factory);
    void ChangeScene(SceneType type);
    void Update(double deltaTime);
    void Draw();

    void Finalize() {
        if (currentScene) {
            currentScene->Finalize();
            currentScene.reset();
        }
        sceneFactories.clear();
    }

    Scene* GetCurrentScene() { return currentScene.get(); }

private:
    SceneManager() = default;
    std::unique_ptr<Scene> currentScene;
    SceneType nextSceneType = SceneType::SCENETYPE_NONE;
    std::unordered_map<SceneType, std::function<std::unique_ptr<Scene>()>> sceneFactories;


private:
    void PerformChangeScene(SceneType type) {
        if (currentScene) {
            currentScene->Finalize();
        }

        auto it = sceneFactories.find(type);
        if (it != sceneFactories.end()) {
            currentScene = it->second();
            currentScene->Initialize();
        }
    }
};


void SceneManager::RegisterScene(SceneType type, std::function<std::unique_ptr<Scene>()> factory) {
    sceneFactories[type] = factory;
}

void SceneManager::ChangeScene(SceneType type) {
    nextSceneType = type;

    /*if (currentScene) {
        currentScene->Finalize();
    }

    auto it = sceneFactories.find(type);
    if (it != sceneFactories.end()) {
        currentScene = it->second();
        currentScene->Initialize();
    }*/
}

void SceneManager::Update(double deltaTime) {
    if (nextSceneType != SceneType::SCENETYPE_NONE) {
        PerformChangeScene(nextSceneType);
        nextSceneType = SceneType::SCENETYPE_NONE;
    }

    if (currentScene) {
        currentScene->Update(deltaTime);
    }
}

void SceneManager::Draw() {
    if (currentScene) {
        currentScene->Draw();
    }
}