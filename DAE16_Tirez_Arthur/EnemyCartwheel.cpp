#include "pch.h"
#include "EnemyCartwheel.h"
#include "TextureManager.h"
#include "Player.h"
#include "LevelManager.h"
#include <iostream>

const float EnemyCartwheel::MAXHEALTH{ 100.0f };
const float EnemyCartwheel::GRAVITY{ 1000.0f };
const float EnemyCartwheel::SPEED{ 25.0f };

EnemyCartwheel::EnemyCartwheel(LevelManager* levelManager, TextureManager* textureManager, Player* player, float x, float y) :
	Enemy(levelManager, textureManager, player, x, y)
{
	m_HitBox.height = 50.0f;
	m_HitBox.width = 18.0f;
	m_Health = MAXHEALTH;
}

void EnemyCartwheel::Draw()
{
	std::string animationPath{ "cartwheel_idle" };
	bool loop{ true };
	float frameTimeModifier{ 1.0f };
	switch (m_State)
	{
	case Enemy::State::idle:
		animationPath = "cartwheel_idle";
		frameTimeModifier = 0.4f;
		break;
	case Enemy::State::walk:
		animationPath = "cartwheel_walk";
		frameTimeModifier = 0.4f;
		break;
	case Enemy::State::attack:
		animationPath = "cartwheel_attack";
		frameTimeModifier = 0.4f;
		loop = false;
		break;
	case Enemy::State::death:
		animationPath = "cartwheel_death";
		loop = false;
		break;
	case Enemy::State::parried:
		animationPath = "cartwheel_parried";
		frameTimeModifier = 0.4f;
		loop = false;
		break;
	default:
		break;
	}
	m_TextureManagerPtr->Animate(animationPath, Point2f{ m_HitBox.left, m_HitBox.bottom }, m_AnimationDuration, m_LeftFacing, loop, frameTimeModifier);
}

void EnemyCartwheel::Update(float elapsedSec)
{
	m_AnimationDuration += elapsedSec;
	m_Velocity.y += -GRAVITY * elapsedSec;
	m_Velocity.y = std::max(m_Velocity.y, -500.0f);
	switch (m_State)
	{
	case Enemy::State::idle:
		if (m_Health < 0.0001f) Death();
		if (CheckPlayerInteract()) break;
		break;
	case Enemy::State::walk:
		if (m_LeftFacing) m_Velocity.x = -SPEED;
		else m_Velocity.x = SPEED;

		if (m_Health < 0.0001f) Death();
		if (PlayerDistance() >= 180.0f) Idle();
		if (PlayerDistance() <= 50.0f) Attack();

		break;
	case Enemy::State::attack:
		if (m_Health < 0.0001f) Death();
		if (m_AnimationDuration > m_TextureManagerPtr->GetAnimationDuration("cartwheel_attack") / 0.4f) Idle();
		break;
	case Enemy::State::death:
		break;
	case Enemy::State::parried:
		if (m_Health < 0.0001f) Death();
		break;
	default:
		break;
	}

	m_HitBox.left += m_Velocity.x * elapsedSec;
	m_HitBox.bottom += m_Velocity.y * elapsedSec;
	m_LevelManagerPtr->CollisionCheck(m_HitBox, m_Velocity);
}

void EnemyCartwheel::PlayerDistance(float& left, float& right)
{
	Rectf& playerRect{ m_PlayerPtr->GetHitbox() };
	if ((playerRect.bottom < m_HitBox.bottom + m_HitBox.height) && (playerRect.bottom + playerRect.height > m_HitBox.bottom)) {
		left = m_HitBox.left - playerRect.left - playerRect.width;
		right = playerRect.left - m_HitBox.left - m_HitBox.width;
	}
	else {
		left = FLT_MAX;
		right = FLT_MAX;
	}
}

float EnemyCartwheel::PlayerDistance()
{
	float leftDistance{};
	float rightDistance{};
	PlayerDistance(leftDistance, rightDistance);
	return std::min(abs(leftDistance), abs(rightDistance));
}

bool EnemyCartwheel::CheckPlayerInteract()
{
	float leftDistance{};
	float rightDistance{};
	PlayerDistance(leftDistance, rightDistance);
	const float distance{ std::min(abs(leftDistance), abs(rightDistance)) };
	if (distance < 50.0f) {
		Attack();
		if (leftDistance > rightDistance) m_LeftFacing = true;
		else m_LeftFacing = false;
		return true;
	}
	else if (distance < 180.0f) {
		Walk();
		if (leftDistance > rightDistance) m_LeftFacing = true;
		else m_LeftFacing = false;
		return true;
	}
	return false;
}
