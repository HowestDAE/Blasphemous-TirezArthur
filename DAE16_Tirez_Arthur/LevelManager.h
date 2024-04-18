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

enum class Interactions {
	ladder
};

class LevelManager final
{
public:
	LevelManager(TextureManager* textureManager);

	bool CollisionCheck(Rectf& hitbox, Vector2f& velocity) const;
	bool Interact(Interactions interaction, Rectf& hitbox) const;

	void DrawBackGround();
	void DrawForeground();
private:
	void LoadLevel(std::string path);

	TextureManager* m_TextureManagerPtr{};

	std::vector<Rectf> m_LevelGeometry;
	std::vector<Door> m_LevelDoors;
	std::vector<Rectf> m_LevelLadders;
	std::string m_CurrentLevel;
};