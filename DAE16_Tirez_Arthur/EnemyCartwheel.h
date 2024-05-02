#pragma once
#include "Enemy.h"

class EnemyCartwheel final : public Enemy
{
public:
	explicit EnemyCartwheel(LevelManager* levelManager, TextureManager* textureManager, float x, float y);

	virtual void Draw() override;
	virtual void Update(float elapsedSec) override;
private:
	static const float MAXHEALTH;
	static const float GRAVITY;
	static const float SPEED;
};

