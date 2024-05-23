#include "pch.h"
#include "EnemyStoner.h"
#include "TextureManager.h"
#include "LevelManager.h"
#include "Player.h"
#include "Utils.h"

const float EnemyStoner::MAXHEALTH{ 30.0f };
const float EnemyStoner::ATTACKDMG{ 8.0f };
const float EnemyStoner::SPEED{ 500.0f };
const float EnemyStoner::POINTS{ 10.0f };

EnemyStoner::EnemyStoner(LevelManager* levelManager, TextureManager* textureManager, Player* player, float x, float y) : 
	Enemy(levelManager, textureManager, player, x, y)
{
	m_HitBox.height = 40.0f;
	m_HitBox.width = 75.0f;
	m_Health = MAXHEALTH;
	m_TextureManagerPtr->PreLoadTexture("rock_land");
	m_TextureManagerPtr->PreLoadTexture("stoner_attack");
}

void EnemyStoner::Draw()
{
	std::string animationPath{ "cartwheel_idle" };
	bool loop{ true };
	float frameTimeModifier{ 1.0f };
	switch (m_State)
	{
	case Enemy::State::idle:
		animationPath = "stoner_idle";
		if (m_LeftFacing) animationPath = "stoner_idle_flipped";
		frameTimeModifier = 0.4f;
		break;
	case Enemy::State::attack:
		animationPath = "stoner_attack";
		if (m_LeftFacing) animationPath = "stoner_attack_flipped";
		frameTimeModifier = 0.4f;
		loop = false;
		break;
	case Enemy::State::death:
		animationPath = "stoner_death";
		if (m_LeftFacing) animationPath = "stoner_death_flipped";
		loop = false;
		break;
	default:
		break;
	}

	m_TextureManagerPtr->Animate(animationPath, Point2f{ m_HitBox.left, m_HitBox.bottom }, m_AnimationDuration, false, loop, frameTimeModifier);
	if (m_RockActive) m_TextureManagerPtr->Animate("rock_air", Point2f{ m_RockHitbox.left, m_RockHitbox.bottom }, m_RockAnimationDuration);
	else if (m_RockAnimationDuration <= m_TextureManagerPtr->GetAnimationDuration("rock_land")) {
		m_TextureManagerPtr->Animate("rock_land", Point2f{ m_RockHitbox.left, m_RockHitbox.bottom }, m_RockAnimationDuration);
	}
}

void EnemyStoner::Update(float elapsedSec)
{
	m_AnimationDuration += elapsedSec;
	m_RockAnimationDuration += elapsedSec;
	if (!m_RockActive) m_AttackCooldown -= elapsedSec;
	m_Velocity.y -= GRAVITY * elapsedSec;
	m_Velocity.y = std::max(m_Velocity.y, -500.0f);
	switch (m_State)
	{
	case Enemy::State::idle:
		CheckPlayerInteract();
		if (m_Health < 0.0001f) Death();
		break;
	case Enemy::State::attack:
		if (!m_RockActive && m_AnimationDuration > 1.7f && m_AttackCooldown < 0.0f)
		{
			SpawnRock();
		}
		if (m_AnimationDuration > m_TextureManagerPtr->GetAnimationDuration("stoner_attack") / 0.4f) Idle();
		if (m_Health < 0.0001f) Death();

		break;
	case Enemy::State::death:
		break;
	default:
		break;
	}

	if (m_RockActive){
		m_RockHitbox.left += m_Velocity.x * elapsedSec;
		m_RockHitbox.bottom += m_Velocity.y * elapsedSec;
		if (utils::IsOverlapping(m_PlayerPtr->GetHitbox(), m_RockHitbox)) {
			RockHit();
			m_PlayerPtr->Attack(m_RockHitbox, ATTACKDMG, m_Velocity.x < 0.0f);
		}
	}
	if (m_LevelManagerPtr->CollisionCheck(m_RockHitbox, m_Velocity)) RockHit();
	m_HitBox.bottom -= GRAVITY * elapsedSec;
	m_LevelManagerPtr->CollisionCheck(m_HitBox);
}

void EnemyStoner::CheckPlayerInteract()
{
	float leftDistance{};
	float rightDistance{};
	PlayerDistance(leftDistance, rightDistance, 50.0f);
	const float distance{ std::min(abs(leftDistance), abs(rightDistance)) };
	if (distance < 300.0f) {
		if (!m_RockActive && m_AttackCooldown < 0.0f) Attack();
		else if (m_State != State::idle) Idle();
		if (leftDistance > rightDistance) m_LeftFacing = true;
		else m_LeftFacing = false;
	}
}

void EnemyStoner::SpawnRock()
{
	const float playerCenterX{ m_RockHitbox.left };
	const float playerCentery{ m_RockHitbox.bottom };
	if (!m_LeftFacing) {
		m_RockHitbox.left = m_HitBox.left + 29.0f;
		m_RockHitbox.bottom = m_HitBox.bottom + 54.0f;
		m_Velocity.x = SPEED;
	}
	else {
		m_RockHitbox.left = m_HitBox.left + 32.0f;
		m_RockHitbox.bottom = m_HitBox.bottom + 54.0f;
		m_Velocity.x = -SPEED;
	}
	Rectf playerHitbox{ m_PlayerPtr->GetHitbox() };
	const float timeX{ (-playerCenterX + (m_RockHitbox.left + m_RockHitbox.width * 0.5f)) / m_Velocity.x };
	const float velocityY{ ((m_RockHitbox.bottom + m_RockHitbox.height * 0.5f) - playerCentery - 0.5f * GRAVITY * timeX * timeX) / timeX };
	m_Velocity.y = velocityY;
	m_RockActive = true;
	m_RockAnimationDuration = 0.0f;
	m_AttackCooldown = (rand() % 40 + 10) * 0.1f;
}

void EnemyStoner::RockHit()
{
	m_RockActive = false;
	m_RockAnimationDuration = 0.0f;
}

void EnemyStoner::Idle()
{
	m_State = State::idle;
	m_AnimationDuration = 0.0f;
}

void EnemyStoner::Attack()
{
	Enemy::Attack();
	Rectf playerHitbox{ m_PlayerPtr->GetHitbox() };
	m_RockHitbox.left = playerHitbox.left + playerHitbox.width * 0.5f;
	m_RockHitbox.bottom = playerHitbox.bottom + playerHitbox.height * 0.5f;
}
