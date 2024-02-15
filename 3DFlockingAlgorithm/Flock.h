#pragma once
#include "Boid.h"

using namespace Play3d;

enum FlockAttributes
{
	COHESION = 0,
	ALIGNMENT,
	SEPARATION
};

class Flock
{
public:
	Flock() 
	{
		m_meshId = Graphics::CreateMeshFromObjFile("../Assets/Models/BoidMesh.obj");
		
	
		//Create a wireframe material
		
		Graphics::SimpleMaterialDesc desc;
		desc.m_state.m_cullMode = Graphics::CullMode::NONE;
		desc.m_state.m_fillMode = Graphics::FillMode::WIREFRAME;
		Colour::Gray.as_float_rgba_srgb(&desc.m_constants.diffuseColour.x);
		m_wireFrameMaterialId = Resources::CreateAsset<Graphics::Material>(desc);

		for (int i = 0; i < 100; i++)
		{
			Boid boid(Vector3f(0.0f, 0.0f, 0.0f));
			AddBoid(boid);
		}

	}
	//Adds a boid to the flock
	void AddBoid(Boid boid);
	void UpdateFlock();
	void RenderFlock(int boidIndex);

	//Math functions
	void Normalise(Vector3f& vector);
	float Magnitude(Vector3f& vector);

	//Samples a feature (e.g. velocity) of all boids that surround a particular feature
	Vector3f SampleSurroundingBoids(FlockAttributes attribute, int boidIndex);

	//Find the average of a feature of group of boids e.g. velocity, control how quickly the boids flock together, and set speed of flock
	Vector3f SetFlockSpecifications(Vector3f sum, float numberOfBoidsSampled, FlockAttributes attribute, int boidIndex);

	Vector3f Alignment(int boidIndex);
	Vector3f Cohesion(int boidIndex);
	Vector3f Separation(int boidIndex);

private:
	Graphics::MeshId m_meshId;
	Graphics::MaterialId m_solidMaterialId;
	Graphics::MaterialId m_wireFrameMaterialId;
	Graphics::MaterialId m_boidMaterialId;

	std::vector<Boid>m_vFlock;

	float m_flockSpeed{ 0.25f };

	//m_maxForce represents how strongly/quickly the boids will flock together
	float m_maxFlockingForce{ 10.0f };
};