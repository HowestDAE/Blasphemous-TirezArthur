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
	if (hitbox.left < 0.0f && hitbox.bottom < 0.0f && m_LevelDoors.size() > 0)
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
	for (Platform platform : m_LevelPlatforms)
	{
		if (utils::IsOverlapping(hitbox, platform.hitbox))
		{
			float deltaX{};
			float deltaY{};
			platform.hitbox.bottom -= 0.1f; // Add margin
			platform.hitbox.height += 0.2f;
			if (velocity.y <= 0.0f && - platform.hitbox.bottom - platform.hitbox.height + hitbox.bottom > -5.0f)
			{
				hitbox.bottom = platform.hitbox.bottom + platform.hitbox.height;
				velocity.y = 0.0f;
				collisionHappened = true;
			}
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

bool LevelManager::Interact(Interactions interaction, Rectf& playerHitbox, const Vector2f& velocity) const
{
	switch (interaction)
	{
	case Interactions::ladder:
		for (const Rectf& ladder : m_LevelLadders)
		{
			if (utils::IsOverlapping(ladder, playerHitbox))
			{
				playerHitbox.left = ladder.left - playerHitbox.width * 0.5f;
				return true;
			}
		}
		return false;
		break;
	case Interactions::ledge:
		for (const Platform& ledge : m_LevelPlatforms)
		{
			float deltaY{ abs(ledge.hitbox.bottom + ledge.hitbox.height - playerHitbox.bottom - playerHitbox.height * 0.5f) };
			if (velocity.x < 0.0f && ledge.rightGrabbable)
			{
				float deltaX{ playerHitbox.left - ledge.hitbox.left - ledge.hitbox.width };
				if (abs(deltaX) < 15.0f && deltaY < 20.0f)
				{
					playerHitbox.bottom = ledge.hitbox.bottom + ledge.hitbox.height - playerHitbox.height;
					playerHitbox.left = ledge.hitbox.left + ledge.hitbox.width;
					return true;
				}
			}
			else if (velocity.x > 0.0f && ledge.leftGrabbable)
			{
				float deltaX{ ledge.hitbox.left - playerHitbox.left - playerHitbox.width };
				if (abs(deltaX) < 15.0f && deltaY < 20.0f)
				{
					playerHitbox.bottom = ledge.hitbox.bottom + ledge.hitbox.height - playerHitbox.height;
					playerHitbox.left = ledge.hitbox.left - playerHitbox.width;
					return true;
				}
			}
		}
		return false;
		break;
	case Interactions::spike:
		for (const Rectf& spike : m_LevelSpikes)
		{
			if (utils::IsOverlapping(spike, playerHitbox))
			{
				playerHitbox.bottom = spike.bottom + 0.1f;
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
	//for (const Rectf& currentPlatform : m_LevelLadders)
	//{
	//	utils::DrawRect(currentPlatform);
	//}
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
	m_LevelLadders.clear();
	m_LevelPlatforms.clear();
	m_LevelSpikes.clear();
	m_LevelDoors.clear();

	Json::Value levelData;
	levelDataFile >> levelData;

	const Json::Value::Members& collisionBoxes{ levelData["collision"].getMemberNames() };
	m_LevelGeometry.reserve(collisionBoxes.size());

	for (const std::string& currentCollissionRect : collisionBoxes)
	{
		Rectf ColissionRect{	levelData["collision"][currentCollissionRect].get("left", -1).asFloat(),
								levelData["collision"][currentCollissionRect].get("bottom", -1).asFloat() ,
								levelData["collision"][currentCollissionRect].get("width", -1).asFloat() ,
								levelData["collision"][currentCollissionRect].get("height", -1).asFloat() };
		m_LevelGeometry.push_back(ColissionRect);
	}

	const Json::Value::Members& platformBoxes{ levelData["platform"].getMemberNames() };
	m_LevelPlatforms.reserve(platformBoxes.size());

	for (const std::string& currentPlatform : platformBoxes)
	{
		Rectf PlatformRect{ levelData["platform"][currentPlatform].get("left", -1).asFloat(),
							levelData["platform"][currentPlatform].get("bottom", -1).asFloat() ,
							levelData["platform"][currentPlatform].get("width", -1).asFloat() ,
							levelData["platform"][currentPlatform].get("height", -1).asFloat() };
		bool rightGrab{		levelData["platform"][currentPlatform].get("grabRight", false).asBool() };
		bool leftGrab{		levelData["platform"][currentPlatform].get("grabLeft", false).asBool() };
		m_LevelPlatforms.push_back(Platform{ PlatformRect, rightGrab, leftGrab });
	}

	const Json::Value::Members& doors{ levelData["door"].getMemberNames() };
	m_LevelDoors.reserve(doors.size());

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
	m_LevelLadders.reserve(ladders.size());

	for (const std::string& currentLadder : ladders)
	{
		Rectf ladderHitbox{	levelData["ladder"][currentLadder].get("left", -1).asFloat(),
							levelData["ladder"][currentLadder].get("bottom", -1).asFloat() ,
							levelData["ladder"][currentLadder].get("width", -1).asFloat() ,
							levelData["ladder"][currentLadder].get("height", -1).asFloat() };
		m_LevelLadders.push_back(ladderHitbox);
	}

	const Json::Value::Members& spikes{ levelData["spike"].getMemberNames() };
	m_LevelSpikes.reserve(spikes.size());

	for (const std::string& currentSpike : spikes)
	{
		Rectf ladderHitbox{ levelData["spike"][currentSpike].get("left", -1).asFloat(),
							levelData["spike"][currentSpike].get("bottom", -1).asFloat(),
							levelData["spike"][currentSpike].get("width", -1).asFloat(),
							levelData["spike"][currentSpike].get("height", -1).asFloat() };
		m_LevelSpikes.push_back(ladderHitbox);
	}
}
