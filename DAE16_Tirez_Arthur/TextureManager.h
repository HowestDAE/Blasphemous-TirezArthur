#pragma once
#include <map>
#define JSON_IS_AMALGAMATION
#include "json/json-forwards.h"
class Texture;

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	void Draw(std::string path, float x, float y, bool flipped = false);
	void Draw(std::string path, Point2f pos, bool flipped = false);
	void Animate(std::string path, float x, float y, float animationDuration, bool flipped = false, bool loop = true);
	void Animate(std::string path, Point2f pos, float animationDuration, bool flipped = false, bool loop = true);

	float GetTextureWidth(std::string path) const;
	float GetTextureHeight(std::string path) const;
private:
	bool LoadTexture(std::string path);
	std::map<std::string, Texture*> m_TextureMap;
	std::map<std::string, Json::Value> m_AnimationMap;

	const int ANIMATIONFRAMERATE{ 24 };
};

