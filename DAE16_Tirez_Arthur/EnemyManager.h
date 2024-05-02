#pragma once
#include <vector>

class TextureManager;
class LevelManager;
class Enemy;

class EnemyManager final
{
public:
	explicit EnemyManager(LevelManager* levelManager, TextureManager* textureManager);
	~EnemyManager();

	void Draw();
	void Update(float elapsedSec);
private:
	LevelManager* m_LevelManager;
	TextureManager* m_TextureManager;
	std::vector<Enemy*> m_EnemyPtrVector{};
};

