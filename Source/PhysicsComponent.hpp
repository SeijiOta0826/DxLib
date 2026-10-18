#pragma once
#include "Component.hpp"
#include "Vector.hpp"
#include "TransformComponent.hpp"

// 物理っぽい動きを実現するクラス
// 重力や摩擦力などを適用したいときはこのコンポーネントを入れる
// ctor に AddComponent<PhysicsComponent>(mass); を入れる


class PhysicsComponent : public Component {
private:
    float mf_Mass = 1.f;
    float mf_invMass = 1.f;

    Vector2f mv_Velocity{ 0.f, 0.f };
    Vector2f mv_Acceleration{ 0.f, 0.f };

    static constexpr float kGravityConstant = 37.f;//9.81f;
    float mf_Damping = 0.988f;

    bool mb_IsEnabled = true;

public:
    // ctor
    PhysicsComponent(float mass = 1.f) : mv_Velocity(Vector2f(0.f, 0.f)), mv_Acceleration(Vector2f(0.f, 0.f)), mf_Mass(mass) {
        if (mf_Mass <= 0.f) {
            mf_invMass = 0.f;
            mf_Mass = 0.f;
        }
        else {
            mf_invMass = 1.f / mf_Mass;
        }
    }

    void Initialize() override { }

    void SetMass(float mass) { mf_Mass = mass; }
    float GetMass() const { return mf_Mass; }
    float GetInvMass() const { return mf_invMass; }

    void SetVelocity(const Vector2f& v) { mv_Velocity = v; }
    const Vector2f& GetVelocity() const { return mv_Velocity; }

    void ApplyForce(const Vector2f& force) {
        // F = ma
        Vector2f acceleration = force * mf_invMass;
        mv_Acceleration += acceleration;
    }

    void ApplyImpulse(const Vector2f& impulse) {
        // J = dp = mdv -> dv = J/m
        Vector2f deltaV = impulse * mf_invMass;
        mv_Velocity += deltaV;
    }

    void Update(double deltaTime) override {
        // 物理コンポーネント適用されてない場合はスルーする
        if (!mb_IsEnabled) return;

        float dt = static_cast<float>(deltaTime);
        if (mf_invMass <= 0) { return; }

        // 重力を入れる
        mv_Acceleration.y += kGravityConstant;
        //ApplyForce(Vector2f(0.f, kGravityConstant * mf_Mass));
        
        // 速度更新
        mv_Velocity = std::pow(mf_Damping, dt * 60.f) * mv_Velocity;
        mv_Velocity += mv_Acceleration * dt; 

        // 移動コンポーネントに反映する
        auto trans = GetOwner()->GetTransform();
        if (trans) {
            trans->mv_position += mv_Velocity * dt;
        }

        // 加速度クリア
        mv_Acceleration = Vector2f(0.f, 0.f);
    }

    Vector2f GetPositionDelta(double deltaTime) const {
        return mv_Velocity * static_cast<float>(deltaTime);
    }

    Vector2f GetDisplacement(double deltaTime) const {
        float dt = static_cast<float>(deltaTime);
        return mv_Velocity * dt;
    }

    void Reset() {
        mv_Velocity = Vector2f(0.0f, 0.0f);
        mv_Acceleration = Vector2f(0.0f, 0.0f);
    }

    void SetEnabled(bool isEnable) { mb_IsEnabled = isEnable; }
};