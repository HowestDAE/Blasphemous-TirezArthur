#include "pch.h"
#include "Player.h"
#include "TextureManager.h"
#include "LevelManager.h"
#include <iostream>

Player::Player(TextureManager* textureManager, LevelManager* levelManager) :
	m_HitBox{200.0f, 383.0f, 18.0f, 50.0f},
	m_PlayerState{State::run},
	m_Velocity{0.f, 0.f},
	m_LeftFacing{false},
	m_TextureManagerPtr{ textureManager },
	m_LevelManagerPtr{ levelManager},
	m_AnimationDuration{0.0f}
{
}

Player::~Player()
{
}

void Player::Update(float elapsedSec)
{
	m_AnimationDuration += elapsedSec;
	m_Velocity.y += - GRAVITY * elapsedSec;
	m_Velocity.y = std::max(m_Velocity.y, -500.0f);

	const Uint8* keyBoardState{ SDL_GetKeyboardState(nullptr) };
	bool leftHeld{ (bool)keyBoardState[SDL_SCANCODE_A] };
	bool rightHeld{ (bool)keyBoardState[SDL_SCANCODE_D] };
	bool spaceHeld{ (bool)keyBoardState[SDL_SCANCODE_SPACE] };

	switch (m_PlayerState)
	{
	case idle:
		FallCheck(elapsedSec);
		if (leftHeld && !rightHeld || !leftHeld && rightHeld)
		{
			m_PlayerState = State::run;
			m_AnimationDuration = 0.0f;
			if (leftHeld) m_Velocity.x = -SPEED;
			else m_Velocity.x = SPEED;
			break;
		}
		if (spaceHeld)
		{
			m_PlayerState = State::jump;
			m_AnimationDuration = 0.0f;
			m_Velocity.y = 2 * GRAVITY / 5;
			break;
		}
		break;
	case run:
		FallCheck(elapsedSec);
		if (!leftHeld && !rightHeld || leftHeld && rightHeld)
		{
			m_PlayerState = State::idle;
			m_AnimationDuration = 0.0f;
			m_Velocity.x = 0.0f;
			break;
		}
		if (spaceHeld)
		{
			m_PlayerState = State::jump;
			m_AnimationDuration = 0.0f;
			m_Velocity.y = 2 * GRAVITY / 5;
			break;
		}
		HorizontalMovement(leftHeld, rightHeld);
		break;
	case jump:
		FallCheck(elapsedSec);
		HorizontalMovement(leftHeld, rightHeld);
		break;
	case fall:
		HorizontalMovement(leftHeld, rightHeld);
		break;
	default:
		break;
	}

	m_HitBox.left += m_Velocity.x * elapsedSec;
	m_HitBox.bottom += m_Velocity.y * elapsedSec;
	if (m_LevelManagerPtr->CollisionCheck(m_HitBox, m_Velocity) && m_PlayerState == State::fall && m_Velocity.y == 0.0f)
	{
		m_PlayerState = State::idle;
		m_AnimationDuration = 0.0f;
		m_Velocity.x = 0.0f;
	}
}

void Player::Draw()
{
	std::string animationPath{ "penitent_idle" };
	bool loop{ true };
	switch (m_PlayerState)
	{
	case idle:
		animationPath = "penitent_idle";
		break;
	case run:
		animationPath = "penitent_run";
		break;
	case jump:
		animationPath = "penitent_jump";
		loop = false;
		break;
	case fall:
		animationPath = "penitent_fall";
		break;
	default:
		break;
	}
	m_TextureManagerPtr->Animate(animationPath, Point2f{ m_HitBox.left, m_HitBox.bottom }, m_AnimationDuration, m_LeftFacing, loop);
}

Rectf Player::GetHitbox() const
{
	return m_HitBox;
}

void Player::HorizontalMovement(bool leftHeld, bool rightHeld)
{
	if (leftHeld && !rightHeld)
	{
		m_LeftFacing = true;
		m_Velocity.x = -SPEED;
	}
	else if (!leftHeld && rightHeld)
	{
		m_LeftFacing = false;
		m_Velocity.x = SPEED;
	}
	else
	{
		m_Velocity.x = 0.0f;
	}
}

void Player::FallCheck(float elapsedSec)
{
	if (m_Velocity.y < - GRAVITY * 0.05f)
	{
		m_PlayerState = State::fall;
		m_AnimationDuration = 0.0f;
	}
}
