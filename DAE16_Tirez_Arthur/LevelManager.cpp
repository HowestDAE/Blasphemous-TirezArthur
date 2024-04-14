#include "pch.h"
#include "json/json.h"
#include "LevelManager.h"
#include "TextureManager.h"
#include "utils.h"
#include <fstream>
#include <iostream>

LevelManager::LevelManager(TextureManager* textureManager):
	m_TextureManagerPtr{textureManager}
{
	LoadLevel("indoor1");
}

LevelManager::~LevelManager()
{

}

bool LevelManager::CollisionCheck(Rectf& hitbox, Point2f& velocity) const
{
	bool collisionHappened{ false };
	for (Rectf collisionRect : m_LevelGeometry)
	{
		if (utils::IsOverlapping(hitbox, collisionRect))
		{
			float deltaX{};
			float deltaY{};
			collisionRect.left -= 0.1f; // Add margin
			collisionRect.bottom -= 0.1f;
			collisionRect.height += 0.2f;
			collisionRect.width += 0.2f;
			if (velocity.x < 0.0f)
			{
				deltaX = - collisionRect.left - collisionRect.width + hitbox.left;
			}
			else
			{
				deltaX = hitbox.left + hitbox.width - collisionRect.left;
			}
			if (velocity.y < 0.0f)
			{
				deltaY = - collisionRect.bottom - collisionRect.height + hitbox.bottom;
			}
			else
			{
				deltaY = hitbox.bottom + hitbox.height - collisionRect.bottom;
			}
			if (abs(deltaY) < abs(deltaX))
			{
				hitbox.bottom -= deltaY;
				velocity.y = 0.0f;
			}
			else
			{
				hitbox.left -= deltaX;
				velocity.x = 0.0f;
			}
			collisionHappened = true;
		}
	}
	return collisionHappened;
}

void LevelManager::DrawBackGround()
{
	m_TextureManagerPtr->Draw(m_CurrentLevel, 0.0f, 0.0f);
}

void LevelManager::DrawForeground()
{

}

void LevelManager::LoadLevel(std::string path)
{
	std::ifstream levelDataFile("LevelData/" + path + ".json", std::ifstream::binary);
	if (!levelDataFile)
	{
		std::cout << "Leveldata for level \"" + path + "\" does not exist.";
		return;
	}
	m_CurrentLevel = path;
	m_LevelGeometry.clear();

	Json::Value levelData;
	levelDataFile >> levelData;

	Json::Value::Members collisionBoxes{ levelData["collision"].getMemberNames() };

	for (const std::string& currentCollissionRect : collisionBoxes)
	{
		Rectf ColissionRect{	levelData["collision"][currentCollissionRect].get("left", -1).asFloat(),
								levelData["collision"][currentCollissionRect].get("bottom", -1).asFloat() ,
								levelData["collision"][currentCollissionRect].get("width", -1).asFloat() ,
								levelData["collision"][currentCollissionRect].get("height", -1).asFloat() };
		m_LevelGeometry.push_back(ColissionRect);
	}
}
