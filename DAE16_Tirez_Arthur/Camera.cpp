#include "pch.h"
#include "Camera.h"

Camera::Camera(float screenwidth, float screenheight):
	m_ScreenDisplaceX{screenwidth * 0.5f},
	m_ScreenDisplaceY{screenheight * 0.5f}
{
	
}

void Camera::Aim(float levelW, float levelH, Point2f center, float elapsedSec, bool instant)
{
	if (instant) {
		m_AimPosition = center;
	}
	else {
		float horizontalDistance{ center.x - m_AimPosition.x };
		float verticalDistance{ center.y - m_AimPosition.y };
		m_AimPosition.y += verticalDistance * AIMFACTOR * elapsedSec;
		m_AimPosition.x += horizontalDistance * AIMFACTOR * elapsedSec;
	}
	m_AimPosition.x = std::max(m_AimPosition.x, (m_ScreenDisplaceX / DISPLAYSCALE));
	m_AimPosition.y = std::max(m_AimPosition.y, (m_ScreenDisplaceY / DISPLAYSCALE));
	m_AimPosition.x = std::min(m_AimPosition.x, levelW - (m_ScreenDisplaceX / DISPLAYSCALE));
	m_AimPosition.y = std::min(m_AimPosition.y, levelH - (m_ScreenDisplaceY / DISPLAYSCALE));
}

void Camera::ApplyS()
{
	glPushMatrix();
	glScalef(DISPLAYSCALE, DISPLAYSCALE, 1.0f);
}

void Camera::ApplyTS()
{
	glPushMatrix();
	glTranslatef(m_ScreenDisplaceX, m_ScreenDisplaceY, 0.0f);
	glScalef(DISPLAYSCALE, DISPLAYSCALE, 1.0f);
	glTranslatef(-m_AimPosition.x, -m_AimPosition.y, 0.0f);
}

void Camera::Reset()
{
	glPopMatrix();
}
