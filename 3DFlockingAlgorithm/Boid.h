#pragma once
#include "Play3d.h"
#include <random>


using namespace Play3d;

class Boid
{
public:
	Boid(Play3d::Vector3f position) : m_pos(position)
	{
		//=======================================================================
		//Set dimensions for the area that the boids will move in
		Play3d::Graphics::SurfaceSize displayScreenSize = Graphics::GetDisplaySurfaceSize();

		//Set the dimensions of the box that the boids will move inside of
		//The box is 20 x 10 x 20
		//Ranges from -10 to 10 for x and z. Ranges from 0 to 10 for y
		m_displaySize.x = (f32)displayScreenSize.m_width / (displayScreenSize.m_width / 10);
		m_displaySize.y = (f32)displayScreenSize.m_height / (displayScreenSize.m_height / 10);
		m_displaySize.z = m_displaySize.x;

		//=======================================================================
		//Generate random values
		std::random_device rd;
		std::mt19937 gen(rd());
		//Generate random velocity
		std::uniform_real_distribution<float>randomVelX(-0.15f, 0.15f);
		std::uniform_real_distribution<float>randomVelY(-0.15f, 0.15f);
		std::uniform_real_distribution<float>randomVelZ(-0.15f, 0.15f);
		//Generate random position
		std::uniform_real_distribution<float>randomXPos(-10.0f, 10.0f);
		std::uniform_real_distribution<float>randomYPos(0.0f, 10.0f);
		std::uniform_real_distribution<float>randomZPos(-10.0f, 10.0f);

		//=======================================================================
		//Set position and velocity for this boid
		m_velocity = Vector3f(randomVelX(gen), randomVelY(gen), randomVelZ(gen));
		
		m_pos = Vector3f(randomXPos(gen), randomYPos(gen), randomZPos(gen));
	
		m_worldMatrix = MatrixTranslate<float>(m_pos.x, m_pos.y, m_pos.z);	
	}
	
	void UpdateBoid();
	void RotateBoid();
	float FindRotationSpeed(float angleDiffernce);
	void BoidEdgeDetection();
	

	//Math functions
	float Lerp(float currentValue, float targetValue, float lerpStrength);
	Vector3f Lerp(Vector3f currentValue, Vector3f targetValue, float lerpStrength);

	Matrix4x4f m_worldMatrix = MatrixTranslate<f32>(0, 0, 0);
	Vector3f m_pos{ 0, 0, 0 };
	Vector3f m_velocity{ 0, 0, 0 };
	Vector3f m_acceleration{ 0, 0, 0 };
	Vector3f m_displaySize{ 0,0,0 };
	float m_rotation{ 0 };

	//Rotation member variables
	float m_yaw{ 0 };
	float m_pitch{ 0 };
	float m_roll{ 0 };

private:

};