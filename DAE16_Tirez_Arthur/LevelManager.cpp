#include "pch.h"
#include "json/json.h"
#include "LevelManager.h"
#include "TextureManager.h"
#include "EnemyManager.h"
#include "Camera.h"
#include "utils.h"
#include <fstream>
#include <iostream>

LevelManager::LevelManager(TextureManager* textureManager, EnemyManager* enemyManager, Camera* camera, SaveManager* saveManager):
	m_TextureManagerPtr{textureManager},
	m_EnemyManagerPtr{enemyManager},
	m_CameraPtr{camera},
	m_SaveManagerPtr{saveManager}
{
	
}

bool LevelManager::CollisionCheck(Rectf& hitbox, Vector2f& velocity, const bool ignorePlatforms)
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
	if (!ignorePlatforms){
		for (Platform platform : m_LevelPlatforms)
		{
			if (utils::IsOverlapping(hitbox, platform.hitbox))
			{
				float deltaX{};
				float deltaY{};
				platform.hitbox.bottom -= 0.1f; // Add margin
				platform.hitbox.height += 0.2f;
				if (velocity.y <= 0.0f && -platform.hitbox.bottom - platform.hitbox.height + hitbox.bottom > -5.0f)
				{
					hitbox.bottom = platform.hitbox.bottom + platform.hitbox.height;
					velocity.y = 0.0f;
					collisionHappened = true;
				}
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
			hitbox.bottom = door.destinationPos.y;
			hitbox.left = door.destinationPos.x;
			LoadLevel(door.destination);
			m_CameraPtr->Aim(utils::GetCenter(hitbox));
		}
	}

	for (int roomIndex{}; roomIndex < m_HiddenAreas.size(); ++roomIndex)
	{
		if (utils::IsOverlapping(hitbox, m_HiddenAreas.at(roomIndex))) {
			m_HiddenAreas.erase(m_HiddenAreas.begin() + roomIndex);
		}
	}

	return collisionHappened;
}

bool LevelManager::CollisionCheck(Rectf& hitbox, const bool ignorePlatforms)
{
	Vector2f velocity{-1.0f, -1.0f};
	return CollisionCheck(hitbox, velocity, ignorePlatforms);
}

bool LevelManager::Interact(Interactions interaction, Rectf& playerHitbox, const Vector2f& velocity)
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
	case Interactions::pickup:
		for (int itemIndex{}; itemIndex < m_Items.size(); ++itemIndex) {
			const Object& item{ m_Items.at(itemIndex) };
			if (utils::IsOverlapping(playerHitbox, Rectf{ item.pos.x, item.pos.y, 12.0f, 12.0f })) {
				m_SaveManagerPtr->CollectItem(item.item);
				playerHitbox.left = utils::GetCenter(Rectf{ item.pos.x, item.pos.y, 12.0f, 12.0f }).x - playerHitbox.width * 0.5f;
				m_Items.erase(m_Items.begin() + itemIndex);
				return true;
			}
		}
		return false;
		break;
	default:
		return false;
		break;
	}
}

void LevelManager::Update(float elapsedSec)
{
	m_AnimationDuration += elapsedSec;
}

void LevelManager::DrawBackGround()
{
	for (std::string& background : m_LevelBackground) {
		const float displayScale{ m_CameraPtr->getDisplayScale() };
		const float viewportSizeX{ m_CameraPtr->getViewportWidth() / displayScale };
		const float viewportSizeY{ m_CameraPtr->getViewportHeight() / displayScale };
		const float XtravelDistanceLevel{ (m_TextureManagerPtr->GetTextureWidth(m_CurrentLevel) - viewportSizeX) };
		const float YtravelDistanceLevel{ (m_TextureManagerPtr->GetTextureHeight(m_CurrentLevel) - viewportSizeY) };
		const float XtravelDistanceBackground{ (m_TextureManagerPtr->GetTextureWidth(background) - viewportSizeX) };
		const float YtravelDistanceBackground{ (m_TextureManagerPtr->GetTextureHeight(background) - viewportSizeY) };
		const float Xratio{ XtravelDistanceBackground / XtravelDistanceLevel  };
		const float Yratio{ YtravelDistanceBackground / YtravelDistanceLevel  };
		Point2f aimPos{ m_CameraPtr->getAimPos() };
		aimPos.x -= viewportSizeX * 0.5f;
		aimPos.y -= viewportSizeY * 0.5f;
		const float Xtravel{ aimPos.x * Xratio };
		const float Ytravel{ aimPos.y * Yratio };
		m_TextureManagerPtr->Draw(background, aimPos.x - Xtravel, aimPos.y - Ytravel);
	}
	m_TextureManagerPtr->Draw(m_CurrentLevel, 0.0f, 0.0f);

	for (const Object& currentItem : m_Items)
	{
		m_TextureManagerPtr->Animate("item", currentItem.pos.x, currentItem.pos.y, m_AnimationDuration);
	}
}

void LevelManager::DrawForeground()
{
	for (const Door& currentDoor : m_LevelDoors)
	{
		if (currentDoor.texture != "") m_TextureManagerPtr->Draw(currentDoor.texture, currentDoor.hitbox.left, currentDoor.hitbox.bottom, currentDoor.flipped);
	}

	for (const Rectf& currentRoom : m_HiddenAreas)
	{
		utils::SetColor(Color4f{ 0.118f, 0.102f, 0.11f, 1.0f });
		utils::FillRect(currentRoom);
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
	m_LevelLadders.clear();
	m_LevelPlatforms.clear();
	m_LevelSpikes.clear();
	m_LevelDoors.clear();
	m_EnemyManagerPtr->Clear();
	m_Items.clear();
	m_HiddenAreas.clear();
	m_LevelBackground.clear();

	m_TextureManagerPtr->PreLoadTexture(path);
	m_CameraPtr->SetLevelDimensions(m_TextureManagerPtr->GetTextureWidth(path), m_TextureManagerPtr->GetTextureHeight(path));

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
		const Rectf doorHitbox{			levelData["door"][currentDoor].get("left", -1).asFloat(),
										levelData["door"][currentDoor].get("bottom", -1).asFloat() ,
										levelData["door"][currentDoor].get("width", -1).asFloat() ,
										levelData["door"][currentDoor].get("height", -1).asFloat() };
		const bool doorFlipped{			levelData["door"][currentDoor].get("flipped", false).asBool() };
		const std::string texture{		levelData["door"][currentDoor].get("texture", "").asString() };
		const std::string destination{	levelData["door"][currentDoor]["destination"].get("area", "").asString()};
		const Point2f destinationPos{	levelData["door"][currentDoor]["destination"].get("left", "").asFloat(),
										levelData["door"][currentDoor]["destination"].get("bottom", "").asFloat() };
		m_LevelDoors.push_back(Door{ doorHitbox,doorFlipped, texture, destination, destinationPos });
	}

	const Json::Value::Members& ladders{ levelData["ladder"].getMemberNames() };
	m_LevelLadders.reserve(ladders.size());

	for (const std::string& currentLadder : ladders)
	{
		const Rectf ladderHitbox{	levelData["ladder"][currentLadder].get("left", -1).asFloat(),
									levelData["ladder"][currentLadder].get("bottom", -1).asFloat() ,
									levelData["ladder"][currentLadder].get("width", -1).asFloat() ,
									levelData["ladder"][currentLadder].get("height", -1).asFloat() };
		m_LevelLadders.push_back(ladderHitbox);
	}

	const Json::Value::Members& spikes{ levelData["spike"].getMemberNames() };
	m_LevelSpikes.reserve(spikes.size());

	for (const std::string& currentSpike : spikes)
	{
		const Rectf ladderHitbox{	levelData["spike"][currentSpike].get("left", -1).asFloat(),
									levelData["spike"][currentSpike].get("bottom", -1).asFloat(),
									levelData["spike"][currentSpike].get("width", -1).asFloat(),
									levelData["spike"][currentSpike].get("height", -1).asFloat() };
		m_LevelSpikes.push_back(ladderHitbox);
	}

	const Json::Value::Members& enemies{ levelData["enemies"].getMemberNames() };

	for (const std::string& currentEnemy : enemies)
	{
		EnemyManager::EnemyType enemyType{};
		const std::string type{ levelData["enemies"][currentEnemy].get("type", "").asString() };
		if (type == "cartwheel") enemyType = EnemyManager::EnemyType::cartwheel;
		else if (type == "shieldmaiden") enemyType = EnemyManager::EnemyType::shieldmaiden;
		else if (type == "stoner") enemyType = EnemyManager::EnemyType::stoner;
		else if (type == "crucified") enemyType = EnemyManager::EnemyType::crucified;
		const float xPos{ levelData["enemies"][currentEnemy].get("x", -1).asFloat() };
		const float yPos{ levelData["enemies"][currentEnemy].get("y", -1).asFloat() };
		m_EnemyManagerPtr->SpawnEnemy(enemyType, xPos, yPos);
	}

	const Json::Value::Members& items{ levelData["items"].getMemberNames() };

	for (const std::string& currentItem : items)
	{
		const Point2f itemPos{	levelData["items"][currentItem].get("x", -1).asFloat(),
								levelData["items"][currentItem].get("y", -1).asFloat() };
		const CategoryId category{ (CategoryId)levelData["items"][currentItem].get("category", 0).asInt() };
		const ItemId id{ (ItemId)levelData["items"][currentItem].get("id", 0).asInt() };
		const std::string name{ levelData["items"][currentItem].get("name", "").asString() };
		const std::string description{ levelData["items"][currentItem].get("description", "").asString() };
		if (!m_SaveManagerPtr->IsItemCollected(category, id)) m_Items.push_back(Object{ itemPos, Item{category, id, name, description} });
	}

	const Json::Value::Members& hiddenRooms{ levelData["hiddenRooms"].getMemberNames() };

	for (const std::string& currentRoom : hiddenRooms)
	{
		const Rectf hitbox{ levelData["hiddenRooms"][currentRoom].get("x", -1).asFloat(),
							levelData["hiddenRooms"][currentRoom].get("y", -1).asFloat(),
							levelData["hiddenRooms"][currentRoom].get("w", -1).asFloat(),
							levelData["hiddenRooms"][currentRoom].get("h", -1).asFloat() };
		m_HiddenAreas.push_back(hitbox);
	}

	for (int backgroundIndex{}; backgroundIndex < levelData["background"].size(); ++backgroundIndex) {
		m_LevelBackground.push_back(levelData["background"][backgroundIndex].asString());
		m_TextureManagerPtr->PreLoadTexture(m_LevelBackground.at(backgroundIndex));
	}
	std::reverse(m_LevelBackground.begin(), m_LevelBackground.end());
}
