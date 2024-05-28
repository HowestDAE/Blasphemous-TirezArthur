#pragma once
#include <vector>

class TextureManager;
class UiElement;
class Player;

class UiManager final
{
public:
	explicit UiManager(TextureManager* textureManager, Player* player);
	~UiManager();

	void Update(float elapsedSec);
	void Draw() const;
private:
	std::vector<UiElement*> m_ScreenVector;
	TextureManager* m_TextureManager;
	Player* m_PlayerPtr;
};

