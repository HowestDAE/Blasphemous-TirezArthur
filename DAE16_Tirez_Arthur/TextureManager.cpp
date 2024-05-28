#include "pch.h"
#include "TextureManager.h"
#include "Texture.h"
#include "json/json.h"
#include <iostream>
#include <fstream>

TextureManager::TextureManager()
{

}

TextureManager::~TextureManager()
{
	for (const std::pair<std::string, Texture*>& texture : m_TextureMap)
	{
		delete texture.second;
	}
}

void TextureManager::Draw(const std::string& path, float x, float y, bool flipped)
{
	Draw(path, Point2f{ x, y }, flipped);
}

void TextureManager::Draw(const std::string& path, Point2f pos, bool flipped)
{
	if (m_TextureMap.find(path) != m_TextureMap.end())
	{
		if (m_TextureMap.at(path) != nullptr) {
			Texture* texture{ m_TextureMap.at(path) };
			texture->Draw( pos, Rectf{ 0.f, 0.f, texture->GetWidth(), texture->GetHeight() }, flipped);
		}
	}
	else
	{
		if (LoadTexture(path))
		{
			Draw(path, pos, flipped);
		}
	}
}

void TextureManager::Animate(const std::string& path, float x, float y, float animationDuration, bool flipped, bool loop, float frameTimeModifier)
{
	Animate(path, Point2f{ x,y }, animationDuration, flipped, loop, frameTimeModifier);
}

void TextureManager::Animate(const std::string& path, Point2f pos, float animationDuration, bool flipped, bool loop, float frameTimeModifier)
{
	while (animationDuration < 0.0f)
	{
		animationDuration += GetAnimationDuration(path) / frameTimeModifier;
	}
	animationDuration *= frameTimeModifier;
	if (m_TextureMap.find(path) != m_TextureMap.end())
	{
		if (m_TextureMap.at(path) != nullptr){
			Texture* texture{ m_TextureMap.at(path) };

			if (m_AnimationMap.find(path) != m_AnimationMap.end())
			{
				Json::Value& animationData{ m_AnimationMap.at(path) };
				const int animationFrames{ (int)animationData["frames"].size() };
				const float secondsPerFrame{ 1.0f / ANIMATIONFRAMERATE };
				int currentFrame{ (int)(animationDuration / secondsPerFrame) % animationFrames };
				if (!loop && secondsPerFrame * animationFrames <= animationDuration) currentFrame = animationFrames - 1;

				Json::Value currentFrameData{ animationData["frames"][std::to_string(currentFrame)] };
				Rectf sourceRect{ currentFrameData["frame"].get("x", -1.f).asFloat(),
									currentFrameData["frame"].get("y", -1.f).asFloat() + currentFrameData["frame"].get("h", -1.f).asFloat(),
									currentFrameData["frame"].get("w", -1.f).asFloat(),
									currentFrameData["frame"].get("h", -1.f).asFloat() };
				Point2f offset{ currentFrameData["offset"].get("left", 0.f).asFloat(),
									currentFrameData["offset"].get("bottom", 0.f).asFloat() };

				if (flipped) offset.x = currentFrameData["offset"].get("right", 0.f).asFloat();
				if (animationData["frames"][std::to_string(currentFrame)].get("flipped", false).asBool()) flipped = !flipped;

				texture->Draw(Rectf{ pos.x - offset.x,
										pos.y - offset.y,
										sourceRect.width,
										sourceRect.height },
					sourceRect,
					flipped);
			}
		}
		else
		{
			Draw(path, pos, flipped);
		}
	}
	else
	{
		if (LoadTexture(path))
		{
			Animate(path, pos, animationDuration, flipped);
		}
	}
}

void TextureManager::PreLoadTexture(const std::string& path)
{
	if (m_TextureMap.find(path) == m_TextureMap.end()) LoadTexture(path);
}

void TextureManager::MakeTextTexture(const std::string& Id, const std::string& font, const std::string& content, const Color4f& color, int size)
{
	if (m_TextureMap.find(Id) != m_TextureMap.end() && m_TextureMap.at(Id) != nullptr) delete m_TextureMap.at(Id);
	m_TextureMap[Id] = new Texture{ content, font, size, color};
}

float TextureManager::GetTextureWidth(const std::string& path) const
{
	if (m_TextureMap.find(path) == m_TextureMap.end() || m_TextureMap.at(path) == nullptr) return -1.0f;
	return m_TextureMap.at(path)->GetWidth();
}

float TextureManager::GetTextureHeight(const std::string& path) const
{
	if (m_TextureMap.find(path) == m_TextureMap.end() || m_TextureMap.at(path) == nullptr) return -1.0f;
	return m_TextureMap.at(path)->GetHeight();
}

float TextureManager::GetAnimationDuration(const std::string& path) const
{
	if (m_AnimationMap.find(path) == m_AnimationMap.end() || m_TextureMap.at(path) == nullptr) return -1.0f;
	return (float)m_AnimationMap.at(path)["frames"].size() / ANIMATIONFRAMERATE;
}

bool TextureManager::LoadTexture(std::string path)
{
	Texture* texture{ new Texture{"Textures/" + path + ".png"} };
	if (!texture->IsCreationOk())
	{
		delete texture;
		m_TextureMap[path] = nullptr;
		return false;
	}
	m_TextureMap[path] = texture;

	std::ifstream textureFile("Textures/" + path + ".json", std::ifstream::binary);
	if (textureFile)
	{
		Json::Value animationData;
		textureFile >> animationData;
		m_AnimationMap[path] = animationData;
	}
	return true;
}