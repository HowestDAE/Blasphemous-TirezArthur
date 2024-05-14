#include "pch.h"
#include "Enemy.h"
#include "LevelManager.h"
#include "TextureManager.h"
#include "utils.h"

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

bool Enemy::Hit(Rectf hitbox, float damage)
{
	if (utils::IsOverlapping(hitbox, m_HitBox)) {
		m_Health = std::max(0.0f, m_Health - damage);
		return true;
	}
	return false;
}

bool Enemy::IsDead()
{
	return (m_State == State::death && m_AnimationDuration > 10.0f);
}

void Enemy::Death()
{
	m_State = State::death;
	m_Velocity.x = 0.0f;
	m_AnimationDuration = 0.0f;
}

void Enemy::Idle()
{
	m_State = State::idle;
	m_Velocity.x = 0.0f;
	m_AnimationDuration = 0.0f;
}

void Enemy::Walk()
{
	m_State = State::walk;
	m_AnimationDuration = 0.0f;
}

void Enemy::Parried()
{
	m_State = State::parried;
	m_Velocity.x = 0.0f;
	m_AnimationDuration = 0.0f;
}

void Enemy::Attack()
{
	m_State = State::attack;
	m_AnimationDuration = 0.0f;
}
