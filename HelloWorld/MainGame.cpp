#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"

int DISPLAY_WIDTH = 640;
int DISPLAY_HEIGHT = 360;
int DISPLAY_SCALE = 2;


struct GameState
{
	float timer = 0.0f;
	int spriteID = 0;
};

GameState game_state;


// The entry point for a PlayBuffer program
void MainGameEntry( PLAY_IGNORE_COMMAND_LINE )
{
	Play::CreateManager( DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE );
}

// Called by PlayBuffer every frame (60 times a second!)
bool MainGameUpdate( float elapsedTime )
{
	Play::ClearDrawingBuffer( Play::cOrange );

	// Update timer
	game_state.timer += elapsedTime;
	
	
	// Obtain information from struct
	Play::DrawDebugText(
		{ DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 }, 
		Play::GetSpriteName(game_state.spriteID), 
		Play::cWhite
	);
	
	// Get mouse position
	Play::DrawSprite(game_state.spriteID, Play::GetMousePos(), game_state.timer);

	// Add one to Id when pressing space
	if ( Play::KeyPressed(VK_SPACE) )
	{
		game_state.spriteID++;
	}


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

