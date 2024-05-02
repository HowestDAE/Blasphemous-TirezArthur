#include "pch.h"
#include "Player.h"
#include "TextureManager.h"
#include "LevelManager.h"
#include <iostream>
#include "utils.h"

const float Player::GRAVITY{ 1000.0f };
const float Player::SPEED{ 170.0f };
const float Player::HITBOXHEIGHT{ 50.0f };
const float Player::HITBOXWIDTH{ 18.0f };

Player::Player(TextureManager* textureManager, LevelManager* levelManager) :
	m_PlayerState{ State::run },
	m_Velocity{ 0.f, 0.f },
	m_LeftFacing{ false },
	m_TextureManagerPtr{ textureManager },
	m_LevelManagerPtr{ levelManager },
	m_AnimationDuration{ 0.0f },
	m_DodgeCooldown{ 0.0f },
	m_LadderCooldown{ 0.0f },
	m_JumpCooldown{ 0.0f },
	m_LedgeCooldown{ 0.0f }
{
	m_HitBox = Rectf{ -1.0f, -1.0f, HITBOXWIDTH, HITBOXHEIGHT };
}

void Player::Update(float elapsedSec)
{
	m_AnimationDuration += elapsedSec;
	m_DodgeCooldown -= elapsedSec;
	m_LadderCooldown -= elapsedSec;
	m_JumpCooldown -= elapsedSec;
	m_LedgeCooldown -= elapsedSec;
	m_Velocity.y += -GRAVITY * elapsedSec;
	m_Velocity.y = std::max(m_Velocity.y, -500.0f);

	const Uint8* keyBoardState{ SDL_GetKeyboardState(nullptr) };
	bool leftHeld{ (bool)keyBoardState[SDL_SCANCODE_A] };
	bool rightHeld{ (bool)keyBoardState[SDL_SCANCODE_D] };
	bool downHeld{ (bool)keyBoardState[SDL_SCANCODE_S] };
	bool upHeld{ (bool)keyBoardState[SDL_SCANCODE_W] };
	bool spaceHeld{ (bool)keyBoardState[SDL_SCANCODE_SPACE] };
	bool dodgeHeld{ (bool)keyBoardState[SDL_SCANCODE_LSHIFT] };

	switch (m_PlayerState)
	{
	case State::crouch:
		if (leftHeld) m_LeftFacing = true;
		else if (rightHeld) m_LeftFacing = false;

		if (FallCheck()) break;
		if (m_LevelManagerPtr->Interact(LevelManager::Interactions::ladder, m_HitBox))
		{
			m_HitBox.bottom -= 15.0f;
			Ladder();
			break;
		}
		if (!downHeld) Idle();
		if (dodgeHeld && m_DodgeCooldown < 0.0f) Dodge();
		if (spaceHeld) m_HitBox.bottom -= 5.0f;

		break;
	case State::idle:
		if (FallCheck()) break;
		if (leftHeld && !rightHeld || !leftHeld && rightHeld) Run();
		if (upHeld && m_LevelManagerPtr->Interact(LevelManager::Interactions::ladder, m_HitBox) && m_LadderCooldown < 0.0f) Ladder();
		if (spaceHeld) Jump();
		if (downHeld) Crouch();
		if (dodgeHeld && m_DodgeCooldown < 0.0f) Dodge();

		break;
	case State::run:
		HorizontalMovement(leftHeld, rightHeld);

		if (FallCheck()) break;
		if (!leftHeld && !rightHeld || leftHeld && rightHeld) Idle();
		if (upHeld && m_LevelManagerPtr->Interact(LevelManager::Interactions::ladder, m_HitBox) && m_LadderCooldown < 0.0f) Ladder();
		if (spaceHeld) Jump();
		if (downHeld) Crouch();
		if (dodgeHeld && m_DodgeCooldown < 0.0f) Dodge();

		break;
	case State::jump:
		HorizontalMovement(leftHeld, rightHeld);

		if ((leftHeld || rightHeld) && m_LedgeCooldown < 0.0f && m_LevelManagerPtr->Interact(LevelManager::Interactions::ledge, m_HitBox, m_Velocity)) Ledge();
		if (upHeld && m_LevelManagerPtr->Interact(LevelManager::Interactions::ladder, m_HitBox) && m_LadderCooldown < 0.0f) Ladder();
		if (FallCheck()) break;

		break;
	case State::fall:
		HorizontalMovement(leftHeld, rightHeld);

		if ((leftHeld || rightHeld) && m_LedgeCooldown < 0.0f && m_LevelManagerPtr->Interact(LevelManager::Interactions::ledge, m_HitBox, m_Velocity)) Ledge();
		if (upHeld && m_LevelManagerPtr->Interact(LevelManager::Interactions::ladder, m_HitBox) && m_LadderCooldown < 0.0f) Ladder();
		if (m_LevelManagerPtr->Interact(LevelManager::Interactions::spike, m_HitBox)) DeathSpike();

		break;
	case State::dodge:
		m_DodgeCooldown = 0.5f;

		if (FallCheck()) break;
		if (spaceHeld) Jump();

		if (m_AnimationDuration >= m_TextureManagerPtr->GetAnimationDuration("penitent_dodge")) Idle();

		break;
	case State::ladder:
		m_LadderCooldown = 0.4f;
		m_AnimationDuration -= elapsedSec;
		if (!upHeld && !downHeld)
		{
			m_Velocity.y = 0.0f;
		}
		else if (!upHeld && downHeld)
		{
			m_AnimationDuration -= elapsedSec;
			m_Velocity.y = -SPEED * 0.6f;
		}
		else
		{
			m_AnimationDuration += elapsedSec;
			m_Velocity.y = SPEED * 0.6f;
		}

		if (!m_LevelManagerPtr->Interact(LevelManager::Interactions::ladder, m_HitBox)) Idle();
		if (spaceHeld && m_JumpCooldown < 0.0f) Jump();

		break;
	case State::ledge:
		m_LedgeCooldown = 0.4f;

		if (!downHeld) m_Velocity.y = 0.0f;
		if (upHeld && m_AnimationDuration > 0.5f) LedgeClimb();

		FallCheck();

		break;
	case State::ledgeclimb:
		if (m_AnimationDuration > m_TextureManagerPtr->GetAnimationDuration("penitent_ledge_climb")) Idle();
	default:
		break;
	}

	m_HitBox.left += m_Velocity.x * elapsedSec;
	m_HitBox.bottom += m_Velocity.y * elapsedSec;
	if (m_LevelManagerPtr->CollisionCheck(m_HitBox, m_Velocity) && (m_PlayerState == State::fall || m_PlayerState == State::ladder) && m_Velocity.y == 0.0f) Idle();
}

void Player::Draw()
{
	std::string animationPath{ "penitent_idle" };
	bool loop{ true };
	float frameTimeModifier{ 1.0f };
	switch (m_PlayerState)
	{
	case State::crouch:
		animationPath = "penitent_crouch";
		loop = false;
		break;
	case State::idle:
		animationPath = "penitent_idle";
		frameTimeModifier = 0.3f;
		break;
	case State::run:
		animationPath = "penitent_run";
		break;
	case State::jump:
		animationPath = "penitent_jump";
		loop = false;
		break;
	case State::fall:
		animationPath = "penitent_fall";
		frameTimeModifier = 0.4f;
		break;
	case State::dodge:
		animationPath = "penitent_dodge";
		loop = false;
		break;
	case State::ladder:
		animationPath = "penitent_ladder";
		frameTimeModifier = 0.4f;
		break;
	case State::ledge:
		animationPath = "penitent_ledge";
		loop = false;
		break;
	case State::ledgeclimb:
		animationPath = "penitent_ledge_climb";
		loop = false;
		frameTimeModifier = 0.5f;
		break;
	case State::death_spike:
		animationPath = "penitent_death_spike";
		loop = false;
		break;
	default:
		break;
	}
	m_TextureManagerPtr->Animate(animationPath, Point2f{ m_HitBox.left, m_HitBox.bottom }, m_AnimationDuration, m_LeftFacing, loop, frameTimeModifier);
}

Rectf& Player::GetHitbox()
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
		m_HitBox.height = HITBOXHEIGHT;
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

void Player::Ladder()
{
	m_PlayerState = State::ladder;
	m_AnimationDuration = 0.0f;
	m_JumpCooldown = 0.5f;
	m_Velocity.x = 0.0f;
	m_Velocity.y = 0.0f;
}

void Player::Ledge()
{
	m_PlayerState = State::ledge;
	m_AnimationDuration = 0.0f;
	m_Velocity.x = 0.0f;
	m_Velocity.y = 0.0f;
}

void Player::LedgeClimb()
{
	m_PlayerState = State::ledgeclimb;
	m_AnimationDuration = 0.0f;
	m_HitBox.bottom += m_HitBox.height + 0.1f;
	if (m_LeftFacing) m_HitBox.left -= m_HitBox.width;
	else m_HitBox.left += m_HitBox.width;
}

void Player::DeathSpike()
{
	m_PlayerState = State::death_spike;
	m_AnimationDuration = 0.0f;
	m_Velocity.x = 0.0f;
	m_Velocity.y = 0.0f;
}
