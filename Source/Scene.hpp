#pragma once

#include "GameObject.hpp"
#include "Camera.hpp"
#include "ColliderComponent.hpp"
#include "PhysicsComponent.hpp"

#include <vector>
#include <memory>
#include <algorithm>

// すべてのシーンの基底クラス
// オブジェクトマネージャーの役割も担っている

class Scene {
protected:
    std::unique_ptr<Camera> m_Camera;
    std::vector<std::unique_ptr<GameObject>> objects;
    
    // ステージの境界
    // ここはテスト用の設定をしたが、実際のステージを作成する時 SetWorldBounds() 関数を呼び出して再設定するのが必要
    Bounds2 m_WorldBounds = Bounds2(Vector2f(0, 0), Vector2f(2000.f, 2000.f));

public:
    virtual ~Scene() = default;

    virtual void Initialize() {}

    virtual void Update(double deltaTime) {
        objects.erase(
            std::remove_if(objects.begin(), objects.end(),
                [](const std::unique_ptr<GameObject>& obj) {
                    return !obj->IsActive();
                }),
            objects.end()
        );

        for (auto& obj : objects) {
            obj->Update(deltaTime);
        }

        ResolveCollisions();

        for (int j = 0; j < 4; ++j) {
            for (size_t i = 0; i < objects.size(); ++i) {
                for (size_t j = i + 1; j < objects.size(); ++j) {
                    auto* a = objects[i].get();
                    auto* b = objects[j].get();

                    if (!a->IsCollidable() || !b->IsCollidable()) continue;

                    auto* colA = a->GetCollider();
                    auto* colB = b->GetCollider();

                    if (colA && colB && colA->CheckCollision(colB)) {
                        a->OnCollision(b);
                        b->OnCollision(a);

                        ResolveTwoBodyCollision(a, b);
                    }
                }
            }

            ResolveWorldBounds();
        }

        // 削除すべきオブジェクトを削除する
        objects.erase(std::remove_if(objects.begin(), objects.end(),
            [](const std::unique_ptr<GameObject>& obj) {
                return obj->IsPendingDestroy();
            }), objects.end());
    }

    virtual void Draw() {
        std::sort(objects.begin(), objects.end(), [](const auto& a, const auto& b) {
            return a->GetZOrder() < b->GetZOrder();
            });

        for (auto& obj : objects) {
            obj->Draw(*m_Camera);
        }
    }
    virtual void Finalize() {}

    void SetWorldBounds(const Vector2f& min, const Vector2f& max) {
        m_WorldBounds = Bounds2(min, max);
    }

    void AddObject(std::unique_ptr<GameObject> obj) {
        obj->SetScene(this);
        objects.push_back(std::move(obj));
    }

    void RemoveAllObjects() {
        objects.clear();
    }

    const std::vector<std::unique_ptr<GameObject>>& GetObjects() const {
        return objects;
    }

    GameObject* FindObjectWithTag(const std::string& tag) {
        auto it = std::find_if(objects.begin(), objects.end(),
            [&tag](const std::unique_ptr<GameObject>& obj) {
                return obj->IsActive() && obj->GetTag() == tag;
            });

        return (it != objects.end()) ? it->get() : nullptr;
    }

    std::vector<GameObject*> FindObjectsWithTag(const std::string& tag) {
        std::vector<GameObject*> result;
        for (auto& obj : objects) {
            if (obj->IsActive() && obj->GetTag() == tag) {
                result.push_back(obj.get());
            }
        }
        return result;
    }

    void ResolveCollisions() {
        for (size_t i = 0; i < objects.size(); ++i) {
            auto* colA = objects[i]->GetComponent<ColliderComponent>();
            if (!colA) continue;

            for (size_t j = i + 1; j < objects.size(); ++j) {
                auto* colB = objects[j]->GetComponent<ColliderComponent>();
                if (!colB) continue;

                if (colA->CheckCollision(colB)) {
                    objects[i]->OnCollision(objects[j].get());
                    objects[j]->OnCollision(objects[i].get());
                }
            }
        }
    }

    void ResolveTwoBodyCollision(GameObject* a, GameObject* b) {
        auto colA = a->GetCollider();
        auto colB = b->GetCollider();

        Bounds2 b1 = colA->GetAbsoluteBounds();
        Bounds2 b2 = colB->GetAbsoluteBounds();

        float overlapLeft = b1.p_max.x - b2.p_min.x;
        float overlapRight = b2.p_max.x - b1.p_min.x;
        float overlapTop = b1.p_max.y - b2.p_min.y;
        float overlapBottom = b2.p_max.y - b1.p_min.y;

        float minOverlapX = (std::min)(overlapLeft, overlapRight);
        float minOverlapY = (std::min)(overlapTop, overlapBottom);

        float invMassA = a->GetComponent<PhysicsComponent>()->GetInvMass();
        float invMassB = b->GetComponent<PhysicsComponent>()->GetInvMass();
        float totalInvMass = invMassA + invMassB;

        if (totalInvMass == 0) return;

        float m1 = invMassA / totalInvMass;
        float m2 = invMassB / totalInvMass;

        if (minOverlapX < minOverlapY) {
            float sign = (overlapLeft < overlapRight) ? -1.0f : 1.0f;
            float move = minOverlapX * sign;

            a->GetTransform()->Translate(move * m1, 0);
            b->GetTransform()->Translate(-move * m2, 0);
        }
        else {
            float sign = (overlapTop < overlapBottom) ? -1.0f : 1.0f;
            float move = minOverlapY * sign;

            a->GetTransform()->Translate(0, move * m1);
            b->GetTransform()->Translate(0, -move * m2);
        }
    }

    // ステージの境界判定
    void ResolveWorldBounds() {
        for (auto& obj : objects) {
            if (!obj->IsActive() || !obj->IsCollidable()) continue;

            auto* col = obj->GetCollider();
            if (!col) continue;

            Bounds2 b = col->GetAbsoluteBounds();
            Vector2f correction(0, 0);

            // left
            if (b.p_min.x < m_WorldBounds.p_min.x) {
                correction.x = m_WorldBounds.p_min.x - b.p_min.x;
            }
            // right
            else if (b.p_max.x > m_WorldBounds.p_max.x) {
                correction.x = m_WorldBounds.p_max.x - b.p_max.x;
            }
            // top
            if (b.p_min.y < m_WorldBounds.p_min.y) {
                correction.y = m_WorldBounds.p_min.y - b.p_min.y;
            }
            // bottom
            else if (b.p_max.y > m_WorldBounds.p_max.y) {
                correction.y = m_WorldBounds.p_max.y - b.p_max.y;
            }

            if (correction.x != 0 || correction.y != 0) {
                obj->GetTransform()->Translate(correction.x, correction.y);
            }
        }
    }
};