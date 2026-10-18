#pragma once
#include "Camera.hpp" 

class GameObject;

// すべてコンポーネントの基底クラス
// Initialize() Update() は各子クラスにもう一度書く必要がある（中身は空っぽでもいい）

class Component {
public:
    virtual ~Component() = default;

    virtual void Initialize() = 0;
    virtual void Update(double deltaTime) = 0;
    virtual void Draw(const Camera& camera) {}

    // Getter / Setter (コンポーネントはどのオブジェクトにくっついているか)
    void SetOwner(GameObject* owner) { mp_owner = owner; }
    GameObject* GetOwner() { return mp_owner; }

protected:
    GameObject* mp_owner;
};