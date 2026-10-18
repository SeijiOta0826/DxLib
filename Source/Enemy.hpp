//#pragma once
//# include "Character.hpp"
//# include "ResourceManager.hpp"
//
//class Enemy : public Character {
//public:
//	Enemy() : Character(0, 0), m_ImageHandle(-1) {
//		//m_ImageHandle = ResourceManager::GetInstance().Load("Resource/images/Enemy.png");
//	}
//	~Enemy() {}
//
//	void Update(double deltaTime) override;
//	void Draw();
//
//private:
//	int m_ImageHandle;
//};