#pragma once
#include "Vector2f.h"
class TextureManager;
class LevelManager;

class Player final
{
public:
	explicit Player(TextureManager* textureManager, LevelManager* levelManager);

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
		death_spike
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

	TextureManager* m_TextureManagerPtr;
	const LevelManager* m_LevelManagerPtr;

	State m_PlayerState;
	Rectf m_HitBox;
	Vector2f m_Velocity;
	bool m_LeftFacing;
	float m_AnimationDuration;
	float m_DodgeCooldown;
	float m_LadderCooldown;
	float m_JumpCooldown;
	float m_LedgeCooldown;

	static const float GRAVITY;
	static const float SPEED;
	static const float HITBOXHEIGHT;
	static const float HITBOXWIDTH;
};

