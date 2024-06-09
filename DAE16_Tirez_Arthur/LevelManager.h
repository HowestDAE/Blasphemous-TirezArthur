#pragma once
#include <vector>
#define JSON_IS_AMALGAMATION
#include "json/json-forwards.h"
#include "Vector2f.h"
#include "SaveManager.h"

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

struct Object {
	Point2f pos;
	Item item;
};

struct Shrine {
	Rectf hitbox;
	bool enabled;
};

class LevelManager final
{
public:
	explicit LevelManager(TextureManager* textureManager, EnemyManager* enemyManager, Camera* camera, SaveManager* saveManager);

	enum class Interactions {
		ladder,
		ledge,
		spike,
		pickup,
		shrine
	};

	bool Interact(Interactions interaction, Rectf& hitbox, const Vector2f& velocity = Vector2f{ 0.0f, 0.0f });
	bool CollisionCheck(Rectf& hitbox, Vector2f& velocity, const bool ignorePlatforms = false);
	bool CollisionCheck(Rectf& hitbox, const bool ignorePlatforms = false);

	void Update(float elapsedSec);
	void DrawBackGround();
	void DrawForeground();
	void LoadLevel(std::string path);

	std::string GetCurrentArea() const;
private:
	TextureManager* m_TextureManagerPtr{};
	EnemyManager* m_EnemyManagerPtr{};
	SaveManager* m_SaveManagerPtr{};
	Camera* m_CameraPtr{};

	float m_AnimationDuration{0.0f};

	std::vector<Rectf> m_LevelGeometry;
	std::vector<Platform> m_LevelPlatforms;
	std::vector<Door> m_LevelDoors;
	std::vector<Rectf> m_LevelLadders;
	std::vector<Rectf> m_LevelSpikes;
	std::vector<Object> m_Items;
	std::vector<Rectf> m_HiddenAreas;
	std::vector<Shrine> m_RespawnShrines;
	std::vector<std::string> m_LevelBackground;
	std::string m_CurrentLevel;
};