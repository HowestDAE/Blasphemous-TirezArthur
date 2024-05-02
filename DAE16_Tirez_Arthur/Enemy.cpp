#include "pch.h"
#include "Enemy.h"
#include "LevelManager.h"
#include "TextureManager.h"

Enemy::Enemy(LevelManager* levelManager, TextureManager* textureManager, float x, float y) :
	m_HitBox{ x, y, -1.0f, -1.0f },
	m_Health{ -1.0f },
	m_LeftFacing{ false },
	m_AnimationDuration{ 0.0f },
	m_State{ State::idle },
	m_LevelManager{ levelManager },
	m_TextureManager{ textureManager },
	m_Velocity{0.f, 0.f}
{
}