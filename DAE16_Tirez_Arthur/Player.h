#pragma once
#include "Vector2f.h"
class TextureManager;
class LevelManager;
class EnemyManager;
class SoundManager;
class SaveManager;
class InputManager;

class Player final
{
public:
	explicit Player(TextureManager* textureManager, LevelManager* levelManager, EnemyManager* enemyManager, SoundManager* soundManager, InputManager* inputManager, SaveManager* saveManager);

	void Update(float elapsedSec);
	void Draw();

	Rectf& GetHitbox();
	bool Attack(Rectf& hurtbox, float damage, bool direction);
	bool IsDead();
	void Respawn();
	void SetTears(int tears);
	const float& GetHealth() const;
	const float& GetMaxHealth() const;
	const int& GetFlask() const;
	const int& GetMaxFlask() const;
	const int& GetTears() const;
private:
	enum class State {
		idle,
		run,
		jump,
		fall,
		crouch,
		dodge,
		ladder,
		ledge,
		ledgeclimb,
		death_spike,
		death,
		attack_part1,
		attack_part2,
		attack_part3,
		attack_jump,
		attack_crouch,
		block,
		parry,
		knockback,
		heal,
		pickup,
		shrine_activate,
		respawn
	};

	void HorizontalMovement(bool leftHeld, bool rightHeld);
	bool FallCheck();
	void Idle();
	void Jump();
	void Dodge();
	void Crouch();
	void Run();
	void Ladder();
	void Ledge();
	void LedgeClimb();
	void DeathSpike();
	void Death();
	void Attack1();
	void Attack2();
	void Attack3();
	void AttackJump();
	void AttackCrouch();
	void KnockBack();
	void Block();
	void Parry();
	void Heal();
	void Pickup();
	void ActivateShrine();

	TextureManager* m_TextureManagerPtr;
	EnemyManager* m_EnemyManagerPtr;
	LevelManager* m_LevelManagerPtr;
	SoundManager* m_SoundManagerPtr;
	InputManager* m_InputManagerPtr;
	SaveManager* m_SaveManagerPtr;

	Rectf m_HitBox;
	State m_PlayerState{ State::idle };
	Vector2f m_Velocity{ 0.0f, 0.0f };
	Point2f m_GuiltLocation{};
	bool m_LeftFacing{ false };
	float m_AnimationDuration{ 0.0f };
	float m_DodgeCooldown{ 0.0f };
	float m_LadderCooldown{ 0.0f };
	float m_JumpCooldown{ 0.0f };
	float m_LedgeCooldown{ 0.0f };
	float m_BlockCooldown{ 0.0f };
	float m_AttackCooldown{ 0.0f };
	float m_ComboTime{ 0.0f };
	int m_ComboCounter{ 0 };
	int m_Flasks{ MAXFLASKS };
	int m_AudioChannel{ -1 };
	int& m_Tears;
	float m_Health{ MAXHEALTH };

	static const float GRAVITY;
	static const float SPEED;
	static const float HITBOXHEIGHT;
	static const float HITBOXWIDTH;
	static const float MAXHEALTH;
	static const float ATTACKDMG;
	static const float HEAVYDMG;
	static const float COMBOTIME;
	static const int MAXFLASKS;
};

