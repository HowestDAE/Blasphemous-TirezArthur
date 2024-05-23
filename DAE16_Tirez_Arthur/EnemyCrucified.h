#pragma once
#include "Enemy.h"

class EnemyCrucified final : public Enemy
{
public:
	explicit EnemyCrucified(LevelManager* levelManager, TextureManager* textureManager, Player* player, float x, float y);

	virtual void Draw() override;
	virtual void Update(float elapsedSec) override;
private:
	void CheckPlayerInteract();
	void PlayerHit();
	virtual void Attack() override;

	float m_AttackCooldown{ 0.0f };

	static const float MAXHEALTH;
	static const float SPEED;
	static const float ATTACKDMG;
	static const float POINTS;
};

