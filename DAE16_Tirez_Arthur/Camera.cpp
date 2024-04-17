#include "pch.h"
#include "Camera.h"

Camera::Camera(float screenwidth, float screenheight):
	m_Screenwidth{screenwidth},
	m_Screenheight{screenheight}
{
	
}

void Camera::Aim(float levelW, float levelH, Point2f center)
{
	levelW *= DISPLAYSCALE;
	levelH *= DISPLAYSCALE;
	center.x *= DISPLAYSCALE;
	center.y *= DISPLAYSCALE;

	center.x -= m_Screenwidth / 2;
	center.y -= m_Screenheight / 2;
	glPushMatrix();
	center.x = std::max(center.x, 0.0f);
	center.y = std::max(center.y, 0.0f);
	
	center.x = std::min(center.x, levelW - m_Screenwidth);
	center.y = std::min(center.y, levelH - m_Screenheight);

	center.x *= -1;
	center.y *= -1;

	glScalef(DISPLAYSCALE, DISPLAYSCALE, 1.0f);
	glTranslatef(center.x / DISPLAYSCALE, center.y / DISPLAYSCALE, 0.0f);
}

void Camera::Reset()
{
	glPopMatrix();
}
