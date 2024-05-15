#include "pch.h"
#include "EnemyCartwheel.h"
#include "TextureManager.h"
#include "LevelManager.h"

const float EnemyCartwheel::MAXHEALTH{ 100.0f };
const float EnemyCartwheel::GRAVITY{ 1000.0f };
const float EnemyCartwheel::SPEED{ 25.0f };

EnemyCartwheel::EnemyCartwheel(LevelManager* levelManager, TextureManager* textureManager, float x, float y) :
	Enemy(levelManager, textureManager, x, y)
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
	m_TextureManager->Animate(animationPath, Point2f{ m_HitBox.left, m_HitBox.bottom }, m_AnimationDuration, m_LeftFacing, loop, frameTimeModifier);
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
		break;
	case Enemy::State::walk:
		break;
	case Enemy::State::attack:
		break;
	case Enemy::State::death:
		break;
	case Enemy::State::parried:
		break;
	default:
		break;
	}

	m_HitBox.left += m_Velocity.x * elapsedSec;
	m_HitBox.bottom += m_Velocity.y * elapsedSec;
	m_LevelManager->CollisionCheck(m_HitBox, m_Velocity);
}
