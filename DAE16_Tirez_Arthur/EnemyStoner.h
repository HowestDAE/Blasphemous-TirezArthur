#pragma once
#include "Enemy.h"

class EnemyStoner final : public Enemy
{
public:
	explicit EnemyStoner(LevelManager* levelManager, TextureManager* textureManager, SoundManager* soundManager, Player* player, float x, float y);

	virtual void Draw() override;
	virtual void Update(float elapsedSec) override;
private:
	void CheckPlayerInteract();
	void SpawnRock();
	void RockHit();

	virtual void Idle() override;
	virtual void Attack() override;
	virtual void Death() override;

	float m_RockAnimationDuration{ 10.0f };
	bool m_RockActive{ false };
	Rectf m_RockHitbox{ m_HitBox.left, m_HitBox.bottom, 10.0f, 10.0f };
	float m_AttackCooldown{ 0.0f };

	static const float MAXHEALTH;
	static const float ATTACKDMG;
	static const float SPEED;
	static const int POINTS;
};

