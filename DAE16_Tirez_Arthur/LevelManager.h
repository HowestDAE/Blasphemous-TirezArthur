#pragma once
#include <vector>
#define JSON_IS_AMALGAMATION
#include "json/json-forwards.h"
#include "Vector2f.h"
class TextureManager;

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

enum class Interactions {
	ladder,
	ledge
};

class LevelManager final
{
public:
	LevelManager(TextureManager* textureManager);

	bool CollisionCheck(Rectf& hitbox, Vector2f& velocity) const;
	bool Interact(Interactions interaction, Rectf& hitbox, const Vector2f& velocity = Vector2f{ 0.0f, 0.0f }) const;

	void DrawBackGround();
	void DrawForeground();
private:
	void LoadLevel(std::string path);

	TextureManager* m_TextureManagerPtr{};

	std::vector<Rectf> m_LevelGeometry;
	std::vector<Platform> m_LevelPlatforms;
	std::vector<Door> m_LevelDoors;
	std::vector<Rectf> m_LevelLadders;
	std::string m_CurrentLevel;
};