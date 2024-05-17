#pragma once
#include "Enemy.h"
#include <utils.h>

class EnemyCartwheel final : public Enemy
{
public:
	explicit EnemyCartwheel(LevelManager* levelManager, TextureManager* textureManager, Player* player, float x, float y);

	virtual void Draw() override;
	virtual void Update(float elapsedSec) override;
private:
	void PlayerDistance(float& left, float& right);
	float PlayerDistance();
	bool CheckPlayerInteract();
	static const float MAXHEALTH;
	static const float GRAVITY;
	static const float SPEED;
};

