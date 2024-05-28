#include "pch.h"
#include "EnemyCrucified.h"
#include "SoundManager.h"
#include "TextureManager.h"
#include "Player.h"
#include "LevelManager.h"

const float EnemyCrucified::MAXHEALTH{ 80.0f };
const float EnemyCrucified::SPEED{ 25.0f };
const float EnemyCrucified::ATTACKDMG{ 26.0f };
const int EnemyCrucified::POINTS{ 20 };

EnemyCrucified::EnemyCrucified(LevelManager* levelManager, TextureManager* textureManager, SoundManager* soundManager, Player* player, float x, float y) :
	Enemy(levelManager, textureManager, soundManager, player, x, y)
{
	m_HitBox.height = 50.0f;
	m_HitBox.width = 25.0f;
	m_Health = MAXHEALTH;
}

void EnemyCrucified::Draw()
{
	std::string animationPath{ "cartwheel_idle" };
	bool loop{ true };
	float frameTimeModifier{ 1.0f };
	switch (m_State)
	{
	case Enemy::State::idle:
		animationPath = "crucified_idle";
		frameTimeModifier = 0.4f;
		break;
	case Enemy::State::walk:
		animationPath = "crucified_walk";
		frameTimeModifier = 0.4f;
		break;
	case Enemy::State::attack:
		animationPath = "crucified_attack";
		frameTimeModifier = 0.4f;
		loop = false;
		break;
	case Enemy::State::death:
		animationPath = "crucified_death";
		loop = false;
		break;
	default:
		break;
	}
	m_TextureManagerPtr->Animate(animationPath, Point2f{ m_HitBox.left, m_HitBox.bottom }, m_AnimationDuration, m_LeftFacing, loop, frameTimeModifier);
}

void EnemyCrucified::Update(float elapsedSec)
{
	const float attackRange{ 50.0f };
	const float detectionRange{ 180.0f };
	m_AnimationDuration += elapsedSec;
	m_AttackCooldown -= elapsedSec;
	m_Velocity.y += -GRAVITY * elapsedSec;
	m_Velocity.y = std::max(m_Velocity.y, -500.0f);
	switch (m_State)
	{
	case Enemy::State::idle:
		CheckPlayerInteract();
		if (m_Health < 0.0001f) Death();
		break;
	case Enemy::State::walk:
		if (m_AudioChannel == -1 || !m_SoundManager->IsPlaying("crucified_walk", m_AudioChannel)) m_AudioChannel = m_SoundManager->Play("crucified_walk");
		if (m_LeftFacing) m_Velocity.x = -SPEED;
		else m_Velocity.x = SPEED;

		if (PlayerDistance() >= detectionRange) Idle();
		if (PlayerDistance() <= attackRange && m_AttackCooldown < 0.0f) Attack();
		else if (PlayerDistance() <= attackRange) Idle();
		if (m_Health < 0.0001f) Death();
		break;
	case Enemy::State::attack:
		if (m_AnimationDuration > m_TextureManagerPtr->GetAnimationDuration("crucified_attack") / 0.4f) Idle();
		if (m_Health < 0.0001f) Death();

		if (m_AttackCooldown < 0.0f) PlayerHit();
		break;
	case Enemy::State::death:
		break;
	default:
		break;
	}

	m_HitBox.left += m_Velocity.x * elapsedSec;
	m_HitBox.bottom += m_Velocity.y * elapsedSec;
	m_LevelManagerPtr->CollisionCheck(m_HitBox, m_Velocity);
}

void EnemyCrucified::CheckPlayerInteract()
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

void EnemyCrucified::PlayerHit()
{
	const float hurtboxWidth{ 166.0f };
	const float hurtboxHeight{ 117.0f };
	const float attackCooldown{ 3.0f };
	Rectf hurtBox{ m_HitBox.left, m_HitBox.bottom, hurtboxWidth, hurtboxHeight };
	if (m_LeftFacing)
		hurtBox.left = m_HitBox.left - hurtboxWidth + m_HitBox.width;
	m_PlayerPtr->Attack(hurtBox, ATTACKDMG, m_LeftFacing);
	m_AttackCooldown = attackCooldown;
}

void EnemyCrucified::Attack()
{
	const float attackFrame{ 2.0f };
	Enemy::Attack();
	m_SoundManager->Play("crucified_attack");
	m_AttackCooldown = attackFrame;
}

void EnemyCrucified::Death()
{
	Enemy::Death();
	m_SoundManager->Play("crucified_death");
	m_PlayerPtr->SetTears(m_PlayerPtr->GetTears() + POINTS);
}
