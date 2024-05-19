#include "pch.h"
#include "Enemy.h"
#include "LevelManager.h"
#include "TextureManager.h"
#include "Player.h"
#include "utils.h"
#include <iostream>

const float Enemy::GRAVITY{ 1000.0f };

Enemy::Enemy(LevelManager* levelManager, TextureManager* textureManager, Player* player, float x, float y) :
	m_HitBox{ x, y, -1.0f, -1.0f },
	m_LevelManagerPtr{ levelManager },
	m_TextureManagerPtr{ textureManager },
	m_PlayerPtr{ player }
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

void Enemy::PlayerDistance(float& left, float& right)
{
	Rectf& playerRect{ m_PlayerPtr->GetHitbox() };
	if ((playerRect.bottom < m_HitBox.bottom + m_HitBox.height) && (playerRect.bottom + playerRect.height > m_HitBox.bottom) && !m_PlayerPtr->IsDead()) {
		left = m_HitBox.left - playerRect.left - playerRect.width;
		right = playerRect.left - m_HitBox.left - m_HitBox.width;
	}
	else {
		left = FLT_MAX;
		right = FLT_MAX;
	}
}

float Enemy::PlayerDistance()
{
	float leftDistance{};
	float rightDistance{};
	PlayerDistance(leftDistance, rightDistance);
	return std::min(abs(leftDistance), abs(rightDistance));
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
	m_Velocity.x = 0.0f;
}
