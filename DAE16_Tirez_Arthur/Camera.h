#pragma once
class Camera final
{
public:
	explicit Camera(float screenwidth, float screenheight);

	void ApplyS() const;
	void ApplyTS() const;
	void Reset();

	void Aim(Point2f center, float elapsedSec = -1.0f);
	void SetLevelDimensions(float width, float height);

	float getViewportWidth() const;
	float getViewportHeight() const;
	float getDisplayScale() const;
	Point2f getAimPos() const;
private:
	float m_ScreenDisplaceX;
	float m_ScreenDisplaceY;
	float m_levelW;
	float m_levelH;
	Point2f m_AimPosition;
	const float DISPLAYSCALE{ 2.0f };
	const float AIMFACTOR{ 7.0f };
};

