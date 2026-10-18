#pragma once
#include<DxLib.h>
#include<vector>
#include"Camera.hpp"

enum EffectType
{
    Effect_Attack,
    Effect_Max
};
class Effect
{

public:
    void Init();                        // 初期化
    void Play(float x, float y, EffectType t);     // エフェクト発動
    void Update();                    // 更新
    void Draw(const Camera& camera);                     // 描画

private:
    int x = 0;
    int y = 0;
    int timer = 0;
    bool isActive = false;
    int type = 0;
    int img[Effect_Max] = {};
};

inline void Effect::Init()
{
    img[Effect_Attack] = LoadGraph(_T("Resource/images/effeckt.png"));
    isActive = false;
}

inline void Effect::Play(float px, float py, EffectType t)
{
    x = px;
    y = py;
    timer = 30;
    isActive = true;
    type = t;
}

inline void Effect::Update()
{
    if (!isActive) return;

    timer--;

    if (timer <= 0)
    {
        isActive = false;
    }
}

inline void Effect::Draw(const Camera& camera)
{
    if (!isActive) return;

    Vector2f screen =
        camera.WorldToScreen(Vector2f(x, y));

    DrawRotaGraph(
        screen.x,
        screen.y,
        1.0,
        0.0,
        img[type],
        TRUE
    );
}