#include "pch.h"
#include "Player.h"
#include "TextureManager.h"
#include "LevelManager.h"
#include <iostream>

Player::Player(TextureManager* textureManager, LevelManager* levelManager) :
	m_PlayerState{State::run},
	m_Velocity{0.f, 0.f},
	m_LeftFacing{false},
	m_TextureManagerPtr{ textureManager },
	m_LevelManagerPtr{ levelManager},
	m_AnimationDuration{0.0f},
	m_DodgeCooldown{0.0f}
{
	m_HitBox = Rectf{ 200.0f, 383.0f, HITBOXWIDTH, HITBOXHEIGHT };
}

void Player::Update(float elapsedSec)
{
	m_AnimationDuration += elapsedSec;
	m_DodgeCooldown -= elapsedSec;
	m_Velocity.y += -GRAVITY * elapsedSec;
	m_Velocity.y = std::max(m_Velocity.y, -500.0f);

	const Uint8* keyBoardState{ SDL_GetKeyboardState(nullptr) };
	bool leftHeld{ (bool)keyBoardState[SDL_SCANCODE_A] };
	bool rightHeld{ (bool)keyBoardState[SDL_SCANCODE_D] };
	bool spaceHeld{ (bool)keyBoardState[SDL_SCANCODE_SPACE] };
	bool downHeld{ (bool)keyBoardState[SDL_SCANCODE_S] };
	bool dodgeHeld{ (bool)keyBoardState[SDL_SCANCODE_LSHIFT] };

	switch (m_PlayerState)
	{
	case crouch:
		if (leftHeld) m_LeftFacing = true;
		else if (rightHeld) m_LeftFacing = false;

		if (!downHeld) Idle();
		if (dodgeHeld && m_DodgeCooldown < 0.0f) Dodge();

		break;
	case idle:
		if (FallCheck()) break;
		if (leftHeld && !rightHeld || !leftHeld && rightHeld) Run();
		if (spaceHeld) Jump();
		if (downHeld) Crouch();
		if (dodgeHeld && m_DodgeCooldown < 0.0f) Dodge();

		break;
	case run:
		if (FallCheck()) break;
		if (!leftHeld && !rightHeld || leftHeld && rightHeld) Idle();
		if (spaceHeld) Jump();
		if (downHeld) Crouch();
		if (dodgeHeld && m_DodgeCooldown < 0.0f) Dodge();

		HorizontalMovement(leftHeld, rightHeld);

		break;
	case jump:
		if (FallCheck()) break;

		HorizontalMovement(leftHeld, rightHeld);

		break;
	case fall:
		HorizontalMovement(leftHeld, rightHeld);

		break;
	case dodge:
		m_DodgeCooldown = 0.5f;

		if (FallCheck()) break;
		if (spaceHeld) Jump();

		if (m_AnimationDuration >= m_TextureManagerPtr->GetAnimationDuration("penitent_dodge")) Idle();

		break;
	default:
		break;
	}

	m_HitBox.left += m_Velocity.x * elapsedSec;
	m_HitBox.bottom += m_Velocity.y * elapsedSec;
	if (m_LevelManagerPtr->CollisionCheck(m_HitBox, m_Velocity) && m_PlayerState == State::fall && m_Velocity.y == 0.0f) Idle();
}

void Player::Draw()
{
	std::string animationPath{ "penitent_idle" };
	bool loop{ true };
	float frameTimeModifier{ 1.0f };
	switch (m_PlayerState)
	{
	case crouch:
		animationPath = "penitent_crouch";
		loop = false;
		break;
	case idle:
		animationPath = "penitent_idle";
		frameTimeModifier = 0.4f;
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
		frameTimeModifier = 0.4f;
		break;
	case dodge:
		animationPath = "penitent_dodge";
		loop = false;
		break;
	default:
		break;
	}
	m_TextureManagerPtr->Animate(animationPath, Point2f{ m_HitBox.left, m_HitBox.bottom }, m_AnimationDuration, m_LeftFacing, loop, frameTimeModifier);
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

bool Player::FallCheck()
{
	if (m_Velocity.y < - GRAVITY * 0.05f)
	{
		m_PlayerState = State::fall;
		m_AnimationDuration = 0.0f;
		return true;
	}
	return false;
}

void Player::Idle()
{
	m_PlayerState = State::idle;
	m_AnimationDuration = 0.0f;
	m_Velocity.x = 0.0f;
	m_HitBox.height = HITBOXHEIGHT;
}

void Player::Jump()
{
	m_PlayerState = State::jump;
	m_AnimationDuration = 0.0f;
	m_Velocity.y = 2 * GRAVITY / 5;
	m_HitBox.height = HITBOXHEIGHT;
}

void Player::Dodge()
{
	m_PlayerState = State::dodge;
	m_AnimationDuration = 0.0f;
	if (m_LeftFacing) m_Velocity.x = -SPEED * 1.35f;
	else m_Velocity.x = SPEED * 1.35f;
	m_HitBox.height = HITBOXHEIGHT / 2.0f;
}

void Player::Crouch()
{
	m_PlayerState = State::crouch;
	m_AnimationDuration = 0.0f;
	m_Velocity.x = 0.0f;
	m_HitBox.height = HITBOXHEIGHT / 2.0f;
}

void Player::Run()
{
	m_PlayerState = State::run;
	m_AnimationDuration = 0.0f;
	if (m_LeftFacing) m_Velocity.x = -SPEED;
	else m_Velocity.x = SPEED;
}
