#include "pch.h"
#include "EnemyCartwheel.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Player.h"
#include "LevelManager.h"

const float EnemyCartwheel::MAXHEALTH{ 60.0f };
const float EnemyCartwheel::SPEED{ 25.0f };
const float EnemyCartwheel::ATTACKDMG{ 10.0f };
const int EnemyCartwheel::POINTS{ 20 };

EnemyCartwheel::EnemyCartwheel(LevelManager* levelManager, TextureManager* textureManager, SoundManager* soundManager, Player* player, float x, float y) :
	Enemy(levelManager, textureManager, soundManager, player, x, y)
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
		if (m_AudioChannel == -1 || !m_SoundManager->IsPlaying("cartwheel_walk", m_AudioChannel)) m_AudioChannel = m_SoundManager->Play("cartwheel_walk");
		if (m_LeftFacing) m_Velocity.x = -SPEED;
		else m_Velocity.x = SPEED;

		if (PlayerDistance() >= detectionRange) Idle();
		if (PlayerDistance() <= attackRange && m_AttackCooldown < 0.0f) Attack();
		else if (PlayerDistance() <= attackRange) Idle();
		if (m_Health < 0.0001f) Death();

		break;
	case Enemy::State::attack:
		if (m_AnimationDuration > m_TextureManagerPtr->GetAnimationDuration("cartwheel_attack") / 0.4f) Idle();
		if (m_Health < 0.0001f) Death();

		if (m_AttackCooldown < 0.0f) PlayerHit();
		break;
	case Enemy::State::death:
		break;
	case Enemy::State::parried:
		if (m_AnimationDuration > m_TextureManagerPtr->GetAnimationDuration("cartwheel_parried") / 0.4f) Idle();
		break;
	default:
		break;
	}

	m_HitBox.left += m_Velocity.x * elapsedSec;
	m_HitBox.bottom += m_Velocity.y * elapsedSec;
	m_LevelManagerPtr->CollisionCheck(m_HitBox, m_Velocity);
}

void EnemyCartwheel::CheckPlayerInteract()
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

void EnemyCartwheel::PlayerHit()
{
	const float hurtboxWidth{ 83.0f };
	const float hurtboxHeight{ 76.0f };
	const float attackCooldown{ 3.0f };
	Rectf hurtBox{ m_HitBox.left, m_HitBox.bottom, hurtboxWidth, hurtboxHeight };
	if (m_LeftFacing)
		hurtBox.left = m_HitBox.left - hurtboxWidth + m_HitBox.width;
	if (m_PlayerPtr->Attack(hurtBox, ATTACKDMG, m_LeftFacing)) Parried();
	m_AttackCooldown = 1.0f;
}

void EnemyCartwheel::Attack()
{
	const float attackFrame{ 1.0f };
	Enemy::Attack();
	m_AttackCooldown = attackFrame;
	m_SoundManager->Play("cartwheel_attack");
}

void EnemyCartwheel::Death()
{
	Enemy::Death();
	m_SoundManager->Play("cartwheel_death");
	m_PlayerPtr->SetTears(m_PlayerPtr->GetTears() + POINTS);
}
