#pragma once
#include "DxLib.h"

// 画像描画関連のクラス
// 使い方例：
// GraphicsManager::GetInstance().GetGraphSize(playerHandle, &mn_ImageSize_x, &mn_ImageSize_y);

// singleton pattern
class GraphicsManager
{
private:
    GraphicsManager() {}   // ctor
    ~GraphicsManager() {}  // dtor

    int mn_ScreenWidth;
    int mn_ScreenHeight;

public:
    static GraphicsManager& GetInstance(){
        static GraphicsManager instance;
        return instance;
    }

    void Initialize(int width, int height)
    {
        mn_ScreenWidth = width;
        mn_ScreenHeight = height;
    }

    void DrawGraph(int x, int y, int graphHandle, int transFlag)
    {
        ::DrawGraph(x, y, graphHandle, transFlag);
    }

    void DrawExtendGraph(int x, int y, int width, int height, int graphHandle, int transFlag)
    {
        ::DrawExtendGraph(x, y, x + width, y + height, graphHandle, transFlag);
    }

    void DrawString(int x, int y, const char* text, int color)
    {
        ::DrawString(x, y, text, color);
    }

    void DrawLine(int x1, int y1, int x2, int y2, int color) {
        ::DrawLine(x1, y1, x2, y2, color);
    }

    void DrawCircle(int x, int y, int r, int color, int fillFlag) {
        ::DrawCircle(x, y, r, color, fillFlag);
    }

    void GetGraphSize(int imageHandle, int* size_x, int* size_y) {
        ::GetGraphSize(imageHandle, size_x, size_y);
    }

    void End() {
        ::DxLib_End();
    }
};