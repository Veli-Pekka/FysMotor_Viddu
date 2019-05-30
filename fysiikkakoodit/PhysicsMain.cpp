///If you want to use Visual Leak Detector, you just have to install Visual Leak Detector to your computer
///and uncomment this include
#include <vld.h>
///Then when you build and run your program in Debug, VLD shows leaks and their sizes and possible data after the
///program has exited. All needed libs are already in-place.
///Check your leaks against below-mentioned CURRENT LEAK COUNT, if numbers are the same, you have done a good job!!
///CURRENT LEAK COUNT: 0 leaks WITH 0 bytes


#include <Engine/UserInterface.hpp>
#include <memory>

///For windows debugging
#include <Windows.h>
#include <string>
#include <iostream>

#include <PhysicalWorld.h>

int main()
{
	///First (DEBUG)
	emt::EngineSettings settings(
		"resources/textures/",
		"resources/models/",
		"resources/sounds/",
		"resources/fonts/",
		"../shaders/"
	);
	//RELEASE
	/*emt::EngineSettings settings(
		"resources/textures/",
		"resources/models/",
		"resources/sounds/",
		"resources/fonts/",
		"..shaders/"
	);*/


	emt::Initialize(settings);
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	///TESTING AREA, KEEP CLEAN!

	///Create material
	emt::Material obsidian(
		"obsidian",
		emt::Vec3f(0.05375f, 0.05f, 0.06625),			///Ambient
		emt::Vec3f(0.18275f, 0.17f, 0.22525f),			///Diffuse
		emt::Vec3f(0.332741f, 0.328634f, 0.346435f),	///Specular
		0.75f);											///Shininess
														///


	emt::AddParentScene(new PhysicalWorld(), true);

	////////////////////////////////////////////////////////////////
	bool loop = true;
	while (loop)
	{
		///In game loop
		emt::Gameloop();

		///End program with SPACE or ESC
		if (GetAsyncKeyState(0x1A) || GetAsyncKeyState(0x20))
		{
			loop = false;
			Sleep(100);
		}
	}
	//////////////////////////////////////////////////////////////////
	emt::Message msg(
		"\n\nProgram exiting...",
		emt::MessageType::Notice,
		emt::AttentionLevel::EXTREME
	);


	///Last
	emt::Uninitialize();
	////////////////////////////////////////////////////////////////

	return 0;
}