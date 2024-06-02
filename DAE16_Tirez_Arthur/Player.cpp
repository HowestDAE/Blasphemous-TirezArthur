#include "pch.h"
#include "Player.h"
#include "TextureManager.h"
#include "LevelManager.h"
#include "SoundManager.h"
#include "InputManager.h"
#include <iostream>
#include "utils.h"
#include "EnemyManager.h"

const float Player::GRAVITY{ 1000.0f };
const float Player::SPEED{ 170.0f };
const float Player::HITBOXHEIGHT{ 50.0f };
const float Player::HITBOXWIDTH{ 18.0f };
const float Player::MAXHEALTH{ 100.0f };
const float Player::ATTACKDMG{ 13.0f };
const float Player::HEAVYDMG{ 30.0f };
const float Player::COMBOTIME{ 4.0f };
const int Player::MAXFLASKS{ 2 };

Player::Player(TextureManager* textureManager, LevelManager* levelManager, EnemyManager* enemyManager, SoundManager* soundManager, InputManager* inputManager) :
	m_TextureManagerPtr{ textureManager },
	m_LevelManagerPtr{ levelManager },
	m_EnemyManagerPtr{ enemyManager },
	m_SoundManagerPtr{ soundManager },
	m_InputManagerPtr{inputManager}
{
	m_HitBox = Rectf{ 200.0f, 100.0f, HITBOXWIDTH, HITBOXHEIGHT };
}

void Player::Update(float elapsedSec)
{
	m_AnimationDuration += elapsedSec;
	m_DodgeCooldown -= elapsedSec;
	m_LadderCooldown -= elapsedSec;
	m_JumpCooldown -= elapsedSec;
	m_LedgeCooldown -= elapsedSec;
	m_BlockCooldown -= elapsedSec;
	m_AttackCooldown -= elapsedSec;
	m_ComboTime -= elapsedSec;
	m_Velocity.y += -GRAVITY * elapsedSec;
	m_Velocity.y = std::max(m_Velocity.y, -500.0f);

	const bool leftHeld{ m_InputManagerPtr->GetKeyState(InputManager::Keybind::moveLeft, false) };
	const bool rightHeld{ m_InputManagerPtr->GetKeyState(InputManager::Keybind::moveRight, false) };
	const bool downHeld{ m_InputManagerPtr->GetKeyState(InputManager::Keybind::moveDown, false) };
	const bool upHeld{ m_InputManagerPtr->GetKeyState(InputManager::Keybind::moveUp, false) };

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
		if (m_DodgeCooldown < 0.0f && m_InputManagerPtr->GetKeyState(InputManager::Keybind::dodge)) Dodge();
		if (m_BlockCooldown < 0.0f && m_InputManagerPtr->GetKeyState(InputManager::Keybind::block)) Block();
		if (m_AttackCooldown < 0.0f && m_InputManagerPtr->GetKeyState(InputManager::Keybind::attack)) AttackCrouch();
		if (m_Health < 0.0001f) Death();

		break;
	case State::idle:
		if (FallCheck()) break;
		if (leftHeld && !rightHeld || !leftHeld && rightHeld) Run();
		m_HitBox.bottom += 1.0f;
		if (upHeld && m_LevelManagerPtr->Interact(LevelManager::Interactions::ladder, m_HitBox) && m_LadderCooldown < 0.0f) Ladder();
		if (m_InputManagerPtr->GetKeyState(InputManager::Keybind::interact) && m_LevelManagerPtr->Interact(LevelManager::Interactions::pickup, m_HitBox)) Pickup();
		m_HitBox.bottom -= 1.0f;
		if (m_InputManagerPtr->GetKeyState(InputManager::Keybind::jump)) Jump();
		if (downHeld) Crouch();
		if (m_ComboTime >= 0.0f && m_ComboCounter == 2 && m_InputManagerPtr->GetKeyState(InputManager::Keybind::attack)) {
			Attack3();
			break;
		}
		if (m_ComboTime >= 0.0f && m_ComboCounter == 1 && m_InputManagerPtr->GetKeyState(InputManager::Keybind::attack)) {
			Attack2();
			break;
		}
		if (m_AttackCooldown < 0.0f && (m_ComboTime < 0.0f || m_ComboCounter == 0) && m_InputManagerPtr->GetKeyState(InputManager::Keybind::attack)) {
			Attack1();
			break;
		}
		if (m_DodgeCooldown < 0.0f && m_InputManagerPtr->GetKeyState(InputManager::Keybind::dodge)) Dodge();
		if (m_BlockCooldown < 0.0f && m_InputManagerPtr->GetKeyState(InputManager::Keybind::block)) Block();
		if (m_Health < MAXHEALTH && m_Flasks > 0 && m_InputManagerPtr->GetKeyState(InputManager::Keybind::flask)) Heal();
		if (m_Health < 0.0001f) Death();

		break;
	case State::run:
		HorizontalMovement(leftHeld, rightHeld);
		if (m_AudioChannel == -1 || !m_SoundManagerPtr->IsPlaying("penitent_run_stone", m_AudioChannel)) m_AudioChannel = m_SoundManagerPtr->Play("penitent_run_stone");

		if (FallCheck()) break;
		if (!leftHeld && !rightHeld || leftHeld && rightHeld) Idle();
		if (upHeld && m_LevelManagerPtr->Interact(LevelManager::Interactions::ladder, m_HitBox) && m_LadderCooldown < 0.0f) Ladder();
		if (m_InputManagerPtr->GetKeyState(InputManager::Keybind::jump)) Jump();
		if (downHeld) Crouch();
		if (m_ComboTime >= 0.0f && m_ComboCounter == 2 && m_InputManagerPtr->GetKeyState(InputManager::Keybind::attack)) {
			Attack3();
			break;
		}
		if (m_ComboTime >= 0.0f && m_ComboCounter == 1 && m_InputManagerPtr->GetKeyState(InputManager::Keybind::attack)) {
			Attack2();
			break;
		}
		if (m_AttackCooldown < 0.0f && (m_ComboTime < 0.0f || m_ComboCounter == 0) && m_InputManagerPtr->GetKeyState(InputManager::Keybind::attack)) {
			Attack1();
			break;
		}
		if (m_DodgeCooldown < 0.0f && m_InputManagerPtr->GetKeyState(InputManager::Keybind::dodge)) Dodge();
		if (m_BlockCooldown < 0.0f && m_InputManagerPtr->GetKeyState(InputManager::Keybind::block)) Block();
		if (m_Health < 0.0001f) Death();

		break;
	case State::jump:
		HorizontalMovement(leftHeld, rightHeld);

		if ((leftHeld || rightHeld) && m_LedgeCooldown < 0.0f && m_LevelManagerPtr->Interact(LevelManager::Interactions::ledge, m_HitBox, m_Velocity)) Ledge();
		if (upHeld && m_LevelManagerPtr->Interact(LevelManager::Interactions::ladder, m_HitBox) && m_LadderCooldown < 0.0f) Ladder();
		if (m_AttackCooldown < 0.0f && m_InputManagerPtr->GetKeyState(InputManager::Keybind::attack)) AttackJump();
		if (m_Health < 0.0001f) Death();
		if (FallCheck()) break;

		break;
	case State::fall:
		HorizontalMovement(leftHeld, rightHeld);

		if ((leftHeld || rightHeld) && m_LedgeCooldown < 0.0f && m_LevelManagerPtr->Interact(LevelManager::Interactions::ledge, m_HitBox, m_Velocity)) Ledge();
		if ((upHeld || downHeld) && m_LevelManagerPtr->Interact(LevelManager::Interactions::ladder, m_HitBox) && m_LadderCooldown < 0.0f) Ladder();
		if (m_Health < 0.0001f) Death();
		if (m_LevelManagerPtr->Interact(LevelManager::Interactions::spike, m_HitBox)) DeathSpike();
		if (m_AttackCooldown < 0.0f && m_InputManagerPtr->GetKeyState(InputManager::Keybind::attack)) AttackJump();

		break;
	case State::dodge:
		m_DodgeCooldown = 0.5f;

		if (FallCheck()) break;
		if (m_InputManagerPtr->GetKeyState(InputManager::Keybind::jump)) Jump();
		if (m_BlockCooldown < 0.0f && m_InputManagerPtr->GetKeyState(InputManager::Keybind::block)) Block();
		if (m_Health < 0.0001f) Death();

		if (m_AnimationDuration >= m_TextureManagerPtr->GetAnimationDuration("penitent_dodge")) Idle();

		break;
	case State::ladder:
		m_LadderCooldown = 0.4f;
		m_AnimationDuration -= elapsedSec;
		if (!upHeld && !downHeld) {
			m_Velocity.y = 0.0f;
		}
		else if (!upHeld && downHeld) {
			m_AnimationDuration -= elapsedSec;
			m_Velocity.y = -SPEED * 0.6f;
			if (m_AudioChannel == -1 || !m_SoundManagerPtr->IsPlaying("penitent_climb_ladder", m_AudioChannel)) m_AudioChannel = m_SoundManagerPtr->Play("penitent_climb_ladder");
		}
		else {
			m_AnimationDuration += elapsedSec;
			m_Velocity.y = SPEED * 0.6f;
			if (m_AudioChannel == -1 || !m_SoundManagerPtr->IsPlaying("penitent_climb_ladder", m_AudioChannel)) m_AudioChannel = m_SoundManagerPtr->Play("penitent_climb_ladder");
		}

		if (!m_LevelManagerPtr->Interact(LevelManager::Interactions::ladder, m_HitBox)) Idle();
		if (m_JumpCooldown < 0.0f && m_InputManagerPtr->GetKeyState(InputManager::Keybind::jump)) Jump();
		if (m_Health < 0.0001f) Death();

		break;
	case State::ledge:
		m_LedgeCooldown = 0.4f;

		if (!downHeld) m_Velocity.y = 0.0f;
		if (upHeld && m_AnimationDuration > 0.5f) LedgeClimb();
		if (m_Health < 0.0001f) Death();

		FallCheck();

		break;
	case State::ledgeclimb:
		if (m_AnimationDuration > m_TextureManagerPtr->GetAnimationDuration("penitent_ledge_climb")) Idle();
		if (m_Health < 0.0001f) Death();

		break;
	case State::attack_part1:
		m_AttackCooldown = 0.2f;
		if (m_AnimationDuration > m_TextureManagerPtr->GetAnimationDuration("penitent_attack_part1")) Idle();
		if (m_Health < 0.0001f) Death();

		break;
	case State::attack_part2:
		m_AttackCooldown = 0.2f;
		if (m_AnimationDuration > m_TextureManagerPtr->GetAnimationDuration("penitent_attack_part2")) Idle();
		if (m_Health < 0.0001f) Death();

		break;
	case State::attack_part3:
		m_AttackCooldown = 0.2f;
		if (m_AnimationDuration > m_TextureManagerPtr->GetAnimationDuration("penitent_attack_part3")) Idle();
		if (m_Health < 0.0001f) Death();

		break;
	case State::attack_jump:
		m_AttackCooldown = 0.5f;
		HorizontalMovement(leftHeld, rightHeld);

		if (m_AnimationDuration > m_TextureManagerPtr->GetAnimationDuration("penitent_attack_jump")) m_PlayerState = State::fall;
		if (m_Health < 0.0001f) Death();
		if (m_LevelManagerPtr->Interact(LevelManager::Interactions::spike, m_HitBox)) DeathSpike();

		break;
	case State::attack_crouch :
		m_AttackCooldown = 0.3f;

		if (m_AnimationDuration > m_TextureManagerPtr->GetAnimationDuration("penitent_attack_crouch")) Crouch();
		if (m_Health < 0.0001f) Death();

		break;
	case State::block:
		m_BlockCooldown = 0.5f;

		if (m_AnimationDuration > m_TextureManagerPtr->GetAnimationDuration("penitent_block")) Idle();
		if (m_Health < 0.0001f) Death();

		break;
	case State::parry:
		if (m_AnimationDuration > m_TextureManagerPtr->GetAnimationDuration("penitent_parry")) Idle();
		if (m_Health < 0.0001f) Death();

		break;
	case State::knockback:
		if (m_LevelManagerPtr->Interact(LevelManager::Interactions::spike, m_HitBox)) DeathSpike();

		break;
	case State::heal:
		if (m_AnimationDuration > m_TextureManagerPtr->GetAnimationDuration("penitent_heal")) Idle();
		if (m_Health < 0.0001f) Death();
		break;
	case State::pickup:
		if (m_AnimationDuration > m_TextureManagerPtr->GetAnimationDuration("penitent_pickup")) Idle();
		if (m_Health < 0.0001f) Death();
	default:
		break;
	}

	m_HitBox.left += m_Velocity.x * elapsedSec;
	m_HitBox.bottom += m_Velocity.y * elapsedSec;
	const bool verticalCollision{ m_LevelManagerPtr->CollisionCheck(m_HitBox, m_Velocity, downHeld && m_InputManagerPtr->GetKeyState(InputManager::Keybind::jump)) && m_Velocity.y == 0.0f };
	if (verticalCollision) {
		switch (m_PlayerState)
		{
		case State::fall:
		case State::knockback:
		case State::attack_jump:
			m_SoundManagerPtr->Play("penitent_land");
		case State::ladder:
			Idle();
			break;
		case State::jump:
			m_PlayerState = State::fall;
			break;
		default:
			break;
		}
	}
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
	case State::death:
		animationPath = "penitent_death";
		loop = false;
		break;
	case State::knockback:
		animationPath = "penitent_knockback";
		loop = false;
		break;
	case State::attack_part1:
		animationPath = "penitent_attack_part1";
		loop = false;
		break;
	case State::attack_part2:
		animationPath = "penitent_attack_part2";
		loop = false;
		break;
	case State::attack_part3:
		animationPath = "penitent_attack_part3";
		loop = false;
		break;
	case State::attack_jump:
		animationPath = "penitent_attack_jump";
		loop = false;
		break;
	case State::attack_crouch:
		animationPath = "penitent_attack_crouch";
		loop = false;
		break;
	case State::block:
		animationPath = "penitent_block";
		loop = false;
		break;
	case State::parry:
		animationPath = "penitent_parry";
		loop = false;
		break;
	case State::heal:
		animationPath = "penitent_heal";
		loop = false;
	case State::pickup:
		animationPath = "penitent_pickup";
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

bool Player::Attack(Rectf& hurtbox, float damage, bool direction)
{
	bool hit{ utils::IsOverlapping(m_HitBox, hurtbox) && m_PlayerState != State::knockback && m_PlayerState != State::death && m_PlayerState != State::death_spike };
	if (hit) {
		if (m_PlayerState == State::block) {
			m_LeftFacing = !direction;
			if (damage <= MAXHEALTH * 0.25f) Parry();
			else {
				const float pushbackSpeed{400.0f};
				const float pushbackDuration{ 0.15f };
				if (!direction) m_Velocity.x = pushbackSpeed;
				else m_Velocity.x = -pushbackSpeed;
				m_AnimationDuration = pushbackDuration;
				m_SoundManagerPtr->Play("penitent_block");
			}
			return true;
		}
		m_Health -= damage;
		m_LeftFacing = !direction;
		m_SoundManagerPtr->Play("penitent_damage");
		KnockBack();
	}
	return false;
}

bool Player::IsDead()
{
	return (m_PlayerState == State::death || m_PlayerState == State::death_spike);
}

void Player::SetTears(int tears)
{
	m_Tears = tears;
}

const float& Player::GetHealth() const
{
	return m_Health;
}

const float& Player::GetMaxHealth() const
{
	return MAXHEALTH;
}

const int& Player::GetFlask() const
{
	return m_Flasks;
}

const int& Player::GetMaxFlask() const
{
	return MAXFLASKS;
}

const int& Player::GetTears() const
{
	return m_Tears;
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
	m_SoundManagerPtr->Play("penitent_jump");
}

void Player::Dodge()
{
	m_PlayerState = State::dodge;
	m_SoundManagerPtr->Play("penitent_dash");
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
	m_Velocity.y = 0.0f;
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
	m_HitBox.height = HITBOXHEIGHT;
}

void Player::Ledge()
{
	m_PlayerState = State::ledge;
	m_AnimationDuration = 0.0f;
	m_Velocity.x = 0.0f;
	m_Velocity.y = 0.0f;
	m_SoundManagerPtr->Play("penitent_edge_grab");
}

void Player::LedgeClimb()
{
	m_PlayerState = State::ledgeclimb;
	m_AnimationDuration = 0.0f;
	m_HitBox.bottom += m_HitBox.height + 0.1f;
	if (m_LeftFacing) m_HitBox.left -= m_HitBox.width;
	else m_HitBox.left += m_HitBox.width;
	m_SoundManagerPtr->Play("penitent_edge_climb");
}

void Player::DeathSpike()
{
	m_PlayerState = State::death_spike;
	m_Health = 0.0f;
	m_AnimationDuration = 0.0f;
	m_Velocity.x = 0.0f;
	m_Velocity.y = 0.0f;
	m_SoundManagerPtr->Play("penitent_death_spike");
}

void Player::Death()
{
	m_PlayerState = State::death;
	m_Health = 0.0f;
	m_AnimationDuration = 0.0f;
	m_Velocity.x = 0.0f;
	m_Velocity.y = 0.0f;
	m_SoundManagerPtr->Play("penitent_death");
}

void Player::Attack1()
{
	m_PlayerState = State::attack_part1;
	m_AnimationDuration = 0.0f;
	m_Velocity.x = 0.0f;
	m_Velocity.y = 0.0f;
	Rectf hurtBox{ m_HitBox.left + m_HitBox.width, m_HitBox.bottom + 10.0f, 72.0f, 40.0f };
	if (m_LeftFacing)
		hurtBox.left = m_HitBox.left - 72.0f;
	if (m_EnemyManagerPtr->Attack(hurtBox, ATTACKDMG)) {
		m_ComboCounter = 1;
		m_ComboTime = m_TextureManagerPtr->GetAnimationDuration("penitent_attack_part1") + COMBOTIME;
		m_SoundManagerPtr->Play("penitent_hit_1");
	}
	else {
		m_ComboCounter = 0;
		m_ComboTime = 0.0f;
		m_SoundManagerPtr->Play("penitent_miss");
	}
}

void Player::Attack2()
{
	m_PlayerState = State::attack_part2;
	m_AnimationDuration = 0.0f;
	m_Velocity.x = 0.0f;
	m_Velocity.y = 0.0f;
	Rectf hurtBox{ m_HitBox.left + m_HitBox.width * 0.5f, m_HitBox.bottom + 10.0f, 80.0f, 50.0f };
	if (m_LeftFacing)
		hurtBox.left = m_HitBox.left - 80.0f;
	if (m_EnemyManagerPtr->Attack(hurtBox, ATTACKDMG)) {
		m_ComboCounter = 2;
		m_ComboTime = m_TextureManagerPtr->GetAnimationDuration("penitent_attack_part2") + COMBOTIME;
		m_SoundManagerPtr->Play("penitent_hit_2");
	}
	else {
		m_ComboCounter = 0;
		m_ComboTime = 0.0f;
		m_SoundManagerPtr->Play("penitent_miss");
	}
}

void Player::Attack3()
{
	m_PlayerState = State::attack_part3;
	m_AnimationDuration = 0.0f;
	m_Velocity.x = 0.0f;
	m_Velocity.y = 0.0f;
	Rectf hurtBox{ m_HitBox.left + m_HitBox.width * 0.5f, m_HitBox.bottom, 77.0f, 84.0f };
	if (m_LeftFacing)
		hurtBox.left = m_HitBox.left - 77.0f;
	if (m_EnemyManagerPtr->Attack(hurtBox, ATTACKDMG)) {
		m_ComboCounter = 0;
		m_SoundManagerPtr->Play("penitent_hit_3");
	}
	else {
		m_ComboCounter = 0;
		m_ComboTime = 0.0f;
		m_SoundManagerPtr->Play("penitent_miss");
	}
}

void Player::AttackJump()
{
	m_PlayerState = State::attack_jump;
	m_AnimationDuration = 0.0f;
	Rectf hurtBox{ m_HitBox.left + m_HitBox.width, m_HitBox.bottom, 90.0f, 65.0f };
	if (m_LeftFacing)
		hurtBox.left = m_HitBox.left - 90.0f;
	if (m_EnemyManagerPtr->Attack(hurtBox, ATTACKDMG)) m_SoundManagerPtr->Play("penitent_hit_1");
	else m_SoundManagerPtr->Play("penitent_miss");
}

void Player::AttackCrouch()
{
	m_PlayerState = State::attack_crouch;
	m_AnimationDuration = 0.0f;
	m_Velocity.x = 0.0f;
	m_Velocity.y = 0.0f;
	Rectf hurtBox{ m_HitBox.left + m_HitBox.width, m_HitBox.bottom - 4.0f, 63.0f, 41.0f };
	if (m_LeftFacing)
		hurtBox.left = m_HitBox.left - 63.0f;
	if (m_EnemyManagerPtr->Attack(hurtBox, ATTACKDMG)) m_SoundManagerPtr->Play("penitent_hit_1");
	else m_SoundManagerPtr->Play("penitent_miss");
}

void Player::KnockBack()
{
	m_PlayerState = State::knockback;
	m_AnimationDuration = 0.0f;
	m_Velocity.y = GRAVITY * 0.2f;
	if (m_LeftFacing)m_Velocity.x = SPEED * 1.5f;
	else m_Velocity.x = -SPEED * 1.5;
	m_SoundManagerPtr->Play("penitent_knockback");
}

void Player::Block()
{
	m_PlayerState = State::block;
	m_AnimationDuration = 0.0f;
	m_Velocity.x = 0.0f;
	m_SoundManagerPtr->Play("penitent_guard");
}

void Player::Parry()
{
	m_PlayerState = State::parry;
	m_AnimationDuration = 0.0f;
	Rectf hurtBox{ m_HitBox.left + m_HitBox.width * 0.5f, m_HitBox.bottom, 82.0f, 100.0f };
	if (m_LeftFacing)
		hurtBox.left = m_HitBox.left - 82.0f;
	if (m_EnemyManagerPtr->Attack(hurtBox, HEAVYDMG)) m_SoundManagerPtr->Play("penitent_parry");
	else m_SoundManagerPtr->Play("penitent_block");
}

void Player::Heal()
{
	m_PlayerState = State::heal;
	m_AnimationDuration = 0.0f;
	const float heal{ 50.0f };
	m_Flasks -= 1;
	m_Health = std::min(MAXHEALTH, m_Health + heal);
	m_SoundManagerPtr->Play("penitent_heal");
}

void Player::Pickup()
{
	m_PlayerState = State::pickup;
	m_AnimationDuration = 0.0f;
	m_Velocity.x = 0.0f;
}
