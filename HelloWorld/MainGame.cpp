#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"

int DISPLAY_WIDTH = 1280;
int DISPLAY_HEIGHT = 720;
int DISPLAY_SCALE = 1;


struct GameState
{
	int score = 0;
};

GameState game_state;

enum GameObjectType
{
	TYPE_NULL = -1,
	TYPE_AGENT8,
	TYPE_FAN,
	TYPE_TOOL,
	TYPE_COIN,
	TYPE_STAR,
	TYPE_LASER,
	TYPE_DESTROYED,
};

void HandlePlayercontrols()
{
	GameObject& obj_agent8 = Play::GetGameObjectByType(TYPE_AGENT8);
	if (Play::KeyDown(VK_UP))
	{
		obj_agent8.velocity = { 0, -4 };
		Play::SetSprite(obj_agent8, "agent8_climb", 0.25f);
	}
	else if (Play::KeyDown(VK_DOWN))
	{
		obj_agent8.acceleration = { 0, 1 };
		Play::SetSprite(obj_agent8, "agent8_fall", 0);
	}
	else
	{
		Play::SetSprite(obj_agent8, "agent8_hang", 0.02f);
		obj_agent8.velocity *= 0.5f;
		obj_agent8.acceleration = { 0, 0 };
	}

	// code above obtain the address of object agent 8. 
	// Then updating Game object's parameters by players input
	Play::UpdateGameObject(obj_agent8);

	if (Play::IsLeavingDisplayArea(obj_agent8)) obj_agent8.pos = obj_agent8.oldPos;
	Play::DrawLine({ obj_agent8.pos.x, 0 }, obj_agent8.pos, Play::cWhite);
	Play::DrawObjectRotated(obj_agent8);
};
void UpdateFan()
{
	GameObject& obj_fan = Play::GetGameObjectByType(TYPE_FAN);
	if (Play::RandomRoll(50) == 50)
	{
		int id = Play::CreateGameObject(TYPE_TOOL, obj_fan.pos, 50, "driver");
		GameObject& obj_tool = Play::GetGameObject(id);
		obj_tool.velocity = Point2f(-8, Play::RandomRollRange(-1, 1) * 6);

		if (Play::RandomRoll( 2 ) == 1)
		{
			Play::SetSprite(obj_tool, "spanner", 0);
			obj_tool.radius = 100;
			obj_tool.velocity.x = -4;
			obj_tool.rotSpeed = 0.1f;
		}
		Play::PlayAudio("tool");
	}
	Play::UpdateGameObject(obj_fan);

	if (Play::IsLeavingDisplayArea(obj_fan))
	{
		obj_fan.pos = obj_fan.oldPos;
		obj_fan.velocity.y *= -1;
	}
	Play::DrawObject(obj_fan);
};
void UpdateTools()
{
	GameObject& obj_agent8 = Play::GetGameObjectByType(TYPE_AGENT8);
	std::vector<int> vTools = Play::CollectGameObjectIDsByType(TYPE_TOOL);

	for (int id: vTools)
	{
		GameObject& obj_tool = Play::GetGameObject(id);

		if (Play::IsColliding(obj_tool, obj_agent8)) 
		{
			Play::StopAudioLoop("music");
			Play::PlayAudio("die");
			obj_agent8.pos = {-100, -100};
		}
		Play::UpdateGameObject(obj_tool);

		if(Play::IsLeavingDisplayArea(obj_tool, Play::VERTICAL))
		{
			obj_tool.pos = obj_tool.oldPos;
			obj_tool.velocity.y *= -1;
		}
		Play::DrawObjectRotated(obj_tool);

		if (!Play::IsVisible(obj_tool)) Play::DestroyGameObject(id);
	}
};


// This looks like unity, I like it
// The entry point for a PlayBuffer program
void MainGameEntry( PLAY_IGNORE_COMMAND_LINE )
{
	Play::CreateManager( DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE );
	Play::CentreAllSpriteOrigins();
	Play::LoadBackground("Data\\Backgrounds\\background.png");
	Play::StartAudioLoop("music");
	Play::CreateGameObject(TYPE_AGENT8, {115, 0}, 50, "agent8");
	int id_fan = Play::CreateGameObject(TYPE_FAN, {1140, 217}, 0, "fan");
	Play::GetGameObject(id_fan).velocity = {0, 3};
	Play::GetGameObject(id_fan).animSpeed = 1.0f;
}

// Called by PlayBuffer every frame (60 times a second!)
bool MainGameUpdate( float elapsedTime )
{
	Play::DrawBackground();
	HandlePlayercontrols();
	UpdateFan();
	UpdateTools();

	// Quit application
	Play::PresentDrawingBuffer();
	return Play::KeyDown( VK_ESCAPE );
}

// Gets called once when the player quits the game 
int MainGameExit( void )
{
	Play::DestroyManager();
	return PLAY_OK;
}