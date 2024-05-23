#include "pch.h"
#include "EnemyShieldMaiden.h"
#include "TextureManager.h"
#include "Player.h"
#include "LevelManager.h"
#include "utils.h"
#include <iostream>

const float EnemyShieldMaiden::MAXHEALTH{ 50.0f };
const float EnemyShieldMaiden::SPEED{ 25.0f };
const float EnemyShieldMaiden::ATTACKDMG{ 12.0f };
const float EnemyShieldMaiden::SHIELDDMG{ 5.0f };
const float EnemyShieldMaiden::POINTS{ 15.0f };

EnemyShieldMaiden::EnemyShieldMaiden(LevelManager* levelManager, TextureManager* textureManager, Player* player, float x, float y) :
	Enemy(levelManager, textureManager, player, x, y)
{
	m_HitBox.height = 50.0f;
	m_HitBox.width = 25.0f;
	m_Health = MAXHEALTH;
}

void EnemyShieldMaiden::Draw()
{
	std::string animationPath{ "shieldmaiden_idle" };
	bool loop{ true };
	float frameTimeModifier{ 1.0f };
	switch (m_State)
	{
	case Enemy::State::idle:
		animationPath = "shieldmaiden_idle";
		frameTimeModifier = 0.4f;
		break;
	case Enemy::State::walk:
		animationPath = "shieldmaiden_walk";
		frameTimeModifier = 0.4f;
		break;
	case Enemy::State::attack:
		animationPath = "shieldmaiden_attack";
		frameTimeModifier = 0.4f;
		loop = false;
		break;
	case Enemy::State::death:
		animationPath = "shieldmaiden_death";
		loop = false;
		break;
	case Enemy::State::parried:
		animationPath = "shieldmaiden_parried";
		frameTimeModifier = 0.4f;
		loop = false;
		break;
	default:
		break;
	}
	m_TextureManagerPtr->Animate(animationPath, Point2f{ m_HitBox.left, m_HitBox.bottom }, m_AnimationDuration, m_LeftFacing, loop, frameTimeModifier);
}

void EnemyShieldMaiden::Update(float elapsedSec)
{
	m_AnimationDuration += elapsedSec;
	m_AttackCooldown -= elapsedSec;
	m_Velocity.y += -GRAVITY * elapsedSec;
	m_Velocity.y = std::max(m_Velocity.y, -500.0f);
	switch (m_State)
	{
	case Enemy::State::idle:
		if (m_Health < 0.0001f) Death();
		CheckPlayerInteract();

		PlayerHitShield();
		break;
	case Enemy::State::walk:
		if (m_LeftFacing) m_Velocity.x = -SPEED;
		else m_Velocity.x = SPEED;

		if (m_Health < 0.0001f) Death();
		if (PlayerDistance() >= 180.0f) Idle();
		if (PlayerDistance() <= 50.0f && m_AttackCooldown < 0.0f) Attack();
		else if (PlayerDistance() <= 50.0f) Idle();

		PlayerHitShield();
		break;
	case Enemy::State::attack:
		if (m_Health < 0.0001f) Death();
		if (m_AnimationDuration > m_TextureManagerPtr->GetAnimationDuration("shieldmaiden_attack") / 0.4f) Idle();

		if (m_AttackCooldown < 0.0f) PlayerHit();
		break;
	case Enemy::State::death:
		break;
	case Enemy::State::parried:
		if (m_AnimationDuration > m_TextureManagerPtr->GetAnimationDuration("shieldmaiden_parried") / 0.4f) Idle();
		if (m_Health < 0.0001f && m_AnimationDuration > 0.8f) Death();

		break;
	default:
		break;
	}

	m_HitBox.left += m_Velocity.x * elapsedSec;
	m_HitBox.bottom += m_Velocity.y * elapsedSec;
	m_LevelManagerPtr->CollisionCheck(m_HitBox, m_Velocity);
}

bool EnemyShieldMaiden::Hit(Rectf hitbox, float damage)
{
	if (utils::IsOverlapping(hitbox, m_HitBox)) {
		Rectf shieldBox{ m_HitBox.left + m_HitBox.width, m_HitBox.bottom, 20.0f, m_HitBox.height };
		if (m_LeftFacing)
			shieldBox.left = m_HitBox.left - 20.0f;
		if ((m_State == State::idle || m_State == State::walk) && utils::IsOverlapping(hitbox, shieldBox)) return false;
		m_Health = std::max(0.0f, m_Health - damage);
		return true;
	}
	return false;
}

void EnemyShieldMaiden::CheckPlayerInteract()
{
	const float attackRange{ 50.0f };
	const float detectionRange{ 180.0f };

	float leftDistance{};
	float rightDistance{};
	PlayerDistance(leftDistance, rightDistance);
	const float distance{ std::min(abs(leftDistance), abs(rightDistance)) };
	if (distance < attackRange) {
		if (m_AttackCooldown < 0.0f) {
			Attack();
			if (leftDistance > rightDistance) m_LeftFacing = true;
			else m_LeftFacing = false;
		}
		else if (m_State != State::idle) Idle();
	}
	else if (distance < detectionRange) {
		Walk();
		if (leftDistance > rightDistance) m_LeftFacing = true;
		else m_LeftFacing = false;
	}
}

void EnemyShieldMaiden::PlayerHit()
{
	const float hurtboxWidth{ 63.0f };
	const float hurtboxHeight{ 72.0f };
	const float attackCooldown{ 3.0f };
	Rectf hurtBox{ m_HitBox.left + m_HitBox.width * 0.5f, m_HitBox.bottom, hurtboxWidth, hurtboxHeight };
	if (m_LeftFacing)
		hurtBox.left = m_HitBox.left - hurtboxWidth + m_HitBox.width * 0.5f;
	if (m_PlayerPtr->Attack(hurtBox, ATTACKDMG, m_LeftFacing)) Parried();
	m_AttackCooldown = 3.0f;
}

void EnemyShieldMaiden::PlayerHitShield()
{
	const float hurtboxWidth{ 20.0f };
	Rectf hurtBox{ m_HitBox.left + m_HitBox.width, m_HitBox.bottom, hurtboxWidth, m_HitBox.height };
	if (m_LeftFacing)
		hurtBox.left = m_HitBox.left - hurtboxWidth;
	m_PlayerPtr->Attack(hurtBox, SHIELDDMG, m_LeftFacing);
}

void EnemyShieldMaiden::Attack()
{
	Enemy::Attack();
	m_AttackCooldown = 0.6f;
}
