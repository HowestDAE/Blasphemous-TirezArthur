#pragma once
#include "Vector2f.h"

class TextureManager;
class LevelManager;
class Player;

class Enemy
{
public:
	explicit Enemy(LevelManager* levelManager, TextureManager* textureManager, Player* player, float x, float y);
	explicit Enemy(const Enemy& other) = delete;
	explicit Enemy(const Enemy&& other) = delete;
	virtual ~Enemy() = default;

	Enemy& operator=(const Enemy& other) = delete;
	Enemy& operator=(const Enemy&& other) = delete;

	virtual void Draw() = 0;
	virtual void Update(float elapsedSec) = 0;
	virtual bool Hit(Rectf hitbox, float damage);
	bool IsDead();
protected:
	enum class State {
		idle,
		walk,
		attack,
		death,
		parried
	};

	void PlayerDistance(float& left, float& right);
	float PlayerDistance();

	virtual void Death();
	virtual void Idle();
	virtual void Walk();
	virtual void Parried();
	virtual void Attack();

	LevelManager* m_LevelManagerPtr;
	TextureManager* m_TextureManagerPtr;
	Player* m_PlayerPtr;
	Vector2f m_Velocity{};
	Rectf m_HitBox;
	float m_Health{ -1.0f };
	bool m_LeftFacing{ false };
	float m_AnimationDuration{ 0.0f };
	State m_State{ State::idle };

	static const float GRAVITY;
};

