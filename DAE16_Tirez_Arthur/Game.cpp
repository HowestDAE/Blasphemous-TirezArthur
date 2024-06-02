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
#include "SaveManager.h"
#include "InputManager.h"
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
	m_InputManagerPtr = new InputManager{};
	m_SaveManagerPtr = new SaveManager{};
	m_TextureManagerPtr = new TextureManager{ };
	m_SoundManagerPtr = new SoundManager{};
	m_CameraPtr = new Camera{ GetViewPort().width, GetViewPort().height };
	m_EnemyManagerPtr = new EnemyManager{ m_TextureManagerPtr, m_SoundManagerPtr };
	m_LevelManagerPtr = new LevelManager{ m_TextureManagerPtr, m_EnemyManagerPtr, m_CameraPtr, m_SaveManagerPtr };
	m_EnemyManagerPtr->SetLevelManager(m_LevelManagerPtr);
	m_PlayerPtr = new Player{ m_TextureManagerPtr, m_LevelManagerPtr, m_EnemyManagerPtr, m_SoundManagerPtr, m_InputManagerPtr };
	m_EnemyManagerPtr->SetTargetPlayer(m_PlayerPtr);
	m_UiManagerPtr = new UiManager{ m_TextureManagerPtr, m_PlayerPtr, m_SoundManagerPtr, m_InputManagerPtr };
	m_LevelManagerPtr->LoadLevel("outside1");
	ShowCursor(false);
}

void Game::Cleanup()
{
	delete m_SaveManagerPtr;
	delete m_TextureManagerPtr;
	delete m_SoundManagerPtr;
	delete m_CameraPtr;
	delete m_PlayerPtr;
	delete m_LevelManagerPtr;
	delete m_UiManagerPtr;
	delete m_EnemyManagerPtr;
	delete m_InputManagerPtr;
	ShowCursor(true);
}

void Game::Update( float elapsedSec )
{
	if (!m_UiManagerPtr->GamePaused()){
		m_LevelManagerPtr->Update(elapsedSec);
		m_PlayerPtr->Update(elapsedSec);
		m_EnemyManagerPtr->Update(elapsedSec);
		m_CameraPtr->Aim(utils::GetCenter(m_PlayerPtr->GetHitbox()), elapsedSec);
	}
	m_UiManagerPtr->Update(elapsedSec);
}

void Game::Draw( ) const
{
	ClearBackground( );
	if (!m_UiManagerPtr->GamePaused()) {
		m_CameraPtr->ApplyTS();
		m_LevelManagerPtr->DrawBackGround();
		m_EnemyManagerPtr->Draw();
		m_PlayerPtr->Draw();
		m_LevelManagerPtr->DrawForeground();
		m_CameraPtr->Reset();
	}

	m_CameraPtr->ApplyS();
	m_UiManagerPtr->Draw();
	m_CameraPtr->Reset();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	m_InputManagerPtr->ProcessKeyDownEvent(e);
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	m_InputManagerPtr->ProcessKeyUpEvent(e);
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	m_InputManagerPtr->ProcessMouseDownEvent(e);
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	m_InputManagerPtr->ProcessMouseUpEvent(e);
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0666f, 0.0313f, 0.0117f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}
