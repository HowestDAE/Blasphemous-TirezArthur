#pragma once
#include <vector>
#define JSON_IS_AMALGAMATION
#include "json/json-forwards.h"
class TextureManager;

class LevelManager
{
public:
	LevelManager(TextureManager* textureManager);
	~LevelManager();

	bool CollisionCheck(Rectf& hitbox, Point2f& velocity) const;

	void DrawBackGround();
	void DrawForeground();
private:
	void LoadLevel(std::string path);

	TextureManager* m_TextureManagerPtr{};

	std::vector<Rectf> m_LevelGeometry;
	std::string m_CurrentLevel;
};