#pragma once
#include "Enemy.h"
#include <utils.h>

class EnemyCartwheel final : public Enemy
{
public:
	explicit EnemyCartwheel(LevelManager* levelManager, TextureManager* textureManager, SoundManager* soundManager, Player* player, float x, float y);

	virtual void Draw() override;
	virtual void Update(float elapsedSec) override;
private:
	void CheckPlayerInteract();
	void PlayerHit();
	virtual void Attack() override;
	virtual void Death() override;

	float m_AttackCooldown{ 0.0f };
	int m_AudioChannel{ -1 };

	static const float MAXHEALTH;
	static const float SPEED;
	static const float ATTACKDMG;
	static const float POINTS;
};

