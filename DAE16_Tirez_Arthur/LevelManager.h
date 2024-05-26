#pragma once
#include <vector>
#define JSON_IS_AMALGAMATION
#include "json/json-forwards.h"
#include "Vector2f.h"
class TextureManager;
class EnemyManager;
class Camera;

struct Door {
	Rectf hitbox;
	bool flipped;
	std::string texture;
	std::string destination;
	Point2f destinationPos;
};

struct Platform {
	Rectf hitbox;
	bool rightGrabbable;
	bool leftGrabbable;
};

class LevelManager final
{
public:
	explicit LevelManager(TextureManager* textureManager, EnemyManager* enemyManager, Camera* camera);

	enum class Interactions {
		ladder,
		ledge,
		spike
	};

	bool Interact(Interactions interaction, Rectf& hitbox, const Vector2f& velocity = Vector2f{ 0.0f, 0.0f }) const;
	bool CollisionCheck(Rectf& hitbox, Vector2f& velocity, const bool ignorePlatforms = false);
	bool CollisionCheck(Rectf& hitbox, const bool ignorePlatforms = false);

	void DrawBackGround();
	void DrawForeground();
	void LoadLevel(std::string path);
private:

	TextureManager* m_TextureManagerPtr{};
	EnemyManager* m_EnemyManagerPtr{};
	Camera* m_CameraPtr{};

	std::vector<Rectf> m_LevelGeometry;
	std::vector<Platform> m_LevelPlatforms;
	std::vector<Door> m_LevelDoors;
	std::vector<Rectf> m_LevelLadders;
	std::vector<Rectf> m_LevelSpikes;
	std::vector<std::string> m_LevelBackground;
	std::string m_CurrentLevel;
};