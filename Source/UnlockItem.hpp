#pragma once
#include "Tile.hpp"
#include "InputMapping.hpp"
#include "GameObject.hpp"
#include "Player.hpp"

#include "InputDisplayPanel.hpp"
#include "InputMapping.hpp"

class UnlockItem : public Tile {
private:
    InputAction m_TargetAction; // このアイテムがどのアクションを解放するか

public:
    // コンストラクタで「何を解放するアイテムか」を指定して生成する
    UnlockItem(float x, float y, InputAction action,std::string filename)
        : Tile(x,y, filename)      // 親クラス (GameObject) の座標初期化を呼ぶ
        , m_TargetAction(action) {
		SetTag("UnlockItem"); // タグを設定（必要に応じて）
        GameObject::SetPosition(Vector2f(x,y));
    }

    ~UnlockItem() {
        auto panel = GetScene()->FindObjectWithTag("DisplayPanel");
        InputDisplayPanel* Panel = dynamic_cast<InputDisplayPanel*>(panel);
        
        // Panelがnullptrなら何もしない（クラッシュを回避）
        if (Panel == nullptr) return;
        switch (m_TargetAction) {
            case InputAction::MOVE_LEFT:
                Panel->AddAcquiredItem("Resource/images/左.png");
				break;
            case InputAction::JUMP:
                Panel->AddAcquiredItem("Resource/images/ジャンプ.png");
                break;
            case InputAction::ATTACK:
                Panel->AddAcquiredItem("Resource/images/攻撃.png");
                break;
            case InputAction::PICKUP:
                Panel->AddAcquiredItem("Resource/images/掴む.png");
                break;
        }
    }

    InputAction GetInputAction() { return m_TargetAction; }
};