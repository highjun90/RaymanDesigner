#include "Player.h"
#pragma region Headers

#include "ContentsEnum.h"
#include <Windows.h>
#include <GameEngineBase/GameEngineTime.h>
#include <GameEngineBase/GameEnginePath.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineWindowTexture.h>
#include <GameEnginePlatform/GameEngineSound.h>
#include <GameEngineCore/ResourcesManager.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>
#include <GameEngineCore/GameEngineCore.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "Bullet.h"
#include "Monster.h"
#include "PlayUIManager.h"
#include "PlayLevel.h"
#include "GoalPoint.h"
#include <GameEnginePlatform/GameEngineInput.h>

#pragma endregion

Player* Player::MainPlayer = nullptr;

Player::Player() 
{
}

Player::~Player() 
{
}


void Player::Start()
{
	if (false == ResourcesManager::GetInst().IsLoadTexture("Idle_RIght.bmp"))
	{
		DebugStartPoint = { 1300,2500 };
		//DebugStartPoint = { 0,0 };
		SetPos(DebugStartPoint);

		GameEnginePath FilePath;
		FilePath.SetCurrentPath();
		FilePath.MoveParentToExistsChild("ContentsResources");

		GameEnginePath FolderPath = FilePath;

		FilePath.MoveChild("ContentsResources\\Texture\\Player\\");

		// ResourcesManager::GetInst().TextureLoad(FilePath.PlusFilePath("Left_Player.bmp"));

		//亜幻備赤奄 什覗虞戚闘 去系
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Idle_Left.bmp"), 5, 3);
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Idle_RIght.bmp"), 5, 3);

		//鞍奄 什覗虞戚闘 去系
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Left_Player.bmp"), 5, 7);
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Right_Player.bmp"), 5, 7);

		//什覗鍵闘 什覗虞戚闘 去系
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Left_RaymanSprint.bmp"), 31, 1);
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Right_RaymanSprint.bmp"), 31, 1);

		//什覗鍵闘繊覗 什覗虞戚闘 去系
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Left_RaymanSprintJump.bmp"), 14, 1);
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Right_RaymanSprintJump.bmp"), 14, 1);

		//繊覗 什覗虞戚闘 去系
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Left_RaymanJump.bmp"), 42, 1);
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Right_RaymanJump.bmp"), 42, 1);

		//渋軒乞芝 什覗虞戚闘 去系
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Left_RaymanVictory.bmp"), 39, 1);
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Right_RaymanVictory.bmp"), 39, 1);

		//UI 去系
		ResourcesManager::GetInst().TextureLoad(FilePath.PlusFilePath("UI_LifeAndHp.bmp"));
		ResourcesManager::GetInst().TextureLoad(FilePath.PlusFilePath("UI_LeftDownMarble.bmp"));

		//識持還坪球
	/*	FolderPath.MoveChild("ContentsResources\\Texture\\");
		ResourcesManager::GetInst().CreateSpriteFolder("FolderPlayer", FolderPath.PlusFilePath("FolderPlayer"));

		ResourcesManager::GetInst().TextureLoad(FilePath.PlusFilePath("Test.bmp"));
		ResourcesManager::GetInst().TextureLoad(FilePath.PlusFilePath("HPBar.bmp"));*/
	}

	{

		MainRenderer = CreateRenderer(200);

		// MainRenderer->SetRenderScale({ 200, 200 });
		// MainRenderer->SetSrite("Left_Player.bmp");
		//MainRenderer->CreateAnimation("Test", "FolderPlayer");


		//亜幻備 赤奄 蕉艦五戚芝 去系
		MainRenderer->CreateAnimation("Left_Idle", "Idle_Left.bmp", 0, 10, 0.1f, true);
		MainRenderer->CreateAnimation("Right_Idle", "Idle_RIght.bmp", 0, 10, 0.1f, true);

		// MainRenderer->CreateAnimation("Right_Idle", "FolderPlayer");

		//鞍奄 蕉艦五戚芝 去系
		MainRenderer->CreateAnimation("Left_Run", "Left_Player.bmp", 0, 31, 0.02f, true);
		MainRenderer->CreateAnimation("Right_Run", "Right_Player.bmp", 0, 31, 0.02f, true);

		//什覗鍵闘 蕉艦五戚芝 去系
		MainRenderer->CreateAnimation("Left_RaymanSprint", "Left_RaymanSprint.bmp", 26, 0, 0.035f, true);
		MainRenderer->CreateAnimation("Right_RaymanSprint", "Right_RaymanSprint.bmp", 0, 26, 0.035f, true);

		//什覗鍵闘繊覗 蕉艦五戚芝 去系
		MainRenderer->CreateAnimation("Left_RaymanSprintJump", "Left_RaymanSprintJump.bmp", 0, 13, 0.04f, true);
		MainRenderer->CreateAnimation("Right_RaymanSprintJump", "Right_RaymanSprintJump.bmp", 0, 13, 0.04f, true);

		//繊覗 蕉艦五戚芝 去系
		MainRenderer->CreateAnimation("Left_RaymanJump", "Left_RaymanJump.bmp", 0, 29, 0.025f, false);
		MainRenderer->CreateAnimation("Right_RaymanJump", "Right_RaymanJump.bmp", 0, 29, 0.025f, false);
		//繊覗 馬悪政走 蕉艦五戚芝 去系
		MainRenderer->CreateAnimation("Left_RaymanJumpHold", "Left_RaymanJump.bmp", 30, 41, 0.04f, true);
		MainRenderer->CreateAnimation("Right_RaymanJumpHold", "Right_RaymanJump.bmp", 30, 41, 0.04f, true);

		//渋軒乞芝 蕉艦五戚芝 去系
		MainRenderer->CreateAnimation("Left_RaymanVictory", "Left_RaymanVictory.bmp", 0, 38, 0.06f, false);
		MainRenderer->CreateAnimation("Right_RaymanVictory", "Right_RaymanVictory.bmp", 0, 38, 0.06f, false);

		//MainRenderer->ChangeAnimation("Test");
		MainRenderer->SetRenderScaleToTexture();
	}

	//hp郊 識持還坪球
	//{
	//	HPRender = CreateRenderer("HPBar.bmp", RenderOrder::Play);
	//	HPRender->SetRenderPos({ 0, -200 });
	//	HPRender->SetRenderScale({ 40, 40 });
	//	HPRender->SetTexture("HPBar.bmp");
	//	// Ptr->SetText("か神格たびかし葛ったびかし葛ったびかし葛った顕し葛君たびしけい稽っ原閑", 40);
	//}

	/*{
		GameEngineRenderer* Ptr = CreateRenderer("HPBar.bmp", RenderOrder::Play);
		Ptr->SetRenderPos({ 0, -100 });
		Ptr->SetRenderScale({ 200, 40 });
		Ptr->SetTexture("HPBar.bmp");

	}*/

	//Player税 中宜端馬蟹研 亜軒徹澗 匂昔斗
	{
		BodyCollsion = CreateCollision(CollisionOrder::PlayerBody);
		BodyCollsion->SetCollisionScale({ 60, 120 });
		BodyCollsion->SetCollisionType(CollisionType::Rect);
	}

	//ChanageState(PlayerState::Idle);
	ChanageState(PlayerState::JumpHold);
	Dir = PlayerDir::Right;


	//BGM 巴傾戚嬢研 Level拭辞 亜閃神奄
	PlayLevel* MyLevel = dynamic_cast<PlayLevel*>(GetLevel());
	MyLevel->GetBGMPlayerToPlayLevel();
	BGMPlayerToPlayer = MyLevel->GetBGMPlayerToPlayLevel();
}

void Player::Update(float _Delta)
{

	// 蕉艦五戚芝 廃 塾戚適 魁蟹檎 陥献 楳疑聖 馬惟馬澗 坪球
	/*if (true == MainRenderer->IsAnimation("Left_Idle")
		&& true == MainRenderer->IsAnimationEnd())
	{
		
		Dir = PlayerDir::Right;
		ChangeAnimationState(CurState);
	}*/
	//森獣澗 図楕亜幻備 赤澗 蕉艦五戚芝 魁蟹檎 神献楕 亜幻備 蕉艦五戚芝生稽 郊荷惟 背轄依


	void CameraPosToDir();

	std::vector<GameEngineCollision*> _Col;
	if (true == BodyCollsion->Collision(CollisionOrder::MonsterBody, _Col
		, CollisionType::CirCle // 蟹研 紫唖莫生稽 坐操
		, CollisionType::CirCle // 雌企亀 紫唖莫生稽 坐操
	))
	{
		for (size_t i = 0; i < _Col.size(); i++)
		{
			GameEngineCollision* Collison = _Col[i];

			GameEngineActor* Actor = Collison->GetActor();

			Actor->Death();
		}
		// 蟹澗 佼什斗櫛 中宜廃暗醤.
	}

	std::vector<GameEngineCollision*> _GoalCollisionTest;
	if (true == BodyCollsion->Collision(CollisionOrder::GoalPoint, _GoalCollisionTest
		, CollisionType::Rect // 蟹研 紫唖莫生稽 坐操
		, CollisionType::Rect // 雌企亀 紫唖莫生稽 坐操
	))
	{
		ChanageState(PlayerState::Victory);
	}



	if (true == GameEngineInput::IsPress('L'))
	{
		// GameEngineSound::SoundLoad("C:\\AAAA\\AAAA\\A\\AAA.Mp3");
		// GameEngineSound::SoundPlay("AAA.Mp3");
		// GameEngineSound::PlayBgm("AAA.Mp3");
		// GameEngineSound::StopBgm("AAA.Mp3");

		//GameEngineWindow::MainWindow.AddDoubleBufferingCopyScaleRatio(1.0f * _Delta);

		// Monster::AllMonsterDeath();
	}

	// 巨獄益 庚切伸 徹壱 塊奄
	if (true == GameEngineInput::IsDown('C'))
	{
		if (DebugModeText == false)
		{
			DebugModeText = true;
		}
		else if (DebugModeText == true)
		{
			DebugModeText = false;
		}
	}


	//巨獄益 乞球
	if (true == GameEngineInput::IsDown('V'))
	{
		if (DebugMode == false)
		{
			DebugMode = true;
			BodyCollsion->Off();
			ChanageState(PlayerState::Debugmode);
		}
		else if(DebugMode == true)
		{
			GravityReset();

			DebugMode = false;
			BodyCollsion->On();
			ChanageState(PlayerState::JumpHold);
		}
		
	}


	// 中宜端 妊獣
	if (true == GameEngineInput::IsDown(VK_F2))
	{
		//GameEngineWindow::MainWindow.AddDoubleBufferingCopyScaleRatio(-1.0f * _Delta);
		GameEngineLevel::CollisionDebugRenderSwitch();
	}


	//獣拙, 曽戟走繊 授娃戚疑
	if (true == GameEngineInput::IsDown(VK_F3))
	{
		SetPos(DebugStartPoint);
	}
	if (true == GameEngineInput::IsDown(VK_F4))
	{
		float4 EndPoint = { 13200, 2850 };
		SetPos(EndPoint);
	}

	//茨匂昔闘 徹壱 塊奄
	if (true == GameEngineInput::IsDown(VK_F5))
	{
		bool GoalPointRender = GoalPoint::GetGoalPointIsRender();

		if (GoalPointRender == false)
		{
			GoalPoint::ChangeGoalPointRenderTrue();

			GameEngineCollision* GoalCollision = GoalPoint::GetGoalPointCollision();
			GoalCollision->On();
		}
		else if (GoalPointRender == true)
		{
			GoalPoint::ChangeGoalPointRenderFalse();

			GameEngineCollision* GoalCollision = GoalPoint::GetGoalPointCollision();
			GoalCollision->Off();
		}

	}

	//巨獄益繊 徹壱 塊奄
	if (true == GameEngineInput::IsDown(VK_F6))
	{
		if (DebugPointIsRender == false)
		{
			DebugPointIsRender = true;
		}
		else if (DebugPointIsRender == true)
		{
			DebugPointIsRender = false;
		}
	}

	//紫錘球 塊壱 徹奄
	if (true == GameEngineInput::IsDown(VK_F7))
	{
		if (SoundPlaying == false)
		{
			*(BGMPlayerToPlayer) = GameEngineSound::SoundPlay("CandyChateauBGM.ogg");
			SoundPlaying = true;
		}
		else if (SoundPlaying == true)
		{
			BGMPlayerToPlayer->Stop();
			SoundPlaying = false;
		}

	}

	

	

	//巷旋乞球

	StateUpdate(_Delta);

	CameraFocus();

	// Gravity();
}

void CameraPosToDir()
{
	
}

void Player::StateUpdate(float _Delta)
{
	switch (State)
	{
	case PlayerState::Idle:
		return IdleUpdate(_Delta);
	case PlayerState::Run:
		return RunUpdate(_Delta);
	case PlayerState::Jump:
		return JumpUpdate(_Delta);
	case PlayerState::JumpHold:
		return JumpHoldUpdate(_Delta);
	case PlayerState::Sprint:
		return SprintUpdate(_Delta);
	case PlayerState::SprintJump:
		return SprintJumpUpdate(_Delta);
	case PlayerState::Debugmode:
		return DebugmodeUpdate(_Delta);
	default:
		break;
	}

}

void Player::ChanageState(PlayerState _State)
{
	if (_State != State)
	{
		switch (_State)
		{
		case PlayerState::Idle:
			IdleStart();
			break;
		case PlayerState::Run:
			RunStart();
			break;
		case PlayerState::Jump:
			JumpStart();
			break;
		case PlayerState::JumpHold:
			JumpHoldStart();
			break;
		case PlayerState::Sprint:
			SprintStart();
			break;
		case PlayerState::SprintJump:
			SprintJumpStart();
			break;
		case PlayerState::Victory:
			VictoryStart();
			break;
		case PlayerState::Debugmode:
			DebugmodeStart();
			break;
		default:
			break;
		}
	}

	State = _State;
}


void Player::DirCheck()
{

	// 坪球級戚 授託旋生稽 叔楳鞠奄 凶庚拭 
	// D研 刊献雌殿稽 A研喚袈聖凶税 号狽穿発精 亜管馬走幻
	// A研 刊献雌殿稽 D研 喚袈聖凶拭澗 A税 坦軒亜 胡煽 戚欠嬢閃辞 号狽穿発戚 鞠走省奄凶庚拭 庚薦亜 降持梅陥.

	// 号狽聖 衣舛馬澗 徹級戚 乞砧 覗軒虞檎 益雌殿 益企稽 政走. 焼掘税 D亜 覗軒析凶 Left亜 鞠澗 依聖 号走.
	if (true == GameEngineInput::IsFree('A') && true == GameEngineInput::IsFree('D'))
	{
		return;
	}

	// A亜 喚携暗蟹 D亜 覗軒戚虞檎 Left稽 号狽穿発 昔汽 亜幻備赤嬢亀 Left研 郊虞左澗 薄雌戚 持沿.
	if (true == GameEngineInput::IsDown('A') || true == GameEngineInput::IsFree('D'))
	{
		Dir = PlayerDir::Left;
		ChangeAnimationState(CurState);
		return;
	}

	// D亜 喚携暗蟹 A亜 覗軒戚檎 Right稽 号狽 穿発.
	if (true == GameEngineInput::IsDown('D') || true == GameEngineInput::IsFree('A'))
	{
		Dir = PlayerDir::Right;
		ChangeAnimationState(CurState);
		return;
	}

	/*if (true == GameEngineInput::IsDown('W') || true == GameEngineInput::IsFree('S'))
	{
		Dir = PlayerDir::Up;
		return;
	}

	if (true == GameEngineInput::IsDown('S') || true == GameEngineInput::IsFree('W'))
	{
		Dir = PlayerDir::Down;
		return;
	}*/

	

	// 据掘 赤揮 坪球.
	/*PlayerDir CheckDir = PlayerDir::Left;

	if (true == GameEngineInput::IsDown('A'))
	{
		CheckDir = PlayerDir::Left;
	}
	else if (true == GameEngineInput::IsDown('D'))
	{
		CheckDir = PlayerDir::Right;
	}

	bool ChangeDir = false;

	if (CheckDir != PlayerDir::Max)
	{
		Dir = CheckDir;
		ChangeDir = true;
	}

	if (CheckDir != PlayerDir::Max && true == ChangeDir)
	{
		ChangeAnimationState(CurState);
	}*/

}

void Player::ChangeAnimationState(const std::string& _StateName)
{
	// "Idle"
	// _StateName

	std::string AnimationName;


	switch (Dir)
	{
	case PlayerDir::Right:
		AnimationName = "Right_";
		break;
	case PlayerDir::Left:
		AnimationName = "Left_";
		break;
	//case PlayerDir::Up:
	//	AnimationName = "Right_";
	//	break;
	//case PlayerDir::Down:
	//	AnimationName = "Right_";
	//	break;

	default:
		break;
	}

	AnimationName += _StateName;

	CurState = _StateName;

	MainRenderer->ChangeAnimation(AnimationName);
}



void Player::LevelStart() 
{
	MainPlayer = this;
}

void Player::Render(float _Delta)
{

	HDC dc = GameEngineWindow::MainWindow.GetBackBuffer()->GetImageDC();
	
	if (DebugModeText == true)
	{
		
		std::string C_Key = "";
		C_Key += "C: 庚切伸 妊獣";
		TextOutA(dc, 2, 3, C_Key.c_str(), (int)C_Key.size());

		

		std::string PlayerColState = "";
		PlayerColState += "Plyaer Collision: ";
		if (DebugMode == false)
		{
			PlayerColState += "(OFF)";
		}
		else if (DebugMode == true)
		{
			PlayerColState += "(ON)";
		}
		TextOutA(dc, 2, 23, PlayerColState.c_str(), (int)PlayerColState.size());
		
		
		/*std::string Text2 = "";
		Text2 += "(徹脊径)";
		TextOutA(dc, 2, 120, Text2.c_str(), (int)Text2.size());*/



		std::string V_Key = "";
		V_Key += "V: 砺什闘乞球 ";
		if (DebugMode == false)
		{
			V_Key += "(OFF)";
		}
		else if (DebugMode == true)
		{
			V_Key += "(ON)";
		}
		TextOutA(dc, 2, 140, V_Key.c_str(), (int)V_Key.size());

		std::string Text1 = "";
		Text1 += "砺什闘乞球 紗亀: (";
		Text1 += std::to_string((int)DebugSpeed);
		Text1 += ")";
		TextOutA(dc, 2, 160, Text1.c_str(), (int)Text1.size());

		std::string One_Key = "";
		One_Key += "1: 砺什闘乞球 戚紗 +100";
		TextOutA(dc, 2, 180, One_Key.c_str(), (int)One_Key.size());

		std::string Two_Key = "";
		Two_Key += "2: 砺什闘乞球 戚紗 -100";
		TextOutA(dc, 2, 200, Two_Key.c_str(), (int)Two_Key.size());

		

		//繕拙狛
		std::string WASD_Key = "";
		WASD_Key += "WD: 戚疑 ";
		TextOutA(dc, 2, 260, WASD_Key.c_str(), (int)WASD_Key.size());

		std::string J_Key = "";
		J_Key += "J: 含軒奄 ";
		TextOutA(dc, 2, 280, J_Key.c_str(), (int)J_Key.size());

		std::string Space_Key = "";
		Space_Key += "Space: 繊覗 ";
		TextOutA(dc, 2, 300, Space_Key.c_str(), (int)Space_Key.size());





		std::string F1_Key = "";
		F1_Key += "F1: 己穿発 ";
		TextOutA(dc, 1130, 30, F1_Key.c_str(), (int)F1_Key.size());

		std::string F2_Key = "";
		F2_Key += "F2: Collision妊獣 ";
		TextOutA(dc, 1130, 50, F2_Key.c_str(), (int)F2_Key.size());

		std::string F3_Key = "";
		F3_Key += "F3: 獣拙走繊戚疑 ";
		TextOutA(dc, 1130, 70, F3_Key.c_str(), (int)F3_Key.size());

		std::string F4_Key = "";
		F4_Key += "F4: 渋軒走繊戚疑 ";
		TextOutA(dc, 1130, 90, F4_Key.c_str(), (int)F4_Key.size());

		std::string F5_Key = "";
		F5_Key += "F5: 茨昔妊走毒 ";
		bool GoalPointRender = GoalPoint::GetGoalPointIsRender();
		if (GoalPointRender == false)
		{
			F5_Key += "(OFF)";
		}
		else if (GoalPointRender == true)
		{
			F5_Key += "(ON)";
		}
		TextOutA(dc, 1130, 110, F5_Key.c_str(), (int)F5_Key.size());

		std::string F6_Key = "";
		F6_Key += "F6: 溌昔繊 ";
		if (DebugPointIsRender == false)
		{
			F6_Key += "(OFF)";
		}
		else if (DebugPointIsRender == true)
		{
			F6_Key += "(ON)";
		}
		TextOutA(dc, 1130, 130, F6_Key.c_str(), (int)F6_Key.size());

		std::string F7_Key = "";
		F7_Key += "F7: BGM ";
		if (SoundPlaying == false)
		{
			F7_Key += "(OFF)";
		}
		else if (SoundPlaying == true)
		{
			F7_Key += "(ON)";
		}
		TextOutA(dc, 1130, 150, F7_Key.c_str(), (int)F7_Key.size());

	/*	std::string F8_Key = "";
		F8_Key += "F8: 亀鐸走 肢薦 ";
		TextOutA(dc, 1130, 170, F8_Key.c_str(), (int)F8_Key.size());

		std::string F9_Key = "";
		F9_Key += "F9: 巨獄益繊妊獣 ";
		TextOutA(dc, 1130, 190, F9_Key.c_str(), (int)F9_Key.size());*/

		
	}

	if (DebugPointIsRender == true)
	{
		//巨獄益遂 馬鍾繊 幻級奄

		//巨獄益遂 呪帖溌昔 痕呪
		float4 a = GetPos();
		float4 b = GetLevel()->GetMainCamera()->GetPos();
		float4 c = GetPos() - GetLevel()->GetMainCamera()->GetPos();
		//馬鍾繊 幻級凶 益撹 Actor税 Pos昔 GetPos()幻 馬檎 鞠澗暗 焼観亜? 訊 瓜戚 朝五虞 是帖研 皐蟹? 益君檎 馬鍾繊戚 Actor税 pos左陥 朝五虞 pos幻鏑 希 菰軒 恭嬢走惟 鞠澗暗 焼劃?
		// -> 


		CollisionData Data;

		Data.Pos = ActorCameraPos();
		Data.Scale = { 5,5 };
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());

		//図楕繊
		Data.Pos = ActorCameraPos() + LeftCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());

		//神献楕繊
		Data.Pos = ActorCameraPos() + RightCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());

		//焼掘繊
		Data.Pos = ActorCameraPos() + DownCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
	}


}