#include "DxLib.h"

#include "ResourceManager.hpp"

ResourceManager::~ResourceManager()
{
    std::lock_guard<std::mutex> lock(m_Mutex);
    for (auto& pair : m_Cache)
    {
        BaseResourceData* data = pair.second.get();
        if (data->isLoaded)
        {
            switch (data->type)
            {
            case ResourceType::GRAPHIC:
            {
                GraphicResourceData* graphicData = static_cast<GraphicResourceData*>(data);
                if (graphicData->graphicHandle != -1)
                {
                    DeleteGraph(graphicData->graphicHandle);
                }
                break;
            }
            case ResourceType::AUDIO_SOUND:
            case ResourceType::AUDIO_MUSIC:
            {
                AudioResourceData* audioData = static_cast<AudioResourceData*>(data);
                if (audioData->audioHandle != -1)
                {
                    DeleteSoundMem(audioData->audioHandle);
                }
                break;
            }
            }
        }
    }
    m_Cache.clear();
}

void ResourceManager::Update()
{
    // ...
}

int ResourceManager::LoadGraphic(const std::string& path)
{
    return InternalLoad(path, ResourceType::GRAPHIC);
}

void ResourceManager::ReleaseGraphic(const std::string& path)
{
    InternalRelease(path, ResourceType::GRAPHIC);
}

int ResourceManager::LoadAudioSound(const std::string& path)
{
    return InternalLoad(path, ResourceType::AUDIO_SOUND);
}

int ResourceManager::LoadAudioMusic(const std::string& path)
{
    return InternalLoad(path, ResourceType::AUDIO_MUSIC);
}

void ResourceManager::ReleaseAudioSound(const std::string& path)
{
    InternalRelease(path, ResourceType::AUDIO_SOUND);
}

void ResourceManager::ReleaseAudioMusic(const std::string& path)
{
    InternalRelease(path, ResourceType::AUDIO_MUSIC);
}

int ResourceManager::GetGraphicHandle(const std::string& path)
{
    return InternalGetHandle(path, ResourceType::GRAPHIC);
}

int ResourceManager::GetAudioSoundHandle(const std::string& path)
{
    return InternalGetHandle(path, ResourceType::AUDIO_SOUND);
}

int ResourceManager::GetAudioMusicHandle(const std::string& path)
{
    return InternalGetHandle(path, ResourceType::AUDIO_MUSIC);
}

int ResourceManager::InternalLoad(const std::string& path, ResourceType type)
{
    if (path.empty()) { return -1; }

    std::lock_guard<std::mutex> lock(m_Mutex);

    ResourceKey key(path, type);
    auto it = m_Cache.find(key);
    if (it != m_Cache.end())
    {
        if (!it->second->isLoaded) { return -1; }
        it->second->refCount++;
        switch (type)
        {
        case ResourceType::GRAPHIC:
            return static_cast<GraphicResourceData*>(it->second.get())->graphicHandle;
        case ResourceType::AUDIO_SOUND:
        case ResourceType::AUDIO_MUSIC:
            return static_cast<AudioResourceData*>(it->second.get())->audioHandle;
        }
    }

    std::unique_ptr<BaseResourceData> newData;
    int handle = -1;

    switch (type)
    {
    case ResourceType::GRAPHIC:
    {
        newData = std::make_unique<GraphicResourceData>();
        handle = LoadGraph(path.c_str());
        if (handle != -1)
        {
            static_cast<GraphicResourceData*>(newData.get())->graphicHandle = handle;
        }
        break;
    }
    case ResourceType::AUDIO_SOUND:
    case ResourceType::AUDIO_MUSIC:
    {
        newData = std::make_unique<AudioResourceData>(type == ResourceType::AUDIO_MUSIC);
        handle = LoadSoundMem(path.c_str());
        if (handle != -1)
        {
            static_cast<AudioResourceData*>(newData.get())->audioHandle = handle;
        }
        break;
    }
    }

    if (handle == -1)
    {
        std::cerr << "[Error] Failed to load: " << path << " (Type: " << static_cast<int>(type) << ")" << std::endl;
        return -1;
    }
    else
    {
        newData->isLoaded = true;
        newData->refCount = 1;
    }

    m_Cache[key] = std::move(newData);
    return handle;
}

void ResourceManager::InternalRelease(const std::string& path, ResourceType type)
{
    std::lock_guard<std::mutex> lock(m_Mutex);

    ResourceKey key(path, type);
    auto it = m_Cache.find(key);
    if (it != m_Cache.end())
    {
        it->second->refCount--;

        if (it->second->refCount <= 0)
        {
            switch (type)
            {
            case ResourceType::GRAPHIC:
            {
                GraphicResourceData* graphicData = static_cast<GraphicResourceData*>(it->second.get());
                if (graphicData->graphicHandle != -1)
                {
                    DeleteGraph(graphicData->graphicHandle);
                }
                break;
            }
            case ResourceType::AUDIO_SOUND:
            case ResourceType::AUDIO_MUSIC:
            {
                AudioResourceData* audioData = static_cast<AudioResourceData*>(it->second.get());
                if (audioData->audioHandle != -1)
                {
                    DeleteSoundMem(audioData->audioHandle);
                }
                break;
            }
            }
            m_Cache.erase(it);
        }
    }
}

int ResourceManager::InternalGetHandle(const std::string& path, ResourceType type)
{
    std::lock_guard<std::mutex> lock(m_Mutex);
    ResourceKey key(path, type);
    auto it = m_Cache.find(key);
    if (it != m_Cache.end() && it->second->isLoaded)
    {
        switch (type)
        {
        case ResourceType::GRAPHIC:
            return static_cast<GraphicResourceData*>(it->second.get())->graphicHandle;
        case ResourceType::AUDIO_SOUND:
        case ResourceType::AUDIO_MUSIC:
            return static_cast<AudioResourceData*>(it->second.get())->audioHandle;
        }
    }
    return -1;
}