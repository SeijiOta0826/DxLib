#pragma once
#include "GameObject.hpp"

#include <vector>
#include <memory>
#include <algorithm>

// 複数のオブジェクト実体が必要の時このクラスを利用できる
// 例えば複数のエネミーを作るとき
template <typename T>
class ObjectPool
{
private:
    std::vector<std::unique_ptr<T>> m_Pool;
    T* m_Prototype;

public:
    ObjectPool(int initialSize = 10)
    {
        for (int i = 0; i < initialSize; ++i)
        {
            T* obj = new T();
            obj->SetActive(false);
            m_Pool.push_back(std::unique_ptr<T>(obj));
        }
    }

    ~ObjectPool() = default;

    T* Get()
    {
        for (auto& obj : m_Pool)
        {
            if (!obj->IsActive())
            {
                obj->SetActive(true);
                return obj.get();
            }
        }

        T* newObj = new T();
        newObj->SetActive(true);
        m_Pool.push_back(std::unique_ptr<T>(newObj));
        return newObj;
    }

    T* FindWithTag(const std::string& tag) {
        for (auto& obj : m_Pool) {
            if (obj->IsActive() && obj->GetTag() == tag) {
                return obj.get();
            }
        }

        return nullptr;
    }

    std::vector<T*> FindAllWithTag(const std::string& tag) {
        std::vector<T*> results;
        for (auto& obj : m_Pool) {
            if (obj->IsActive() && obj->GetTag() == tag) {
                results.push_back(obj.get());
            }
        }
        return results;
    }

    void Return(T* obj, bool resetTag = true)
    {
        obj->SetActive(false);
        if (resetTag) { obj->SetTag("Untagged"); }
    }

    void UpdateAll(double deltaTime)
    {
        for (auto& obj : m_Pool)
        {
            if (obj->IsActive())
            {
                obj->Update(deltaTime);
            }
        }
    }

    void DrawAll()
    {
        for (auto& obj : m_Pool)
        {
            if (obj->IsActive())
            {
                obj->Draw();
            }
        }
    }

    size_t GetActiveCount() const
    {
        return std::count_if(m_Pool.begin(), m_Pool.end(),
            [](const std::unique_ptr<T>& obj) { return obj->IsActive(); });
    }
};
