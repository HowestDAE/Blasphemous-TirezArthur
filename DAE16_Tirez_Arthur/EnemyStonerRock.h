#pragma once
#include "Vector2f.h"

class LevelManager;
class TextureManager;

class EnemyStonerRock
{
public:
	explicit EnemyStonerRock(LevelManager* levelManager, TextureManager* textureManager, Point2f position, Vector2f velocity);

	void Update(float elapsedSec);
	void Draw();
private:
	LevelManager* m_LevelManagerPtr;
	TextureManager* m_TextureManagerPtr;

	Vector2f m_Velocity;
	Point2f m_Position;
	bool m_Hit{ false };
	float m_AnimationDuration{ 0.0f };

	static const float ROCKDMG;
	static const float GRAVITY;
	static const float RADIUS;
};

