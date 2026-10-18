# include <vector>
# include <string>
# include <unordered_map>

// プレイヤーのステータスを格納する構造体
// シーン遷移の際にプレイヤーのステータスを遷移前のまま保持できる
struct PlayerProgress {
    // プレイヤーステータス
    int maxHp = 99;
    int currentHp = 99;
    int inputCount = 0;  // 20260508 update : input counting

    // プレイヤーの能力スイッチ
    bool canMoveLeft = false;   // 左に移動できるか
    bool canMoveUp = false;     // 上に移動できるか
    bool canMoveDown = false;   // 下に移動できるか
    bool canJump = false;       // ジャンプできるか
    bool hasDash = false;       // 加速できるか
    // ...                      // TODO : 他の能力スイッチを追加する
    

    // 既に手に入れたアイテムを格納する（重複獲得防止）
    std::unordered_map<std::string, bool> collectedItems;
};

// singleton pattern
class ProgressManager {
public:
    static ProgressManager& GetInstance() { 
        static ProgressManager instance; 
        return instance; 
    }

    // プレイヤーデータを取得したいとき：
    // ProgressaManager::GetInstance().data
    PlayerProgress data;
};