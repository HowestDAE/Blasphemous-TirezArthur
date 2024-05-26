#include "pch.h"
#include "Game.h"
#include "TextureManager.h"
#include "Texture.h"
#include "Camera.h"
#include "Player.h"
#include "LevelManager.h"
#include "UiManager.h"
#include "EnemyManager.h"
#include "SoundManager.h"
#include "utils.h"
#include <iostream>

Game::Game( const Window& window ) 
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize()
{
	m_TextureManagerPtr = new TextureManager{ };
	m_SoundManagerPtr = new SoundManager{};
	m_CameraPtr = new Camera{ GetViewPort().width, GetViewPort().height };
	m_EnemyManagerPtr = new EnemyManager{ m_TextureManagerPtr, m_SoundManagerPtr };
	m_LevelManagerPtr = new LevelManager{ m_TextureManagerPtr, m_EnemyManagerPtr, m_CameraPtr };
	m_EnemyManagerPtr->SetLevelManager(m_LevelManagerPtr);
	m_PlayerPtr = new Player{ m_TextureManagerPtr, m_LevelManagerPtr, m_EnemyManagerPtr, m_SoundManagerPtr };
	m_EnemyManagerPtr->SetTargetPlayer(m_PlayerPtr);
	m_UiManagerPtr = new UiManager{};
	m_LevelManagerPtr->LoadLevel("outside1");
	ShowCursor(false);
}

void Game::Cleanup()
{
	delete m_TextureManagerPtr;
	delete m_SoundManagerPtr;
	delete m_CameraPtr;
	delete m_PlayerPtr;
	delete m_LevelManagerPtr;
	delete m_UiManagerPtr;
	delete m_EnemyManagerPtr;
	ShowCursor(true);
}

void Game::Update( float elapsedSec )
{
	m_PlayerPtr->Update(elapsedSec);
	m_EnemyManagerPtr->Update(elapsedSec);
	m_CameraPtr->Aim(utils::GetCenter(m_PlayerPtr->GetHitbox()), elapsedSec);
}

void Game::Draw( ) const
{
	ClearBackground( );
	m_CameraPtr->ApplyTS();
	m_LevelManagerPtr->DrawBackGround();
	m_EnemyManagerPtr->Draw();
	m_PlayerPtr->Draw();
	m_LevelManagerPtr->DrawForeground();
	m_CameraPtr->Reset();

	m_CameraPtr->ApplyS();
	// Draw ui
	m_CameraPtr->Reset();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0666f, 0.0313f, 0.0117f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}
