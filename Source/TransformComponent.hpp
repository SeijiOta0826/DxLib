#pragma once

# include "Component.hpp"
# include "Vector.hpp"

#include <vector>

// オブジェクトの移動を管理するコンポーネント
// 移動する必要があるオブジェクトにこのオブジェクトをくっつく

class TransformComponent : public Component {
public:
    Vector2f mv_position = { 0.0, 0.0 };

    float mf_rotation = 0.0;
    Vector2f mv_scale = { 1.0, 1.0 };

    void Initialize() override {}
    void Update(double deltaTime) override {}

    void Draw(const Camera& camera) override {}

    void Translate(float dx, float dy) {
        mv_position.x += dx;
        mv_position.y += dy;
    }
};