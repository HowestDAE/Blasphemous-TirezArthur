#pragma once
class Camera final
{
public:
	explicit Camera(float screenwidth, float screenheight);

	void Aim(float levelW, float levelH, Point2f center);
	void ApplyScale();
	void Reset();
private:
	float m_Screenwidth;
	float m_Screenheight;
	const float DISPLAYSCALE{ 2.0f };
};

