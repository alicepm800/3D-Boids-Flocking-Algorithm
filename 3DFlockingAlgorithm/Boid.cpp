#include "Boid.h"

#define PI 3.141592635

void Boid::UpdateBoid()
{
	BoidEdgeDetection();
	RotateBoid();
	m_pos += m_velocity;

	//targetVelocity is the velocity that the boid steers towards
	Vector3f targetVelocity = Vector3f(0.0f, 0.0f, 0.0f);
	targetVelocity = m_velocity + m_acceleration;

	//Use linear interpolation to reduce jitteriness of movement
	m_velocity = Lerp(m_velocity, targetVelocity, 0.05f);

	//Reset acceleration on every frame
	m_acceleration = Vector3f(0.0f, 0.0f, 0.0f);
}

void Boid::RotateBoid()
{
	float targetYaw = 0.0f;
	float targetPitch = 0.0f;

	float yawAngleDifference = 0.0f;
	float pitchAngleDifference = 0.0f;

	float yawRotationSpeed = 0.0f;
	float pitchRotationSpeed = 0.0f;

	//rotation around y axis
	targetYaw = std::atan2(m_velocity.x, m_velocity.z);
	float yawDifference = targetYaw - m_yaw;
	yawRotationSpeed = FindRotationSpeed(yawDifference);
	m_yaw = Lerp(m_yaw, targetYaw, yawRotationSpeed);

	//rotation and x axis
	//PI/2 rotates sprite as if it is facing down the z axis instead of up the y axis if rotation is 0 - basically reorients sprite to intended starting rotation
	targetPitch = std::atan2(-m_velocity.y, std::sqrt(m_velocity.x * m_velocity.x + m_velocity.z * m_velocity.z)) + PI/2 ;
	float pitchDifference = targetPitch - m_pitch;
	pitchRotationSpeed = FindRotationSpeed(pitchDifference);
	m_pitch = Lerp(m_pitch, targetPitch, pitchRotationSpeed);

	//Rotation around z axis
	m_roll = 0;
	
}

float Boid::FindRotationSpeed(float angleDifference)
{
	while (angleDifference > PI)
	{
		angleDifference -= 2 * PI;
	}
	while (angleDifference < -PI)
	{
		angleDifference += 2 * PI;
	}

	// Calculate interpolation factor based on the magnitude of angle difference
	float rotationSpeed = 0.1f;
	if (std::abs(angleDifference) > PI / 2)
	{
		//If the angle difference is larger than PI/2, reduce the interpolation factor
		rotationSpeed *= 0.8f;
	}
	return rotationSpeed;
}
void Boid::BoidEdgeDetection()
{
	//If boid is at the edge of the box it should stay in, make it go to the other side of the box
	if (m_pos.x > m_displaySize.x)
	{
		m_pos.x = -m_displaySize.x;
	}
	else if (m_pos.x < -m_displaySize.x)
	{
		m_pos.x = m_displaySize.x;
	}

	if (m_pos.y > m_displaySize.y)
	{
		m_pos.y = 0.0f;
	}
	else if (m_pos.y < 0.0f)
	{
		m_pos.y = m_displaySize.y;
	}

	if (m_pos.z > m_displaySize.z)
	{
		m_pos.z = -m_displaySize.z;
	}
	if (m_pos.z < -m_displaySize.z)
	{
		m_pos.z = m_displaySize.z;
	}



}

//Linear interpolation functions to reduce jitteriness of movement
//The lower lerpStrength is, the smoother the movement will be
float Boid::Lerp(float currentValue, float targetValue, float lerpStrength)
{
	return currentValue + lerpStrength * (targetValue - currentValue);
}

Vector3f Boid::Lerp(Vector3f currentValue, Vector3f targetValue, float lerpStrength)
{
	return Vector3f(Lerp(currentValue.x, targetValue.x, lerpStrength), Lerp(currentValue.y, targetValue.y, lerpStrength), Lerp(currentValue.z, targetValue.z, lerpStrength));
}
