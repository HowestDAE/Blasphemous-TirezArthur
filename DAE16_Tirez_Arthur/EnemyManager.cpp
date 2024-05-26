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

EnemyManager::EnemyManager(TextureManager* textureManager, SoundManager* soundManager) :
	m_TextureManagerPtr{ textureManager },
	m_SoundManager{ soundManager }
{
	
}

EnemyManager::~EnemyManager()
{
	Clear();
}

void EnemyManager::SetLevelManager(LevelManager* levelManager)
{
	m_LevelManagerPtr = levelManager;
}

void EnemyManager::Clear()
{
	for (Enemy* enemy : m_EnemyPtrVector)
	{
		delete enemy;
		m_EnemyPtrVector.clear();
	}
}

void EnemyManager::SetTargetPlayer(Player* player)
{
	m_PlayerPtr = player;
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

void EnemyManager::SpawnEnemy(EnemyType type, float x, float y)
{
	switch (type)
	{
	case EnemyManager::EnemyType::cartwheel:
		m_EnemyPtrVector.emplace_back(new EnemyCartwheel{ m_LevelManagerPtr, m_TextureManagerPtr, m_SoundManager, m_PlayerPtr, x, y });
		break;
	case EnemyManager::EnemyType::shieldmaiden:
		m_EnemyPtrVector.emplace_back(new EnemyShieldMaiden{ m_LevelManagerPtr, m_TextureManagerPtr, m_SoundManager, m_PlayerPtr, x, y });
		break;
	case EnemyManager::EnemyType::stoner:
		m_EnemyPtrVector.emplace_back(new EnemyStoner{ m_LevelManagerPtr, m_TextureManagerPtr, m_SoundManager, m_PlayerPtr, x, y });
		break;
	case EnemyManager::EnemyType::crucified:
		m_EnemyPtrVector.emplace_back(new EnemyCrucified{ m_LevelManagerPtr, m_TextureManagerPtr, m_SoundManager, m_PlayerPtr, x, y });
		break;
	}
}