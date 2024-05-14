#pragma once
#include "Vector2f.h"
class TextureManager;
class LevelManager;
class EnemyManager;

class Player final
{
public:
	explicit Player(TextureManager* textureManager, LevelManager* levelManager, EnemyManager* enemyManager);

	void Update(float elapsedSec);
	void Draw();

	Rectf& GetHitbox();
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
		attack_part1,
		attack_part2,
		attack_part3,
		attack_heavy,
		attack_jump,
		attack_crouch,
		block
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
	void Attack1();

	TextureManager* m_TextureManagerPtr;
	EnemyManager* m_EnemyManagerPtr;
	const LevelManager* m_LevelManagerPtr;

	Rectf m_HitBox;
	State m_PlayerState{ State::idle };
	Vector2f m_Velocity{ 0.0f, 0.0f };
	bool m_LeftFacing{ false };
	float m_ComboTime{ 0.0f };
	float m_AnimationDuration{ 0.0f };
	float m_DodgeCooldown{ 0.0f };
	float m_LadderCooldown{ 0.0f };
	float m_JumpCooldown{ 0.0f };
	float m_LedgeCooldown{ 0.0f };
	float m_BlockCooldown{ 0.0f };

	static const float GRAVITY;
	static const float SPEED;
	static const float HITBOXHEIGHT;
	static const float HITBOXWIDTH;
};

