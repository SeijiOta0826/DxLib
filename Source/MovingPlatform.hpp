#pragma once
#include "Tile.hpp"
#include "Vector.hpp"
#include <vector>
class MovingPlatform : public Tile
{
public:
    MovingPlatform(float px, float py, std::vector<Vector2f> points,std::string filename);
    //~MovingPlatform();
    void Update(double deltaTime) override;
    void Draw(const Camera& camera) override;

private:
    float x, y;        // 現在位置
    float startX;      // 初期位置
    float endX;        // 消える位置
    float speed;       // 移動速度
    bool isActive;     // 表示するか
    int respawnTimer;  // 再出現までの時間

    std::vector<Vector2f> m_points; // 目標リスト
    int currentIndex;           // 今の目標
};

MovingPlatform::MovingPlatform(float px, float py, std::vector<Vector2f> points, std::string filename)
    : Tile(px, py, filename)
    , m_points(points)
    , currentIndex(0)
    , speed(2.0f)

{
    SetTag("MoveingPlatfrom");

    isActive = true;
    respawnTimer = 0;

    // 初期位置
    x = px;
    y = py;

    for (auto point = m_points.begin();
        point < m_points.end();
        point++) {
        (*point).x += px;
        (*point).y += py;
    }
}

void MovingPlatform::Update(double deltaTime) 
{
    if (!isActive)
    {
        respawnTimer--;

        if (respawnTimer <= 0)
        {
            // 最初から再出現
            currentIndex = 0;
            x = m_points[0].x;
            y = m_points[0].y;
            isActive = true;

            // 重要：ここで基底クラスの座標も同期させる
            SetPosition(Vector2f(x, y));
        }
        return;
    }

    Vector2f target = m_points[currentIndex];
    Vector2f pos = Vector2f(x, y);
    Vector2f dir = target - pos;

    float dist = dir.magnitude();  

    if (dist < 5.0f)
    {
        currentIndex++;

        // 最後のポイントに到達
        if (currentIndex >= m_points.size())
        {
            isActive = false;
            respawnTimer = 60; // 1秒後に復活
        }
        return;
    }

    dir = dir.normalize();
    x += dir.x * speed;
    y += dir.y * speed;

    SetPosition(Vector2f(x, y));
    Tile::Update(deltaTime);
}

void MovingPlatform::Draw(const Camera& camera)
{
    if (!isActive) return;
    Tile::Draw(camera);
}