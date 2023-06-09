#include "ContentCore.h"
#include "TitleLevel.h"
#include "TitleLevel2.h"
#include "PlayLevel.h"
#include "EndingLevel.h"
#include <GameEnginePlatform/GameEngineWindow.h>

ContentCore::ContentCore() 
{
}

ContentCore::~ContentCore() 
{
}

void ContentCore::Start() 
{
	GameEngineWindow::MainWindow.SetPosAndScale({100, 100}, { 1280, 800 });

	GameEngineCore::CreateLevel<TitleLevel>("TitleLevel");
	GameEngineCore::CreateLevel<TitleLevel2>("TitleLevel2");
	GameEngineCore::CreateLevel<PlayLevel>("PlayLevel");
	GameEngineCore::CreateLevel<EndingLevel>("EndingLevel");

	// 이 레벨이 화면에 보여라.
	GameEngineCore::ChangeLevel("TitleLevel");
}

// 행동한고.
void ContentCore::Update(float _Delta)
{
}

// 그려지고.
void ContentCore::Render(float _Delta)
{
}

// 정리된다.
void ContentCore::Release() 
{
}