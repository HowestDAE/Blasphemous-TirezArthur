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

bool LevelManager::CollisionCheck(Rectf& hitbox, Vector2f& velocity) const
{
	bool collisionHappened{ false };
	if (hitbox.left < 0.0f && hitbox.bottom < 0.0f)
	{
		hitbox.left = m_LevelDoors.at(0).destinationPos.x;
		hitbox.bottom = m_LevelDoors.at(0).destinationPos.y;
		return collisionHappened;
	}
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
	for (const Door& door : m_LevelDoors)
	{
		if (utils::IsOverlapping(hitbox, door.hitbox) 
			&&
			(!door.flipped &&
			hitbox.left <= door.hitbox.left &&
			hitbox.left + hitbox.width >= door.hitbox.left
			||
			door.flipped &&
			hitbox.left <= door.hitbox.left + door.hitbox.width &&
			hitbox.left + hitbox.width >= door.hitbox.left + door.hitbox.width))
		{
			std::cout << "travel to " << door.destination << std::endl;
		}
	}
	return collisionHappened;
}

bool LevelManager::Interact(Interactions interaction, Rectf& hitbox) const
{
	switch (interaction)
	{
	case Interactions::ladder:
		for (const Rectf& ladder : m_LevelLadders)
		{
			if (utils::IsOverlapping(ladder, hitbox))
			{
				hitbox.left = ladder.left - hitbox.width * 0.5f;
				return true;
			}
		}
		return false;
		break;
	default:
		break;
	}
}

void LevelManager::DrawBackGround()
{
	m_TextureManagerPtr->Draw(m_CurrentLevel, 0.0f, 0.0f);
}

void LevelManager::DrawForeground()
{
	for (const Door& currentDoor : m_LevelDoors)
	{
		m_TextureManagerPtr->Draw(currentDoor.texture, currentDoor.hitbox.left, currentDoor.hitbox.bottom, currentDoor.flipped);
	}
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

	const Json::Value::Members& collisionBoxes{ levelData["collision"].getMemberNames() };

	for (const std::string& currentCollissionRect : collisionBoxes)
	{
		Rectf ColissionRect{	levelData["collision"][currentCollissionRect].get("left", -1).asFloat(),
								levelData["collision"][currentCollissionRect].get("bottom", -1).asFloat() ,
								levelData["collision"][currentCollissionRect].get("width", -1).asFloat() ,
								levelData["collision"][currentCollissionRect].get("height", -1).asFloat() };
		m_LevelGeometry.push_back(ColissionRect);
	}

	const Json::Value::Members& doors{ levelData["door"].getMemberNames() };

	for (const std::string& currentDoor : doors)
	{
		Rectf doorHitbox{			levelData["door"][currentDoor].get("left", -1).asFloat(),
									levelData["door"][currentDoor].get("bottom", -1).asFloat() ,
									levelData["door"][currentDoor].get("width", -1).asFloat() ,
									levelData["door"][currentDoor].get("height", -1).asFloat() };
		bool doorFlipped{			levelData["door"][currentDoor].get("flipped", false).asBool() };
		std::string texture{		levelData["door"][currentDoor].get("texture", "").asString() };
		std::string destination{	levelData["door"][currentDoor]["destination"].get("area", "").asString()};
		Point2f destinationPos{		levelData["door"][currentDoor]["destination"].get("left", "").asFloat(),
									levelData["door"][currentDoor]["destination"].get("bottom", "").asFloat() };
		m_LevelDoors.push_back(Door{ doorHitbox,doorFlipped, texture, destination, destinationPos });
	}

	const Json::Value::Members& ladders{ levelData["ladder"].getMemberNames() };

	for (const std::string& currentLadder : ladders)
	{
		Rectf ladderHitbox{	levelData["ladder"][currentLadder].get("left", -1).asFloat(),
							levelData["ladder"][currentLadder].get("bottom", -1).asFloat() ,
							levelData["ladder"][currentLadder].get("width", -1).asFloat() ,
							levelData["ladder"][currentLadder].get("height", -1).asFloat() };
		m_LevelLadders.push_back(ladderHitbox);
	}
}
