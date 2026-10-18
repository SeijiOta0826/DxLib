#pragma once
#include "DxLib.h"
#include <vector>
#include <string>

#define KEY_MAX 256

// singleton pattern
class InputManager
{
private:
    InputManager() {}   // ctor
    ~InputManager() {}  // dtor

    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

    bool mb_CurrentKeys[KEY_MAX]; 
    bool mb_PreviousKeys[KEY_MAX];

public:
    static InputManager& GetInstance()
    {
        static InputManager instance;
        return instance;
    }

    void Update()
    {
        for (int i = 0; i < KEY_MAX; i++)
        {
            mb_PreviousKeys[i] = mb_CurrentKeys[i];
        }

        for (int i = 0; i < KEY_MAX; i++)
        {
            mb_CurrentKeys[i] = (CheckHitKey(i) == 1);
        }
    }

    bool IsKeyDown(int keyCode) const
    {
        return mb_CurrentKeys[keyCode];
    }

    bool IsKeyTriggered(int keyCode) const
    {
        return mb_CurrentKeys[keyCode] && !mb_PreviousKeys[keyCode];
    }

    bool IsKeyReleased(int keyCode) const
    {
        return !mb_CurrentKeys[keyCode] && mb_PreviousKeys[keyCode];
    }
};


