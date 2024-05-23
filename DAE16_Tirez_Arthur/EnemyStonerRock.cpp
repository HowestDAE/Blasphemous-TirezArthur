#include "pch.h"
#include "EnemyStonerRock.h"
#include "TextureManager.h"
#include "LevelManager.h"

const float EnemyStonerRock::ROCKDMG{ 8.0f };
const float EnemyStonerRock::GRAVITY{ 1000.0f };
const float EnemyStonerRock::RADIUS{ 5.0f };

EnemyStonerRock::EnemyStonerRock(LevelManager* levelManager, TextureManager* textureManager, Point2f position, Vector2f velocity) :
	m_Position{position},
	m_Velocity{velocity},
	m_LevelManagerPtr{levelManager},
	m_TextureManagerPtr{textureManager}
{
}

void EnemyStonerRock::Update(float elapsedSec)
{
	m_Velocity.y -= GRAVITY * elapsedSec;
	m_AnimationDuration += elapsedSec;

	m_Position.x += m_Velocity.x * elapsedSec;
	m_Position.y += m_Velocity.y * elapsedSec;

	m_LevelManagerPtr->CollisionCheck(Rectf{ m_Position.x - RADIUS, m_Position.y - RADIUS, RADIUS * 2.0f, RADIUS * 2.0f }, m_Velocity);
}

void EnemyStonerRock::Draw()
{

}
