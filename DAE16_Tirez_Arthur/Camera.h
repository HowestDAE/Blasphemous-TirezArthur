#pragma once
class Camera final
{
public:
	explicit Camera(float screenwidth, float screenheight);

	void Aim(float levelW, float levelH, Point2f center, float elapsedSec, bool instant = false);
	void ApplyS();
	void ApplyTS();
	void Reset();
private:
	float m_ScreenDisplaceX;
	float m_ScreenDisplaceY;
	Point2f m_AimPosition;
	const float DISPLAYSCALE{ 2.0f };
	const float AIMFACTOR{ 7.0f };
};

