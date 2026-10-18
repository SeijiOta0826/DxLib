#pragma once

# include "Scene.hpp"
# include "GameObject.hpp"
# include "ProgressManager.hpp"
#include "ObjectPool.hpp"
#include "UnlockItem.hpp"

# include "Character.hpp"
# include "InputManager.hpp"
# include "InputMapping.hpp"
# include "GraphicsManager.hpp"
# include "ResourceManager.hpp"
# include "AudioManager.hpp"
# include "PhysicsComponent.hpp"
# include "SpriteComponent.hpp"
# include "ColliderComponent.hpp"
# include "StateComponent.hpp"
#include "Effect.hpp"
#include "Particle.hpp"

# include <algorithm>

#include <unordered_set>
#include <iostream>


class Player : public Character
{
private:
    // 画像サイズ
    int mn_ImageSize_x;
    int mn_ImageSize_y;

    // 移動
    float mf_Speed = 70.f;
    float mf_JumpForce = -60.f;

    // 攻撃範囲
    float mf_AttackRange = 70.0f;
    float mf_AttackHeight = 5.0f;
    Bounds2 attackCollider; // test

    // 加速
    float mf_dash_factor = 1.f;

    // 持っているオブジェクトに指すポインター
    GameObject* mp_HeldObject = nullptr;
    float mf_InteractRange = 60.0f;

    // プレイヤーの向き
    bool mb_FacingRight = true;

    // 解放されたアクションを保存するセット
    std::unordered_set<InputAction> unlockedActions;

    Effect effect;
    Particle particle;
    int blinkTimer = 60;
    bool isBlinking = false;
public:
    Player(float x, float y) : Character(x, y), mn_ImageSize_x(0), mn_ImageSize_y(0)
    {
        this->SetTag("Player");

        auto trans = GetTransform();
        auto& progress = ProgressManager::GetInstance().data; // データ（ステータス）読み込み

        int playerHandle = ResourceManager::GetInstance().LoadGraphic("Resource/images/Player.png");

        GraphicsManager::GetInstance().GetGraphSize(playerHandle, &mn_ImageSize_x, &mn_ImageSize_y);

        AddComponent<SpriteComponent>(playerHandle, trans);
        AddComponent<PhysicsComponent>(75.0f);
        AddComponent<ColliderComponent>(trans, Vector2f(mn_ImageSize_x, mn_ImageSize_y));

        UnlockAction(InputAction::MOVE_RIGHT);
        UnlockAction(InputAction::PICKUP);

        UnlockAction(InputAction::MOVE_UP);

        effect.Init();
    }

    ~Player() {
        ResourceManager::GetInstance().ReleaseGraphic("Resource/images/Player.png");
    }

    // アイテムから呼ばれる解放関数
    void UnlockAction(InputAction action) {
        unlockedActions.insert(action);
    }

    bool IsActionUnlocked(InputAction action) const {
        return unlockedActions.find(action) != unlockedActions.end();
    }

    void DamageEffectStart() {
        isBlinking = true;
        blinkTimer = 60;
    }

    void Update(double deltaTime) override {
        // 20260507追加 : ゲームオーバーシーンに移る
        if (ProgressManager::GetInstance().data.currentHp <= 0) {
            SceneManager::GetInstance().ChangeScene(SceneType::SCENETYPE_GAMEOVER);
            return;
        }

        Move(deltaTime);  // 移動
        FacingRight();    // 向きを判断

        // オブジェクトを拾う / 捨てる
        //if (InputMapping::GetInstance().IsJustPressed(InputAction::PICKUP)
        //    && IsActionUnlocked(InputAction::PICKUP)) {
        //    bool processed = false;

        //    if (!mp_HeldObject && !processed) {
        //        TryPickUp();
        //        ProgressManager::GetInstance().data.inputCount++; // 20260508
        //        if (mp_HeldObject) processed = true;
        //    }

        //    if (mp_HeldObject && !processed) {
        //        Throw();
        //        ProgressManager::GetInstance().data.inputCount++; // 20260508
        //    }
        //}

        if (mp_HeldObject) {
            // 持っているときオブジェクトをプレイヤーの上にのせる
            // TODO : 画像リソースのサイズを合わせて調整することが必要
            Vector2f holdOffset(0.0f, -40.0f);
            mp_HeldObject->SetPosition(this->GetPosition() + holdOffset);
        }

        // 攻撃
        if (InputMapping::GetInstance().IsJustPressed(InputAction::ATTACK)
            && IsActionUnlocked(InputAction::ATTACK)) {
            Attack();
            ProgressManager::GetInstance().data.inputCount++; // 20260508
            AudioManager::GetInstance()->PlaySound("Resource/sounds/SE/からぶった.mp3");
        }

        // アイテムとの衝突をチェックして能力を解放する
        auto unlockObj = GetScene()->FindObjectsWithTag("UnlockItem");
        for (auto index : unlockObj) {
            if (GetComponent<ColliderComponent>()->CheckCollision(index->GetComponent<ColliderComponent>())) {  //衝突しているかどうかをチェック
                UnlockItem* item = static_cast<UnlockItem*>(index);
                UnlockAction(item->GetInputAction());
                item->Destroy();
                particle.Play(GetTransform()->mv_position.x, GetTransform()->mv_position.y, Rondom);
                particle.Play(GetTransform()->mv_position.x, GetTransform()->mv_position.y, Rondom);
                particle.Play(GetTransform()->mv_position.x, GetTransform()->mv_position.y, Circle);
                AudioManager::GetInstance()->PlaySound("Resource/sounds/SE/取得.mp3");
            }
        }

        if (isBlinking)
        {
            blinkTimer--;

            if (blinkTimer <= 0)
            {
                isBlinking = false;
            }
        }
        effect.Update();
        particle.Update();

        GameObject::Update(deltaTime);  //PhysicsComponent::Update()
    }

    void Draw(const Camera& camera) override
    {
        if (!mb_IsActive) return;     

        DrawHPBar();

        // テスト用：インターアクション可能範囲
        /*auto pos = camera.WorldToScreen(Vector2f(GetTransform()->mv_position.x, GetTransform()->mv_position.y));
        GraphicsManager::GetInstance().DrawCircle(
            pos.x, pos.y, 
            mf_InteractRange,
            GetColor(255, 255, 0), 0);*/

        // テスト用：攻撃時のコライダーを描く
        attackCollider.DrawAABB(camera);
        attackCollider.p_min = 0.f;
        attackCollider.p_max = 0.f;
        attackCollider.DrawAABB(camera);

        //DrawFormatString(10.f,100.f, GetColor(0,0,0),"HP: %d",  ProgressManager::GetInstance().data.currentHp);

        if (blinkTimer)
        {
            if ((blinkTimer / 5) % 2 == 0)
            {
                GameObject::Draw(camera);
            }
        }
        else {
            GameObject::Draw(camera);
        }
        effect.Draw(camera);
        particle.Draw(camera);
    }

    // プレイヤーの向き（右か左か）
    void FacingRight() {
        if (this->GetComponent<PhysicsComponent>()->GetVelocity().x < 0) { mb_FacingRight = false; }
        else { mb_FacingRight = true; }
    }

    // プレイヤーの移動処理
    void Move(float deltaTime) {

        mf_dash_factor = 1.f;

        auto* physComp = GetComponent<PhysicsComponent>();

        if (InputMapping::GetInstance().IsPressed(InputAction::DASH)) {
            mf_dash_factor = 2.f;
        }

        // -------------------------------------------------------------------------------------
        // TODO : fix the translate logic
        if (!physComp) {
            std::cerr << "Warning: Player has no PhysicsComponent!" << std::endl;
            return;
        }

        // MOVE
        if (InputMapping::GetInstance().IsPressed(InputAction::MOVE_RIGHT)
            && IsActionUnlocked(InputAction::MOVE_RIGHT)) {
            //GetTransform()->Translate(1.f * mf_dash_factor, 0.f);
            //physComp->ApplyForce(Vector2f(mf_Speed, 0.0f));
            physComp->SetVelocity({ mf_Speed * mf_dash_factor, physComp->GetVelocity().y });
        }
        if (InputMapping::GetInstance().IsPressed(InputAction::MOVE_LEFT)
             && IsActionUnlocked(InputAction::MOVE_LEFT)) {
            //GetTransform()->Translate(-1.f * mf_dash_factor, 0.f);
            physComp->SetVelocity({ -mf_Speed * mf_dash_factor, physComp->GetVelocity().y });
        }
        //if (InputMapping::GetInstance().IsPressed(InputAction::MOVE_UP)) {
        //    //GetTransform()->Translate(0.f, -1.f * mf_dash_factor);
        //    physComp->SetVelocity({ physComp->GetVelocity().x, -mf_Speed * mf_dash_factor });
        //}
        /*
        if (InputMapping::GetInstance().IsPressed(InputAction::MOVE_DOWN)) {
            //GetTransform()->Translate(0.f, 1.f * mf_dash_factor);
            physComp->SetVelocity({ physComp->GetVelocity().x, mf_Speed * mf_dash_factor });
        }*/

        // JUMP
        if (InputMapping::GetInstance().IsJustPressed(InputAction::JUMP)
            && IsActionUnlocked(InputAction::JUMP)) {


            bool onGround = false; 
            {
                auto colliders = GetScene()->FindObjectsWithTag("Tile");
                for (auto collider : colliders) {
                    if (collider->GetComponent<ColliderComponent>()->CheckCollision(GetComponent<ColliderComponent>())) {
                        onGround = true;
                        break;
                    }
                }
            }

            {
                auto colliders = GetScene()->FindObjectsWithTag("MoveingPlatfrom");
                for (auto collider : colliders) {
                    if (collider->GetComponent<ColliderComponent>()->CheckCollision(GetComponent<ColliderComponent>())) {
                        onGround = true;
                        break;
                    }
                }
            }

            if (physComp->GetVelocity().y >= -0.1f && GetPosition().y >= 0.f
                && onGround) {
                //physComp->ApplyImpulse(Vector2f(0.f, mf_JumpForce));
                // AudioManager::GetInstance()->PlaySound("Resource/sounds/se_test.mp3");  // audio test
                ProgressManager::GetInstance().data.inputCount++; // 20260508
                physComp->SetVelocity({ physComp->GetVelocity().x, -mf_Speed * mf_dash_factor });
            }
        }

        // update player's position
        Vector2f displacement = physComp->GetDisplacement(deltaTime);
        GetTransform()->Translate(displacement.x, displacement.y);
    }

    // オブジェクトを拾う
    void TryPickUp() {
        // 拾うことが可能なオブジェクトを探す
        auto props = GetScene()->FindObjectsWithTag("Tile");

        for (auto* prop : props) {
            float dist = (prop->GetPosition() - this->GetPosition()).magnitude();

            if (dist < mf_InteractRange) {
                mp_HeldObject = prop;

                auto phys = mp_HeldObject->GetComponent<PhysicsComponent>();
                auto stateComp = mp_HeldObject->GetComponent<StateComponent>();
                if (phys && stateComp) {
                    phys->Reset();
                    phys->SetEnabled(false);
                    stateComp->SetState(ActorState::Held);
                }
                //mp_HeldObject->SetCollidable(false);
                break;
            }
        }
    }

    // オブジェクトを捨てる
    void Throw() {
        if (!mp_HeldObject) return;

        auto stateComp = mp_HeldObject->GetComponent<StateComponent>();

        // コライダーコンポーネントを有効にする
        auto phys = mp_HeldObject->GetComponent<PhysicsComponent>();
        if (phys && stateComp) {
            stateComp->SetState(ActorState::Thrown);
            phys->SetEnabled(true);

            // 投げ出す力と方向を設定する
            float throwForce = 60.0f + (std::abs)(this->GetComponent<PhysicsComponent>()->GetVelocity().x);
            Vector2f throwDir = mb_FacingRight ? Vector2f(1.0f, -1.0f) : Vector2f(-1.0f, -1.0f);
            throwDir = throwDir.normalize() * throwForce;

            phys->SetVelocity(throwDir);
        }

        mp_HeldObject->SetCollidable(true);

        // 少しずらしてからポインターを削除する
        mp_HeldObject->GetTransform()->Translate(mb_FacingRight ? 10.f : -10.f, 0);
        mp_HeldObject = nullptr;
    }

    void Attack() {
        Vector2f offset = mb_FacingRight ? Vector2f(mf_AttackRange, 0) : Vector2f(-mf_AttackRange, 0);
        Vector2f attackCenter = GetPosition() + offset;

        // create attack collider
        Bounds2 attackBounds;
        attackBounds.p_min = attackCenter - Vector2f(mf_AttackRange, mf_AttackHeight);
        attackBounds.p_max = attackCenter + Vector2f(mf_AttackRange, mf_AttackHeight);
        attackCollider = attackBounds; // test

        auto scene = GetScene();
        if (!scene) return;

        const auto& targets = scene->GetObjects();
        for (const auto& target : targets) {
            if (target.get() == this) continue;

            auto col = target->GetCollider();
            if (col && col->GetAbsoluteBounds().Overlaps(attackBounds)
                && target->GetTag() == "Tile" && target->GetTag() != "MovingPlatform") {
                OnHitTarget(target.get());

            }
        }
    }

    void OnHitTarget(GameObject* target) {
        if (!target) return;

        float x = target->GetTransform()->mv_position.x;
        float y = target->GetTransform()->mv_position.y;
        auto state = target->GetComponent<StateComponent>();
        if (state) {
            state->SetState(ActorState::PendingDestroy);
        }

        effect.Play(x, y, Effect_Attack);
        target->Destroy();
        AudioManager::GetInstance()->PlaySound("Resource/sounds/SE/ヒット.mp3");
    }

    void DrawHPBar()
    {
        auto& progress = ProgressManager::GetInstance().data;

        int barWidth = 500;
        int barHeight = 50;

        int x = 30;
        int y = 1000;

        // 背景（灰色）
        DrawBox(
            x,
            y,
            x + barWidth,
            y + barHeight,
            GetColor(80, 80, 80),
            TRUE
        );

        float HpRate =
            (float)progress.currentHp / (float)progress.maxHp;

        int currentWidth = (int)(barWidth * HpRate);

        DrawBox(x, y, x + currentWidth, y + barHeight, GetColor(0, 255, 0), TRUE);

        DrawBox(
            x,
            y,
            x + barWidth,
            y + barHeight,
            GetColor(255, 255, 255),
            FALSE
        );
    }
};