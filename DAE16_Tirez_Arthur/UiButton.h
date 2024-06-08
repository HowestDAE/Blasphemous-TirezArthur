#pragma once
#include "UiElement.h"

class InputManager;
class UiManager;
class SoundManager;

class UiButton final : public UiElement
{
public:
	explicit UiButton(InputManager* inputManager, UiManager* uiManager, SoundManager* soundManager, int functionId, const std::string& functionData);

	virtual void Update(float elapsedSec, bool selected = true) override;
	virtual void Draw(bool selected = false) const override;
private:
	InputManager* m_InputManagerPtr;
	UiManager* m_UiManagerPtr;
	SoundManager* m_SoundManagerPtr;
	const int m_FunctionId;
	const std::string m_FunctionData;
};

