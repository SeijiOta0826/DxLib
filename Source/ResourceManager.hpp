#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <mutex>
#include <memory>

// リソース全体を管理するクラス
// リソースを読み込む・破棄するときはこのクラスを使う
// 使い方例：m_ImageHandle = ResourceManager::GetInstance().Load("Resource/images/Enemy.png")
//           ResourceManager::GetInstance().ReleaseGraphic("Resource/images/test_player.png");

enum class ResourceType {
    GRAPHIC,
    AUDIO_SOUND,
    AUDIO_MUSIC,
};

struct BaseResourceData
{
    ResourceType type;
    int refCount;     
    bool isLoaded;    

    BaseResourceData(ResourceType t) : type(t), refCount(0), isLoaded(false) {}
};

struct GraphicResourceData : public BaseResourceData {
    int graphicHandle;

    GraphicResourceData() : BaseResourceData(ResourceType::GRAPHIC), graphicHandle(-1) {}
};

struct AudioResourceData : public BaseResourceData {
    int audioHandle;
    bool isMusic;  // true = music, false = se

    AudioResourceData(bool music) : BaseResourceData(music ? ResourceType::AUDIO_MUSIC : ResourceType::AUDIO_SOUND),
        audioHandle(-1), isMusic(music) {
    }
};

struct ResourceKey {
    std::string path;
    ResourceType type;

    ResourceKey(const std::string& p, ResourceType t) : path(p), type(t) {}

    bool operator==(const ResourceKey& other) const {
        return path == other.path && type == other.type;
    }
};

struct ResourceKeyHash {
    size_t operator()(const ResourceKey& key) const {
        return std::hash<std::string>()(key.path) ^
            std::hash<int>()(static_cast<int>(key.type));
    }
};

// singleton pattern
class ResourceManager
{
private:
    ResourceManager() {}
    ~ResourceManager();

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    std::unordered_map<ResourceKey, std::unique_ptr<BaseResourceData>, ResourceKeyHash> m_Cache;
    std::mutex m_Mutex;

public:
    static ResourceManager& GetInstance()
    {
        static ResourceManager instance;
        return instance;
    }

    void Update();

    int LoadGraphic(const std::string& path);
    void ReleaseGraphic(const std::string& path);

    int LoadAudioSound(const std::string& path);
    int LoadAudioMusic(const std::string& path);
    void ReleaseAudioSound(const std::string& path);
    void ReleaseAudioMusic(const std::string& path);


    int GetGraphicHandle(const std::string& path);
    int GetAudioSoundHandle(const std::string& path);
    int GetAudioMusicHandle(const std::string& path);

private:
    int InternalLoad(const std::string& path, ResourceType type);
    void InternalRelease(const std::string& path, ResourceType type);
    int InternalGetHandle(const std::string& path, ResourceType type);
};


inline int LoadTexture(const std::string& path) {
    return ResourceManager::GetInstance().LoadGraphic(path);
}

inline int LoadSound(const std::string& path) {
    return ResourceManager::GetInstance().LoadAudioSound(path);
}

inline int LoadMusic(const std::string& path) {
    return ResourceManager::GetInstance().LoadAudioMusic(path);
}

