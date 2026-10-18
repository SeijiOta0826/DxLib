#pragma once

#include "DxLib.h"

#include <unordered_map>
#include <string>
#include <memory>

// 音声ファイル管理のクラス
// 音声ファイルを読み込むとき、再生などをしたいときはこのクラスを呼び出す
// 使い方例：AudioManager::GetInstance()->PlaySound("Resource/sounds/se_test.mp3");
// ↑SEを再生する

// singleton pattern
class AudioManager
{
private:
    static AudioManager* s_Instance;

    std::unordered_map<std::string, int> m_PlayingSounds;
    std::unordered_map<std::string, int> m_PlayingMusics;

    int m_SoundVolume = 255;  // (0-255)
    int m_MusicVolume = 255;  // (0-255)

	int m_bgmHandle = -1; // 現在再生中のBGMのハンドル

public:
    AudioManager();
    ~AudioManager();

    static AudioManager* GetInstance()
    {
        if (!s_Instance)
        {
            s_Instance = new AudioManager();
        }
        return s_Instance;
    }

    static void Destroy()
    {
        if (s_Instance)
        {
            delete s_Instance;
            s_Instance = nullptr;
        }
    }

    bool PlaySound(const std::string& path); // SE再生
    void StopSound(const std::string& path); // SE再生停止
    void StopAllSounds();                    // 全てのSEを停止
    void SetSoundVolume(int volume);         // 音量調整(0, 255)
    int GetSoundVolume() const {             // 現在の音量を獲得(0, 255)
        return m_SoundVolume; 
    }

    // BGMも大体同じ
    bool PlayMusic(const std::string& path, bool loop = true);
    void StopMusic();
    void PauseMusic();
    void ResumeMusic();  // TODO
    bool IsMusicPlaying() const;
    void SetMusicVolume(int volume);  // (0, 255)
    int GetMusicVolume() const { return m_MusicVolume; }

    void Update();

private:
    void ApplyVolumeToChannel(int handle, int volume);
};