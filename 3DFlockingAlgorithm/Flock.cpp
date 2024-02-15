#include "Flock.h"

void Flock::AddBoid(Boid boid)
{
	m_vFlock.push_back(boid);
}

void Flock::UpdateFlock()
{
	Vector3f boidAlignment = Vector3f(0.0f, 0.0f, 0.0f);
	Vector3f boidCohesion = Vector3f(0.0f, 0.0f, 0.0f);
	Vector3f boidSeparation = Vector3f(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < m_vFlock.size(); i++)
	{
		//Steer the boid towards the average velocity of its neighboring boids
		boidAlignment = Alignment(i);
		m_vFlock[i].m_acceleration += boidAlignment;

		//Steer the boids towards the average position of is neighboring boids
		boidCohesion = Cohesion(i);
		float cohesionWeakener = 1.5f;
		m_vFlock[i].m_acceleration += boidCohesion;

		//The closer two boids are, increase separation forces between them
		boidSeparation = Separation(i);
		m_vFlock[i].m_acceleration += boidSeparation;

		m_vFlock[i].UpdateBoid();
		RenderFlock(i);
	}
}

void Flock::RenderFlock(int boidIndex)
{
	Graphics::SetMaterial(m_wireFrameMaterialId);
	Matrix4x4f transform;
	Matrix4x4f rotationMatrix = MatrixRotationY(m_vFlock[boidIndex].m_yaw) * MatrixRotationX(m_vFlock[boidIndex].m_pitch) * MatrixRotationZ(m_vFlock[boidIndex].m_roll);
	transform = MatrixTranslate<f32>(m_vFlock[boidIndex].m_pos) * MatrixScale<f32>(0.1f, 0.1f, 0.1f) * rotationMatrix;
	Graphics::DrawMesh(m_meshId, transform);	
}

Vector3f Flock::Alignment(int boidIndex)
{
	//averageVelocity represents the velocity to steer this boid towards
	Vector3f averageVelocity = Vector3f(0.0f, 0.0f, 0.0f);

	//Find the average velocity of neigboring boids
	averageVelocity = SampleSurroundingBoids(ALIGNMENT, boidIndex);

	return averageVelocity;
}

Vector3f Flock::Cohesion(int boidIndex)
{
	//averagePosition represents the position to steer this boid towards
	Vector3f averagePosition = Vector3f(0.0f, 0.0f, 0.0f);

	//Find the average position of neighoring boids
	averagePosition = SampleSurroundingBoids(COHESION, boidIndex);
	
	return averagePosition;
}

Vector3f Flock::Separation(int boidIndex)
{
	Vector3f average = Vector3f(0.0f, 0.0f, 0.0f);
	Vector3f sum = Vector3f(0.0f, 0.0f, 0.0f);
	int totalBoidsSampled = 0;
	float desiredSeparation = 1.5f;

	for (int i = 0; i < m_vFlock.size(); i++)
	{
		//Go through the whole flock exception from the current boid
		if (boidIndex == i)
		{
			continue;
		}

		//Find the distance between this boid and the rest of the boids in the flock
		float distance = sqrt(pow(m_vFlock[boidIndex].m_pos.x - m_vFlock[i].m_pos.x, 2) + pow(m_vFlock[boidIndex].m_pos.y - m_vFlock[i].m_pos.y, 2) + pow(m_vFlock[boidIndex].m_pos.z - m_vFlock[i].m_pos.z, 2));

		//If distance of other boid is close enough to current boid, find out how much you need to separate the two boids
		if ((distance > 0) && (distance < desiredSeparation))
		{
			Vector3f positionDifference = Vector3f(0.0f, 0.0f, 0.0f);

			//Find the difference between the positions of the current boid and another boid
			positionDifference = m_vFlock[boidIndex].m_pos - m_vFlock[i].m_pos;

			Normalise(positionDifference);

			//Make separation force stronger when distance between boids is smaller
			positionDifference.x /= pow(distance, 2);
			positionDifference.y /= pow(distance, 2);
			positionDifference.z /= pow(distance, 2);

			//Sum up the separation forces required for neighboring boids
			sum += positionDifference;

			totalBoidsSampled++;
		}
	}

	average = SetFlockSpecifications(sum, totalBoidsSampled, SEPARATION, boidIndex);
	return Vector3f(average);
}

Vector3f Flock::SampleSurroundingBoids(FlockAttributes attribute, int boidIndex)
{
	Vector3f average = Vector3f(0.0f, 0.0f, 0.0f);
	Vector3f sum = Vector3f(0.0f, 0.0f, 0.0f);
	int totalBoidsSampled = 0;
	
	//Go through the entire flock to:
	//1) Find other boid's distances from the current boid
	//2) If other boid's distance is within the current boid's perception, their attribute is sampled to find the average for that attribute e.g. velocity
	for (int i = 0; i < m_vFlock.size(); i++)
	{
		//Go through the whole flock except from the current boid
		if (boidIndex == i)
		{
			continue;
		}
		//Find the distance between this boid and the rest of the boids in the flock
		float distance = sqrt(pow(m_vFlock[boidIndex].m_pos.x - m_vFlock[i].m_pos.x, 2) + pow(m_vFlock[boidIndex].m_pos.y - m_vFlock[i].m_pos.y, 2) + pow(m_vFlock[boidIndex].m_pos.z - m_vFlock[i].m_pos.z, 2));

		if ((distance > 0) && (distance < 5.0f))
		{
			Vector3f otherBoidsAttribute;
			switch (attribute)
			{
			case(ALIGNMENT):
				otherBoidsAttribute = m_vFlock[i].m_velocity;
				break;

			case(COHESION):
				otherBoidsAttribute = m_vFlock[i].m_pos;
				break;
			}
			sum += otherBoidsAttribute;
			totalBoidsSampled++;
		}
	}
	average = SetFlockSpecifications(sum, totalBoidsSampled, attribute, boidIndex);
	return Vector3f(average);
}

Vector3f Flock::SetFlockSpecifications(Vector3f sum, float numberOfBoidsSampled, FlockAttributes attribute, int boidIndex)
{
	Vector3f average = Vector3f(0.0f, 0.0f, 0.0f);
	
	if (numberOfBoidsSampled > 0)
	{
		average = sum / numberOfBoidsSampled;

		if (attribute == COHESION)
		{
			average -= m_vFlock[boidIndex].m_pos;
		}

		//Normalise the vector so each boid can be set to the same speed (so each vector has a unit of 1 length)
		Normalise(average);

		//Set speed of flock
		average.x *= m_flockSpeed;
		average.y *= m_flockSpeed;
		average.z *= m_flockSpeed;

		average -= m_vFlock[boidIndex].m_velocity;

		float magnitude = Magnitude(average);
		//Control how quickly the boids flock together
		if (magnitude > m_maxFlockingForce)
		{
			average = Vector3f(average.x / magnitude, average.y / magnitude, average.z / magnitude);
		}

	}
	return Vector3f(average);
}

void Flock::Normalise(Vector3f &vector)
{
	//Find length of the vector
	float magnitude = Magnitude(vector);
	//Find the unit vector by dividing the vector by its length
	vector.x /= magnitude;
	vector.y /= magnitude;
	vector.z /= magnitude;
}

float Flock::Magnitude(Vector3f& vector)
{
	return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}