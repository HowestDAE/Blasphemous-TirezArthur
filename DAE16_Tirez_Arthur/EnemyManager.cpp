#include "pch.h"
#include "EnemyManager.h"
#include "LevelManager.h"
#include "TextureManager.h"
#include "Enemy.h"
#include "EnemyCartwheel.h"

EnemyManager::EnemyManager(LevelManager* levelManager, TextureManager* textureManager) :
	m_LevelManager{ levelManager },
	m_TextureManager{ textureManager }
{
	m_EnemyPtrVector.emplace_back(new EnemyCartwheel{ m_LevelManager, m_TextureManager, 150.0f, 100.0f });
}

EnemyManager::~EnemyManager()
{
	for (Enemy* enemy : m_EnemyPtrVector)
	{
		delete enemy;
	}
}

void EnemyManager::Draw()
{
	for (Enemy* enemy : m_EnemyPtrVector)
	{
		enemy->Draw();
	}
}

void EnemyManager::Update(float elapsedSec)
{
	for (Enemy* enemy : m_EnemyPtrVector)
	{
		enemy->Update(elapsedSec);
	}
	for (int index{}; index < m_EnemyPtrVector.size(); ++index)
	{
		if (m_EnemyPtrVector.at(index)->IsDead()) {
			delete m_EnemyPtrVector.at(index);
			m_EnemyPtrVector.erase(m_EnemyPtrVector.begin() + index);
		}
	}
}

bool EnemyManager::Attack(Rectf hitbox, float damage)
{
	bool enemyHit{};
	for (Enemy* enemy : m_EnemyPtrVector)
	{
		if (enemy->Hit(hitbox, damage))
			enemyHit = true;
	}
	return enemyHit;
}
