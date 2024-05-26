#pragma once
#include <vector>

class TextureManager;
class LevelManager;
class Enemy;
class Player;
class SoundManager;

class EnemyManager final
{
public:
	enum class EnemyType {
		cartwheel,
		shieldmaiden,
		stoner,
		crucified
	};
	explicit EnemyManager(TextureManager* textureManager, SoundManager* soundManager);
	~EnemyManager();

	void SetLevelManager(LevelManager* levelManager);

	void Clear();
	void SetTargetPlayer(Player* player);
	void Draw();
	void Update(float elapsedSec);
	bool Attack(Rectf hitbox, float damage);
	void SpawnEnemy(EnemyType type, float x, float y);
private:
	TextureManager* m_TextureManagerPtr;
	LevelManager* m_LevelManagerPtr{ nullptr };
	Player* m_PlayerPtr{ nullptr };
	SoundManager* m_SoundManager;
	std::vector<Enemy*> m_EnemyPtrVector{};
};

