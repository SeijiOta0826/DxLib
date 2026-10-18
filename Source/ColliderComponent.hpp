#pragma once

#include "Component.hpp"
#include "TransformComponent.hpp"
#include "Bounds2.hpp"

// コライダーコンポーネント（ボックス）
// 当たり判定が必要なオブジェクトにくっつく
// ctor に AddComponent<ColliderComponent>(transformComp, Vector2f(Size_x, Size_y)); を入れる

class ColliderComponent : public Component {
private:
    Vector2f mv_Size;
    Vector2f mv_Offset;
    TransformComponent* mp_Transform;

public:
    ColliderComponent(TransformComponent* trans, const Vector2f& size, const Vector2f& offset = { 0, 0 })
        : mp_Transform(trans), mv_Size(size), mv_Offset(offset) {}

    void Initialize() override {}

    Bounds2 GetAbsoluteBounds() const {
        Vector2f center = mp_Transform->mv_position + mv_Offset;
        return Bounds2::FromCenterSize(center, mv_Size);
    }

    bool CheckCollision(const ColliderComponent* other) const {
        if (!other) return false;
        return GetAbsoluteBounds().Overlaps(other->GetAbsoluteBounds());
    }

    void Update(double deltaTime) override {}

    void Draw(const Camera& camera) override {
        // テスト用
        //GetAbsoluteBounds().DrawAABB(camera);
    }

    // Getter / Setter
    void SetSize(const Vector2f& size) { mv_Size = size; }
    void SetOffset(const Vector2f& offset) { mv_Offset = offset; }
    Vector2f GetSize() const { return mv_Size; }
};