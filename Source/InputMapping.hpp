#pragma once
# include "InputManager.hpp"
# include <unordered_map>
# include <vector>
# include <set>

// プレイヤーからの入力を設定するクラス
// 実際のゲームロジックを書く時キーコードを使うではなく、プレイヤーのアクションで指定する
// これで入力デバイスを変えてもここを修正しただけで済む

enum class InputAction {
    TEST,
    CONFIRM,
    BACK,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN,
    DASH,
	JUMP,
	ATTACK,
    PICKUP,
    // ...
    // TODO : add other input actions

    //---------------------------------------
    // 要注意：アクションを追加する時必ず COUNT を最後にする
    COUNT // counting all input actions
};

// singleton pattern
class InputMapping {
private:
    std::unordered_map<InputAction, std::set<int>> m_ActionKeyMaps;
    std::vector<bool> m_CurrentState;
    std::vector<bool> m_PreviousState;

    InputMapping() = default;

public:
    inline static InputMapping& GetInstance() {
        static InputMapping instance;
        static bool initialized = false;
        if (!initialized) {
            instance.InitializeInternal();
            initialized = true;
        }
        return instance;
    }

    void SetKeyMapping(InputAction action, int key);
    void AddKeyMapping(InputAction action, int key);

    void RemoveKeyMapping(InputAction action, int key);
    void ClearKeyMappings(InputAction action);

    int GetMappedKey(InputAction action);
    std::set<int> GetMappedKeys(InputAction action);

    void Update();
    bool IsPressed(InputAction action);
    bool IsJustPressed(InputAction action);
    bool IsJustReleased(InputAction action);

private:
    void InitializeInternal();
    size_t GetIndex(InputAction action);
};


inline void InputMapping::InitializeInternal() {
    size_t count = static_cast<size_t>(InputAction::COUNT);
    m_CurrentState.resize(count, false);
    m_PreviousState.resize(count, false);

    // default key mappings
    AddKeyMapping(InputAction::CONFIRM, KEY_INPUT_RETURN);      // 決定
    AddKeyMapping(InputAction::BACK, KEY_INPUT_BACK);           // 決定

    AddKeyMapping(InputAction::DASH, KEY_INPUT_LSHIFT);         // 加速（左シフト）
    AddKeyMapping(InputAction::DASH, KEY_INPUT_RSHIFT);         // 加速（右シフト）

    AddKeyMapping(InputAction::MOVE_UP, KEY_INPUT_UP);          // 移動（上）
    AddKeyMapping(InputAction::MOVE_UP, KEY_INPUT_W); 

    AddKeyMapping(InputAction::MOVE_DOWN, KEY_INPUT_DOWN);      // 移動（下）
    AddKeyMapping(InputAction::MOVE_DOWN, KEY_INPUT_S);

    AddKeyMapping(InputAction::MOVE_LEFT, KEY_INPUT_LEFT);      // 移動（左）
    AddKeyMapping(InputAction::MOVE_LEFT, KEY_INPUT_A);

    AddKeyMapping(InputAction::MOVE_RIGHT, KEY_INPUT_RIGHT);    // 移動（右）
    AddKeyMapping(InputAction::MOVE_RIGHT, KEY_INPUT_D);

    AddKeyMapping(InputAction::JUMP, KEY_INPUT_SPACE);          // ジャンプ

    AddKeyMapping(InputAction::ATTACK, KEY_INPUT_L);            // 攻撃

    AddKeyMapping(InputAction::PICKUP, KEY_INPUT_J);            // アイテムを拾う / 捨てる

    // TODO : add other key mappings
    // ...
}

inline size_t InputMapping::GetIndex(InputAction action) {
    size_t index = static_cast<size_t>(action);
    return index;
}

inline void InputMapping::SetKeyMapping(InputAction action, int key) {
    m_ActionKeyMaps[action].clear();
    m_ActionKeyMaps[action].insert(key);
}

inline void InputMapping::AddKeyMapping(InputAction action, int key) {
    m_ActionKeyMaps[action].insert(key);
}

inline void InputMapping::RemoveKeyMapping(InputAction action, int key) {
    auto it = m_ActionKeyMaps.find(action);
    if (it != m_ActionKeyMaps.end()) {
        it->second.erase(key);
    }
}

inline void InputMapping::ClearKeyMappings(InputAction action) {
    m_ActionKeyMaps[action].clear();
}

inline int InputMapping::GetMappedKey(InputAction action) {
    auto it = m_ActionKeyMaps.find(action);
    if (it != m_ActionKeyMaps.end() && !it->second.empty()) {
        return *(it->second.begin());
    }
    return 0;
}

inline std::set<int> InputMapping::GetMappedKeys(InputAction action) {
    auto it = m_ActionKeyMaps.find(action);
    if (it != m_ActionKeyMaps.end()) {
        return it->second;
    }
    return std::set<int>();
}

inline void InputMapping::Update() {
    m_PreviousState = m_CurrentState;

    for (auto& pair : m_ActionKeyMaps) {
        size_t index = static_cast<size_t>(pair.first);
        if (index < m_CurrentState.size()) {
            bool isPressed = false;
            for (int key : pair.second) {
                if (InputManager::GetInstance().IsKeyDown(key)) {
                    isPressed = true;
                    break;
                }
            }
            m_CurrentState[index] = isPressed;
        }
    }
}

inline bool InputMapping::IsPressed(InputAction action) {
    size_t index = static_cast<size_t>(action);
    if (index >= m_CurrentState.size()) {
        return false;
    }
    return m_CurrentState[index];
}

inline bool InputMapping::IsJustPressed(InputAction action) {
    size_t index = static_cast<size_t>(action);
    if (index >= m_CurrentState.size() || index >= m_PreviousState.size()) {
        return false;
    }
    return m_CurrentState[index] && !m_PreviousState[index];
}

inline bool InputMapping::IsJustReleased(InputAction action) {
    size_t index = static_cast<size_t>(action);
    if (index >= m_CurrentState.size() || index >= m_PreviousState.size()) {
        return false;
    }
    return !m_CurrentState[index] && m_PreviousState[index];
}