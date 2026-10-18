#include "AudioManager.hpp"
#include "ResourceManager.hpp"

AudioManager* AudioManager::s_Instance = nullptr;

AudioManager::AudioManager()
{
    ChangeVolumeSoundMem(m_SoundVolume, -1);
}

AudioManager::~AudioManager()
{
    StopAllSounds();
    StopMusic();
}

bool AudioManager::PlaySound(const std::string& path)
{
    int handle = ResourceManager::GetInstance().GetAudioSoundHandle(path);

    if (handle == -1) {
        handle = ResourceManager::GetInstance().LoadAudioSound(path);
        if (handle == -1) {
            return false;
        }
    }

    auto it = m_PlayingSounds.find(path);
    if (it != m_PlayingSounds.end()) {
        StopSoundMem(it->second);
    }

    int result = PlaySoundMem(handle, DX_PLAYTYPE_BACK);
    if (result == -1) {
        return false;
    }

    m_PlayingSounds[path] = handle;
    ApplyVolumeToChannel(handle, m_SoundVolume);

    return true;
}

void AudioManager::StopSound(const std::string& path)
{
    auto it = m_PlayingSounds.find(path);
    if (it != m_PlayingSounds.end()) {
        StopSoundMem(it->second);
        m_PlayingSounds.erase(it);
    }
}

void AudioManager::StopAllSounds()
{
    for (const auto& pair : m_PlayingSounds) {
        StopSoundMem(pair.second);
    }
    m_PlayingSounds.clear();
}

void AudioManager::SetSoundVolume(int volume)
{
    m_SoundVolume = (std::max)(0, (std::min)(255, volume));

    for (const auto& pair : m_PlayingSounds) {
        ApplyVolumeToChannel(pair.second, m_SoundVolume);
    }
}

bool AudioManager::PlayMusic(const std::string& path, bool loop)
{
    int handle = ResourceManager::GetInstance().GetAudioMusicHandle(path);

    if (handle == -1) {
        handle = ResourceManager::GetInstance().LoadAudioMusic(path);
        if (handle == -1) {
            return false;
        }
    }

    // 古いBGMが鳴っていれば止める
    StopMusic();

    // ★重要：すでにロード済みかどうかにかかわらず、AudioManagerが管理するBGMハンドルを最新に更新する
    m_bgmHandle = handle;

    int playType = loop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_NORMAL;
    // 最新のハンドルを使って再生する
    int result = PlaySoundMem(m_bgmHandle, playType);
    if (result == -1) {
        return false;
    }

    ApplyVolumeToChannel(m_bgmHandle, m_MusicVolume);
    return true;
}

void AudioManager::StopMusic()
{
    if (m_bgmHandle != -1) {
        StopSoundMem(m_bgmHandle);
    }
}

void AudioManager::PauseMusic()
{
    if (m_bgmHandle != -1) {
        StopSoundMem(m_bgmHandle);
    }
}

void AudioManager::ResumeMusic()
{
    // TODO
}

bool AudioManager::IsMusicPlaying() const
{
    if (m_bgmHandle != -1) {
        return CheckSoundMem(m_bgmHandle) == 1;
    }
    return false;
}

void AudioManager::SetMusicVolume(int volume)
{
    m_MusicVolume = (std::max)(0, (std::min)(255, volume));
}

void AudioManager::Update()
{
    auto it = m_PlayingSounds.begin();
    while (it != m_PlayingSounds.end()) {
        if (CheckSoundMem(it->second) != 1) {
            it = m_PlayingSounds.erase(it);
        }
        else {
            ++it;
        }
    }
}

void AudioManager::ApplyVolumeToChannel(int handle, int volume)
{
    ChangeVolumeSoundMem(volume, handle);
}