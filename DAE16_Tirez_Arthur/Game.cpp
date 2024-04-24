#include "pch.h"
#include "Game.h"
#include "TextureManager.h"
#include "Texture.h"
#include "Camera.h"
#include "Player.h"
#include "LevelManager.h"
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
	m_CameraPtr = new Camera{ GetViewPort().width, GetViewPort().height };
	m_LevelManagerPtr = new LevelManager{ m_TextureManagerPtr };
	m_PlayerPtr = new Player{ m_TextureManagerPtr, m_LevelManagerPtr };
	ShowCursor(false);
}

void Game::Cleanup()
{
	delete m_TextureManagerPtr;
	delete m_CameraPtr;
	delete m_PlayerPtr;
	delete m_LevelManagerPtr;
	ShowCursor(true);
}

void Game::Update( float elapsedSec )
{
	std::cout << "ElapsedSec: " << elapsedSec * 1000 << std::endl;
	m_PlayerPtr->Update(elapsedSec);
}

void Game::Draw( ) const
{
	ClearBackground( );
	m_CameraPtr->Aim(m_TextureManagerPtr->GetTextureWidth("indoor1"), m_TextureManagerPtr->GetTextureHeight("indoor1"), Point2f{ m_PlayerPtr->GetHitbox().left, m_PlayerPtr->GetHitbox().bottom});
	m_LevelManagerPtr->DrawBackGround();
	m_PlayerPtr->Draw();
	m_LevelManagerPtr->DrawForeground();
	m_CameraPtr->Reset();

	m_CameraPtr->ApplyScale();
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
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}
