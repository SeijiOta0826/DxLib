#pragma once
#include "Component.hpp"
#include "GraphicsManager.hpp"
#include "Vector.hpp"
#include "Camera.hpp"
#include "Global.hpp"

// 背景コンポーネント
// 使い方例：TestSceneをご参照ください（create background）

class BackgroundComponent : public Component {
private:
    int   mn_ImageHandle;
    float mf_ScrollFactor;
    Vector2f mv_WorldPos;

public:
    BackgroundComponent(int handle, float scrollFactor)
        : mn_ImageHandle(handle), mf_ScrollFactor(scrollFactor), mv_WorldPos(0.0f, 0.0f) {
    }

    void Initialize() override {
    }

    void Update(double deltaTime) override {
    }

    void Draw(const Camera& camera) {
        Vector2f cameraPos = camera.GetPosition();

        float drawX = mv_WorldPos.x - (cameraPos.x * mf_ScrollFactor) + (Config::SCREEN_WIDTH * 0.5f);
        float drawY = mv_WorldPos.y - (cameraPos.y * mf_ScrollFactor) + (Config::SCREEN_HEIGHT * 0.5f);

        GraphicsManager::GetInstance().DrawGraph(static_cast<int>(drawX), static_cast<int>(drawY), mn_ImageHandle, TRUE);
    }

    // Tiledマップを利用する時使えそう
    void DrawTiled(const Camera& camera, int bgWidth, int bgHeight) {
        Vector2f cameraPos = camera.GetPosition();

        int offsetX = static_cast<int>(cameraPos.x * mf_ScrollFactor) % bgWidth;
        int offsetY = static_cast<int>(cameraPos.y * mf_ScrollFactor) % bgHeight;

        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                GraphicsManager::GetInstance().DrawGraph(x * bgWidth - offsetX, y * bgHeight - offsetY, mn_ImageHandle, TRUE);
            }
        }
    }
};