///////////////////////////////////////////////////////////////////////////
//	Copyright (C) Sumo Digital Ltd. All rights reserved.
///////////////////////////////////////////////////////////////////////////

// We have to define this in one .cpp only to bring in our implementation code.
// It must be defined just before the #include "Play3d.h"
#define PLAY_IMPLEMENTATION
#include "Play3d.h"
#include "Flock.h"

using namespace Play3d;
void SetupGameGraphics();


int PlayMain()
{
	// If we need to assert and/or log our code there some handy macros.
	// PLAY_ASSERT()
	// PLAY_ASSERT_MSG(false, "errcode = 0x%x", 1234);
	// PLAY_TRACE("Hello Play3d");

	// First we initialise the Play3d library.
	SystemDesc systemDesc;
	systemDesc.width = 1920;
	systemDesc.height = 1024;
	System::Initialise(systemDesc);

	//Setup the lighting and camera
	SetupGameGraphics();

	//Create a flock of boids
	Flock flock;

	//////////////////////////////////////
	// Main Game Loop
	//////////////////////////////////////
	bool bKeepGoing = true;
	while (bKeepGoing)
	{
		// BeginFrame should be called first, it will return RESULT_QUIT if the user has quit via 'Close' icons.
		if (System::BeginFrame() != RESULT_OK)
		{
			bKeepGoing = false;
		}

		// We want to quit on the next frame if the user presses ESC.
		if (Input::IsKeyPressed(VK_ESCAPE))
		{
			bKeepGoing = false;
		}

		// You can use 'WASD' 'Q' and 'E' while dragging with the right mouse button to move the debug camera.
		// This function updates the camera using user input.
		Demo::UpdateDebugCamera();

		// The camera code produces 'view' and 'projection' matrices which must be passed to the GPU.
		// There are many ways to construct and control a camera.
		Demo::SetDebugCameraMatrices();

		// The 'Primitive Drawing Interface' collects lines points and triangles in batches and then draws them in one go.
		// This is suitable for debugging vector maths or testing gameplay features.
		Graphics::BeginPrimitiveBatch();

		// Make a simple animated value.
		static f32 fTime = 0.f;
		fTime += System::GetDeltaTime();

		// Finish the batch to pass these calls to the GPU.
		Graphics::EndPrimitiveBatch();

		// We can collect related primitives into further batches.
		Demo::DrawDebugGrid();

		//Update the flock of boids
		flock.UpdateFlock();
	
		static u32 frameCounter = 0;
		
		//Signal the framework to finish the frame.
		System::EndFrame();
	}

	// Make sure to shutdown the library before we end our main function.
	System::Shutdown();

	return 0;
};

void SetupGameGraphics()
{
	// The lighting interface allows us to set some light properties.
	// This could be animated during a frame.
	// Here we use using 3 lights in differing directions Red, Green and Blue
	Graphics::SetLightColour(0, ColourValue(0xFF0000));
	Graphics::SetLightDirection(0, Vector3f(1, 1, 1));
	Graphics::SetLightColour(1, ColourValue(0x00FF00));
	Graphics::SetLightDirection(1, Vector3f(1, 1, -1));
	Graphics::SetLightColour(2, ColourValue(0x0000FF));
	Graphics::SetLightDirection(2, Vector3f(-1, 1, -1));

	// A simple camera is provided in the demo code.
	// We can setup some of the debug camera parameters.
	Demo::SetDebugCameraPosition(Vector3f(0, 2, -15), 0.f, 0.0f);
	Demo::SetDebugCameraFOV(kfPi / 4.f, 0.1f, 25.f);
}

