#include "pch.h"
#include "EnemyManager.h"
#include "LevelManager.h"
#include "TextureManager.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyCartwheel.h"
#include "EnemyShieldMaiden.h"
#include "EnemyStoner.h"
#include "EnemyCrucified.h"

EnemyManager::EnemyManager(LevelManager* levelManager, TextureManager* textureManager) :
	m_LevelManagerPtr{ levelManager },
	m_TextureManagerPtr{ textureManager }
{
	
}

EnemyManager::~EnemyManager()
{
	for (Enemy* enemy : m_EnemyPtrVector)
	{
		delete enemy;
	}
}

void EnemyManager::SetTargetPlayer(Player* player)
{
	m_PlayerPtr = player;

	m_EnemyPtrVector.emplace_back(new EnemyCartwheel{ m_LevelManagerPtr, m_TextureManagerPtr, m_PlayerPtr, 350.0f, 80.0f });
	m_EnemyPtrVector.emplace_back(new EnemyShieldMaiden{ m_LevelManagerPtr, m_TextureManagerPtr, m_PlayerPtr, 550.0f, 80.0f });
	m_EnemyPtrVector.emplace_back(new EnemyCrucified{ m_LevelManagerPtr, m_TextureManagerPtr, m_PlayerPtr, 750.0f, 80.0f });
	m_EnemyPtrVector.emplace_back(new EnemyStoner{ m_LevelManagerPtr, m_TextureManagerPtr, m_PlayerPtr, 950.0f, 80.0f });
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
