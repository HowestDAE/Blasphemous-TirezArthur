#pragma once
#include "Vector2f.h"
class TextureManager;
class LevelManager;

enum class State {
	idle,
	run,
	jump,
	fall,
	crouch,
	dodge,
	ladder,
	ledge
};
class Player final
{
public:
	Player(TextureManager* textureManager, LevelManager* levelManager);

	void Update(float elapsedSec);
	void Draw();

	Rectf& GetHitbox();
private:
	void HorizontalMovement(bool leftHeld, bool rightHeld);
	bool FallCheck();
	void Idle();
	void Jump();
	void Dodge();
	void Crouch();
	void Run();
	void Ladder();
	void Ledge();

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

	const float GRAVITY{ 1000.0f };
	const float SPEED{ 170.0f };
	const float HITBOXHEIGHT{ 50.0f };
	const float HITBOXWIDTH{ 18.0f };
};

