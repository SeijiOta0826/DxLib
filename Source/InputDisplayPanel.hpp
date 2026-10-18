#pragma once
# include "GameObject.hpp" 
# include "ResourceManager.hpp"
# include "SpriteComponent.hpp"
# include "ColliderComponent.hpp"
# include "PhysicsComponent.hpp"
#include "InputMapping.hpp"

# include "Vector.hpp"
class InputDisplayPanel : public GameObject
{
private:
    int mn_ImageSize_x;
    int mn_ImageSize_y;

    std::string ms_filename;

	std::vector<std::string> m_acquiredItems; // 取得したアイテムの情報を保持するリスト
public:
    InputDisplayPanel(float x = 0.f, float y = 0.f, std::string sfilename = "") :GameObject(), ms_filename(sfilename)
    {
        SetPosition(Vector2f(x, y));    //初期座標を適用
        SetTag("DisplayPanel");     //タグ設定

        int tileHandle = ResourceManager::GetInstance().LoadGraphic(ms_filename);
        auto trans = GetTransform();
        GraphicsManager::GetInstance().GetGraphSize(tileHandle, &mn_ImageSize_x, &mn_ImageSize_y);

        AddAcquiredItem("Resource/images/右.png");
    }

    virtual ~InputDisplayPanel() {
        ResourceManager::GetInstance().ReleaseGraphic(ms_filename);
        m_acquiredItems.clear();
    }

    static InputDisplayPanel& GetInstance()
    {
        static InputDisplayPanel instance;
        return instance;
    }


    // 追加：外部（Playerの衝突判定など）からアイテム取得時に呼ばれる関数
    void AddAcquiredItem(const std::string& itemFilename) {
        // 画像をメモリにロードし、リストの末尾（実体）に追加する
       int handle = ResourceManager::GetInstance().LoadGraphic(itemFilename);
        m_acquiredItems.push_back(itemFilename);
    }

    virtual void Update(double deltaTime) override
    {
        if (!mb_IsActive) return;
        GameObject::Update(deltaTime);
    }

    virtual void Draw(const Camera& camera) override
    {
        if (!mb_IsActive) return;

        // 黒枠の基準座標（左上）を計算して保持
        int panelX = static_cast<int>(GetPosition().x - 310.f);
        int panelY = static_cast<int>(GetPosition().y - 200.f);

        // 1. 黒枠パネルの描画（半透明）
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
        GraphicsManager::GetInstance().DrawGraph(panelX, panelY, ResourceManager::GetInstance().GetGraphicHandle(ms_filename), true);

        // アイテム画像は半透明にしないため、ブレンドモードを元に戻す
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        // 2. 取得したアイテムの描画（不透明）
        int offsetX = 30;     // 黒枠の左端からの描画開始位置（余白）
        int offsetY = 40;     // 黒枠の上端からの描画開始位置（余白）
        int iconSpacing_y = 80; // アイテム同士のX座標の間隔
		int space_y = 0;        // アイテムの描画位置をずらすためのスペース変数

        int iconSpacing_x = 150; // アイテム同士のX座標の間隔
        int space_x = 0;        // アイテムの描画位置をずらすためのスペース変数

        for (auto index : m_acquiredItems) {
            // iが増えるごとに右へずらして描画座標を決定
            int drawX = panelX + offsetX + space_x;
            int drawY = panelY + offsetY + space_y;
			GraphicsManager::GetInstance().DrawGraph(drawX, drawY, ResourceManager::GetInstance().GetGraphicHandle(index), true);

            space_y += iconSpacing_y;
            if (space_y > mn_ImageSize_y - 80) {
                space_y = 0;
                space_x += iconSpacing_x;
            }
        }
    }
};