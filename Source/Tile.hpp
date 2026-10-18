#pragma once
# include "GameObject.hpp" 
# include "ResourceManager.hpp"
# include "SpriteComponent.hpp"
# include "ColliderComponent.hpp"
# include "PhysicsComponent.hpp"


# include "Vector.hpp"
class Tile : public GameObject
{
private:
    int mn_ImageSize_x; 
    int mn_ImageSize_y;

    std::string ms_filename;
public:
    Tile(float x = 0.f, float y = 0.f,std::string sfilename = "") :GameObject(), ms_filename(sfilename)
    {
        SetPosition(Vector2f(x, y));    //初期座標を適用
        SetTag("Tile");     //タグ設定

        int tileHandle = ResourceManager::GetInstance().LoadGraphic(ms_filename);   
        auto trans = GetTransform();    

        GraphicsManager::GetInstance().GetGraphSize(tileHandle, &mn_ImageSize_x, &mn_ImageSize_y);

        AddComponent<SpriteComponent>(tileHandle, trans);  
        AddComponent<ColliderComponent>(trans, Vector2f(mn_ImageSize_x, mn_ImageSize_y));  
        AddComponent<PhysicsComponent>(0.f);
        GetComponent<PhysicsComponent>()->SetEnabled(true); //タイルは動かないので物理コンポーネントは無効にする
    }

    virtual ~Tile() {
        ResourceManager::GetInstance().ReleaseGraphic(ms_filename);
    }

    virtual void Update(double deltaTime) override
    {
        if (!mb_IsActive) return;
        GetComponent<PhysicsComponent>()->Reset();
        GameObject::Update(deltaTime);
    }

    virtual void Draw(const Camera& camera) override
    {
        if (!mb_IsActive) return;
        GameObject::Draw(camera);
    }
};