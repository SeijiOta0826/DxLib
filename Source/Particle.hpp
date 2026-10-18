#pragma once
#include<DxLib.h>
#include<vector>


struct  ParticleData
{
    float x, y;
    float vx, vy;
    int timer;
};

enum ParticlrType
{
    Rondom,
    Circle,
};

class Particle
{
public:
    void Init();                        // 初期化
    void Play(float x, float y, ParticlrType t);     // エフェクト発動
    void Update();                    // 更新
    void Draw(const Camera& camera);

private:
    std::vector<ParticleData> particles;
    int x = 0;
    int y = 0;
    int timer = 0;

};
void Particle::Init()
{
    particles.clear();
}

void Particle::Play(float px, float py, ParticlrType t)
{
    int count = 20;


    for (int i = 0; i < count; i++)
    {
        ParticleData p;

        p.x = px;
        p.y = py;
        p.timer = 30;

        if (t == Rondom)
        {
            // 今までのやつ（ランダム）
            float angle = GetRand(360) * DX_PI_F / 180.0f;
            float speed = GetRand(10) / 2.0f + 1.0f;

            p.vx = cosf(angle) * speed;
            p.vy = sinf(angle) * speed;
        }
        else if (t == Circle)
        {
            // 円形
            float angle = (2.0f * DX_PI_F / count) * i;
            float speed = 3.0f;

            p.vx = cosf(angle) * speed;
            p.vy = sinf(angle) * speed;
        }

        particles.push_back(p);
    }

}

void Particle::Update()
{
    for (auto& p : particles)
    {
        p.x += p.vx;
        p.y += p.vy;

        p.vy += 0.2f; // 重力

        p.timer--;
    }

    // 消えた粒を削除
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
            [](ParticleData& p)
            {
                return p.timer <= 0;
            }),
        particles.end()
    );
}

void Particle::Draw(const Camera& camera)
{
    for (auto& p : particles)
    {
        int alpha = p.timer * 8;

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

        Vector2f screen =
            camera.WorldToScreen(Vector2f(p.x, p.y));

        DrawCircle(
            (int)screen.x,
            (int)screen.y,
            3,
            GetColor(255, 200, 50),
            TRUE
        );
    }

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}