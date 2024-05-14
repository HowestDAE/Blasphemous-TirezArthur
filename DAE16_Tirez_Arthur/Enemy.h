#pragma once
#include "Vector2f.h"

class TextureManager;
class LevelManager;

class Enemy
{
public:
	explicit Enemy(LevelManager* levelManager, TextureManager* textureManager, float x, float y);

	virtual void Draw() = 0;
	virtual void Update(float elapsedSec) = 0;
	bool Hit(Rectf hitbox, float damage);
	bool IsDead();
protected:
	enum class State {
		idle,
		walk,
		attack,
		death,
		parried
	};

	void Death();
	void Idle();
	void Walk();
	void Parried();
	void Attack();

	LevelManager* m_LevelManager;
	TextureManager* m_TextureManager;
	Vector2f m_Velocity;
	Rectf m_HitBox;
	float m_Health;
	bool m_LeftFacing;
	float m_AnimationDuration;
	State m_State;
};

