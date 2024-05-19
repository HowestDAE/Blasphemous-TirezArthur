#pragma once
#include <vector>

class TextureManager;
class LevelManager;
class Enemy;
class Player;

class EnemyManager final
{
public:
	explicit EnemyManager(LevelManager* levelManager, TextureManager* textureManager);
	~EnemyManager();

	void SetTargetPlayer(Player* player);
	void Draw();
	void Update(float elapsedSec);
	bool Attack(Rectf hitbox, float damage);
private:
	LevelManager* m_LevelManagerPtr;
	TextureManager* m_TextureManagerPtr;
	Player* m_PlayerPtr{ nullptr };
	std::vector<Enemy*> m_EnemyPtrVector{};
};

