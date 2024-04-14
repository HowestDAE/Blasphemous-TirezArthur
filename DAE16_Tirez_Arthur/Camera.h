#pragma once
class Camera
{
public:
	Camera(float screenwidth, float screenheight);
	~Camera();

	void Aim(float levelW, float levelH, Point2f center);
	void Reset();
private:
	float m_Screenwidth;
	float m_Screenheight;
	const float DISPLAYSCALE{ 2.0f };
};

