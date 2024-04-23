#pragma once
#include <map>
#define JSON_IS_AMALGAMATION
#include "json/json-forwards.h"
class Texture;

class TextureManager final
{
public:
	TextureManager();
	TextureManager(const TextureManager& textureManager) = delete;
	~TextureManager();

	TextureManager& operator=(const TextureManager& textureManager) = delete;

	void Draw(std::string path, float x = 0.0f, float y = 0.0f, bool flipped = false);
	void Draw(std::string path, Point2f pos, bool flipped = false);
	void Animate(std::string path, float x = 0.0, float y = 0.0f, float animationDuration = 0.0f, bool flipped = false, bool loop = true, float frameTimeModifier = 1.0f);
	void Animate(std::string path, Point2f pos, float animationDuration, bool flipped = false, bool loop = true, float frameTimeModifier = 1.0f);

	float GetTextureWidth(std::string path) const;
	float GetTextureHeight(std::string path) const;
	float GetAnimationDuration(std::string path) const;
private:
	bool LoadTexture(std::string path);

	std::map<std::string, Texture*> m_TextureMap;
	std::map<std::string, Json::Value> m_AnimationMap;

	const int ANIMATIONFRAMERATE{ 24 };
};

