#pragma once
class TextureManager;
class LevelManager;

enum State {
	idle,
	run,
	jump,
	fall
};
class Player
{
public:
	Player(TextureManager* textureManager, LevelManager* levelManager);
	~Player();

	void Update(float elapsedSec);
	void Draw();

	Rectf GetHitbox() const;
private:
	void HorizontalMovement(bool leftHeld, bool rightHeld);
	void FallCheck(float elapsedSec);

	TextureManager* m_TextureManagerPtr;
	const LevelManager* m_LevelManagerPtr;

	State m_PlayerState;
	Rectf m_HitBox;
	Point2f m_Velocity;
	bool m_LeftFacing;
	float m_AnimationDuration;

	const float GRAVITY{ 1000.0f };
	const float SPEED{ 170.0f };
};

