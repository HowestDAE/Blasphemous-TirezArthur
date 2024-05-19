#pragma once
#include "Enemy.h"

class ShieldMaiden final : public Enemy
{
public:
	explicit ShieldMaiden(LevelManager* levelManager, TextureManager* textureManager, Player* player, float x, float y);

	virtual void Draw() override;
	virtual void Update(float elapsedSec) override;
	virtual bool Hit(Rectf hitbox, float damage) override;
private:
	void CheckPlayerInteract();
	void PlayerHit();
	void PlayerHitShield();
	virtual void Attack() override;

	float m_AttackCooldown{ 0.0f };

	static const float MAXHEALTH;
	static const float SPEED;
	static const float ATTACKDMG;
	static const float SHIELDDMG;
	static const float POINTS;
};

