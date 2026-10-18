#pragma once

# include "Component.hpp"
# include "TransformComponent.hpp"
# include "ColliderComponent.hpp"

# include <string>
#include <vector>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <type_traits>

class Scene;

// すべてのオブジェクトの基底クラス
class GameObject
{
protected:
    bool mb_IsActive;  // オブジェクトが有効かどうか
    bool mb_Collidable = true; // 当たり判定を有効にするかどうか
    float mf_ZOrder = 0.f;  // z-order : 描画の順番を表す
    std::string ms_tag = "Untagged";  // オブジェクトのタグ（例えばPlayerに"Player"タグをくっつく）

    bool mb_IsPendingDestroy = false; // このオブジェクトは削除する必要があるかどうか

    Scene* mp_ParentScene = nullptr;
    class ColliderComponent* mp_Collider = nullptr;
    TransformComponent* mp_transform = nullptr;

    std::vector<std::unique_ptr<Component>> m_Components;  // オブジェクトにくっつくコンポーネント
    std::unordered_map<std::type_index, Component*> m_ComponentMap;

public:
    GameObject() : mb_IsActive(true) {
        mp_transform = AddComponent<TransformComponent>();
    }
    virtual ~GameObject() {}

    virtual void Update(double deltaTime) {
        for (auto& comp : m_Components) {
            comp->Update(deltaTime);
        }
    }

    virtual void Draw(const Camera& camera) {
        if (!mb_IsActive) return;

        for (auto& component : m_Components) {
            component->Draw(camera);
        }
    }

    virtual void OnCollision(GameObject* other) {}
    virtual void OnCollisionEnter(GameObject* other) {}
    virtual void OnCollisionExit(GameObject* other) {}

    // Getter / Setter (削除)
    bool IsPendingDestroy() const { return mb_IsPendingDestroy; }
    void Destroy() { mb_IsPendingDestroy = true; }

    // Getter / Setter (コライダーコンポーネント)
    void RegisterCollider(class ColliderComponent* col) { mp_Collider = col; }
    class ColliderComponent* GetCollider() const { return mp_Collider; }

    // Getter / Setter (当たり判定の有効/無効)
    void SetCollidable(bool collidable) { mb_Collidable = collidable; }
    bool IsCollidable() const { return mb_Collidable && mb_IsActive; }

    // Getter / Setter (オブジェクト有効/無効)
    bool IsActive() const { return mb_IsActive; }
    void SetActive(bool active) { mb_IsActive = active; }

    // Getter / Setter（移動コンポーネント）
    TransformComponent* GetTransform() const { return GetComponent<TransformComponent>(); }
    Vector2f GetPosition() const {
        auto t = GetTransform();
        return t ? t->mv_position : Vector2f(0.f, 0.f);
    }
    void SetPosition(const Vector2f& pos) { GetTransform()->mv_position = pos; }

    // Getter / Setter (z-order)
    float GetZOrder() const { return mf_ZOrder; }
    void SetZOrder(const float& z) { mf_ZOrder = z; }

    // Getter / Setter（tag）
    void SetTag(const std::string& tag) { ms_tag = tag; }
    const std::string& GetTag() const { return ms_tag; }
    bool CompareTag(const std::string& tag) const { return ms_tag == tag; }

    // Getter / Setter（移動コンポーネント）
    Scene* GetScene() const { return mp_ParentScene; }
    void SetScene(Scene* scene) { mp_ParentScene = scene; }

    // オブジェクトに新しいコンポーネントを追加
    template<typename T, typename... Args>
    T* AddComponent(Args&&... args) {
        static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

        auto pNewComponent = std::make_unique<T>(std::forward<Args>(args)...);
        T* rawPtr = pNewComponent.get();

        rawPtr->SetOwner(this);

        if constexpr (std::is_same_v<T, ColliderComponent>) {
            mp_Collider = rawPtr;
        }

        m_Components.push_back(std::move(pNewComponent));
        m_ComponentMap[std::type_index(typeid(T))] = rawPtr;

        return rawPtr;
    }

    // オブジェクトにくっつくコンポーネントを取得
    template<typename T>
    T* GetComponent() const {
        static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

        auto it = m_ComponentMap.find(std::type_index(typeid(T)));
        if (it != m_ComponentMap.end()) {
            return static_cast<T*>(it->second);
        }
        return nullptr;
    }

    // オブジェクトにコンポーネントを追加されたかどうか確認
    template<typename T>
    bool HasComponent() const {
        return m_ComponentMap.count(std::type_index(typeid(T))) > 0;
    }
};
