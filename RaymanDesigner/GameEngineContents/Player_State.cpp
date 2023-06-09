#include "Player.h"
#include "ContentsEnum.h"
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>
#include <GameEnginePlatform/GameEngineWindow.h>

#include "PlayLevel.h"

void Player::IdleStart()
{
	ChangeAnimationState("Idle");

	// MainRenderer->ChangeAnimation("Right_Idle");
}

void Player::RunStart()
{
	ChangeAnimationState("Run");
}

void Player::JumpStart()
{
	ChangeAnimationState("RaymanJump");
	SetGravityVector(float4::UP * 1100.0f);
}

void Player::JumpHoldStart()
{
	ChangeAnimationState("RaymanJumpHold");
}

void Player::SprintStart()
{
	ChangeAnimationState("RaymanSprint");
}

void Player::SprintJumpStart()
{
	ChangeAnimationState("RaymanSprintJump");
	SetGravityVector(float4::UP * 1100.0f);
}

void Player::LieStart()
{
	SwitchToLieCollision();
	ChangeAnimationState("RaymanLie");
}

void Player::LieMoveStart()
{
	SwitchToLieCollision();
	ChangeAnimationState("RaymanLieMove");
}


void Player::VictoryStart()
{
	//배경음끄기
	if (SoundPlaying == true)
	{
		BGMPlayerToPlayer->Stop();
		SoundPlaying = false;
	}

	GameEngineSound::SoundPlay("Victory.mp3");
	ChangeAnimationState("RaymanVictory");
}

void Player::DieStart()
{
	MainRenderer->SetRenderPos({ 0,0 });
	GameEngineSound::SoundPlay("RaymanDie.mp3");
	ChangeAnimationState("RaymanDie");
}


void Player::IdleUpdate(float _Delta)
{
	if (true == MainRenderer->IsAnimationEnd())
	{
		++TestValue;
	}

	{
		unsigned int Color = GetGroundColor(RGB(255, 255, 255), DownCheck);
		if (RGB(255, 255, 255) == Color)
		{
			Gravity(_Delta);
		}
		else
		{
			unsigned int CheckColor = GetGroundColor(RGB(255, 255, 255), float4::UP);

			while (CheckColor != RGB(255, 255, 255))
			{
				CheckColor = GetGroundColor(RGB(255, 255, 255), float4::UP);
				AddPos(float4::UP);
			}


			GravityReset();
		}
	}


	if (true == GameEngineInput::IsDown('A')
		|| true == GameEngineInput::IsDown('W')
		|| true == GameEngineInput::IsDown('S')
		|| true == GameEngineInput::IsDown('D'))
	{
		DirCheck();
		ChanageState(PlayerState::Run);
		return;
	}

	if (true == GameEngineInput::IsPress('A')
		|| true == GameEngineInput::IsPress('D'))
	{
		DirCheck();
		ChanageState(PlayerState::Run);
		return;
	}

	if (true == GameEngineInput::IsPress(VK_SPACE))
	{
		ChanageState(PlayerState::Jump);
		return;
	}

	if (true == GameEngineInput::IsPress('S'))
	{
		ChanageState(PlayerState::Lie);
		return;
	}

	// 줄줄이 사탕으로 
	//if (true)
	//{
	//	ChanageState(PlayerState::Idle);
	//}

}


void Player::RunUpdate(float _Delta)
{
	// 중력 적용 
	{
		unsigned int Color = GetGroundColor(RGB(255, 255, 255), DownCheck);
		if (RGB(255, 255, 255) == Color)
		{
			Gravity(_Delta);
		}
		else
		{
			unsigned int CheckColor = GetGroundColor(RGB(255, 255, 255), DownCheck);

			while (CheckColor != RGB(255, 255, 255))
			{
				CheckColor = GetGroundColor(RGB(255, 255, 255), DownCheck);
				AddPos(float4::UP);
			}

			GravityReset();
		}
	}

	DirCheck();

	float Speed = 350.0f;

	float4 MovePos = float4::ZERO;
	float4 CheckPos = float4::ZERO;


	//걸으면서 뛰기
	if (true == GameEngineInput::IsPress('A') && GameEngineInput::IsPress(VK_SPACE) && Dir == PlayerDir::Left)
	{
		DirCheck();
		SetJumpSpeed(450.0f);
		ChanageState(PlayerState::Jump);
	}
	else if (true == GameEngineInput::IsPress('D') && GameEngineInput::IsPress(VK_SPACE) && Dir == PlayerDir::Right)
	{
		DirCheck();
		SetJumpSpeed(450.0f);
		ChanageState(PlayerState::Jump);
	}


		if (true == GameEngineInput::IsPress('A') && Dir == PlayerDir::Left)
		{
			CheckPos = LeftCheck;

			MovePos = { -Speed * _Delta, 0.0f };

			// unsigned int Color = GetGroundColor(RGB(255, 255, 255));

		}
		else if (true == GameEngineInput::IsPress('D') && Dir == PlayerDir::Right)
		{
			CheckPos = RightCheck;

			MovePos = { Speed * _Delta, 0.0f };
		}
	


	//달리는 키 눌렀을때 상태전환
	if (true == GameEngineInput::IsPress('A') && GameEngineInput::IsPress('J') && Dir == PlayerDir::Left)
	{
		DirCheck();
		ChanageState(PlayerState::Sprint);
	}
	else if (true == GameEngineInput::IsPress('D') && GameEngineInput::IsPress('J') && Dir == PlayerDir::Right)
	{
		DirCheck();
		ChanageState(PlayerState::Sprint);
	}



	{
		unsigned int Color = GetGroundColor(RGB(255, 255, 255), CheckPos);

		if (Color == RGB(255, 255, 255))
		{
			// MovePos를 바꿔버리는 방법이 있을것이고.

			if (RGB(255, 255, 255) == GetGroundColor(RGB(255, 255, 255), MovePos))
			{
				float4 XPos = float4::ZERO;
				float4 Dir = MovePos.X <= 0.0f ? float4::RIGHT : float4::LEFT;

				while (RGB(136, 136, 136) != GetGroundColor(RGB(255, 255, 255), MovePos + XPos))
				{
					XPos += Dir;

					if (abs(XPos.X) > 50.0f)
					{
						break;
					}
				}

				float4 YPos = float4::ZERO;
				while (RGB(136, 136, 136) != GetGroundColor(RGB(255, 255, 255), MovePos + YPos))
				{
					YPos.Y += 1;

					if (YPos.Y > 60.0f)
					{
						break;
					}
				}

				if (abs(XPos.X) >= YPos.Y)
				{
					while (RGB(136, 136, 136) != GetGroundColor(RGB(255, 255, 255), MovePos))
					{
						MovePos.Y += 1;
					}
				}

			}

			// 내가 움직이려는 
			// GetGroundColor(RGB(255, 255, 255), MovePos);

			AddPos(MovePos);
		}
	}

	if (true == GameEngineInput::IsDown('S'))
	{
		DirCheck();
		ChanageState(PlayerState::Lie);

	}

	if (MovePos == float4::ZERO)
	{
		DirCheck();
		ChanageState(PlayerState::Idle);
	}

}


void Player::JumpUpdate(float _Delta)
{
	Gravity(_Delta);
	DirCheck();

	float Speed = GetJumpSpeed();
	float4 MovePos = float4::ZERO;
	float4 CheckPos = float4::ZERO;

	if (true == GameEngineInput::IsPress('A') && Dir == PlayerDir::Left)
	{
		CheckPos = LeftCheck;

		MovePos = { -Speed * _Delta, 0.0f };

		// unsigned int Color = GetGroundColor(RGB(255, 255, 255));

	}
	else if (true == GameEngineInput::IsPress('D') && Dir == PlayerDir::Right)
	{
		CheckPos = RightCheck;

		MovePos = { Speed * _Delta, 0.0f };
	}



	{
		unsigned int Color = GetGroundColor(RGB(255, 255, 255), CheckPos);

		if (Color == RGB(255, 255, 255))
		{
			// MovePos를 바꿔버리는 방법이 있을것이고.

			if (RGB(255, 255, 255) == GetGroundColor(RGB(255, 255, 255), MovePos))
			{
				float4 XPos = float4::ZERO;
				float4 Dir = MovePos.X <= 0.0f ? float4::RIGHT : float4::LEFT;

				while (RGB(136, 136, 136) != GetGroundColor(RGB(255, 255, 255), MovePos + XPos))
				{
					XPos += Dir;

					if (abs(XPos.X) > 50.0f)
					{
						break;
					}
				}

				float4 YPos = float4::ZERO;
				while (RGB(136, 136, 136) != GetGroundColor(RGB(255, 255, 255), MovePos + YPos))
				{
					YPos.Y += 1;

					if (YPos.Y > 60.0f)
					{
						break;
					}
				}

				if (abs(XPos.X) >= YPos.Y)
				{
					while (RGB(136, 136, 136) != GetGroundColor(RGB(255, 255, 255), MovePos))
					{
						MovePos.Y += 1;
					}
				}

			}

			// 내가 움직이려는 
			// GetGroundColor(RGB(255, 255, 255), MovePos);

			AddPos(MovePos);
			//GetLevel()->GetMainCamera()->AddPos(MovePos);
			//원래는 Play_State에서 카메라 화면 이동했는데 카메라 오버 막는 기능 추가하면서 PlayActor로 기능이 이동함
		}
	}



	//AddPos(MovePos);
	{
		unsigned int Color = GetGroundColor(RGB(255, 255, 255), DownCheck);
		if (RGB(255, 255, 255) != Color)
		{
			ChanageState(PlayerState::Idle);
			return;
		}
	}

	if (MainRenderer->GetCurFrame() == 29)
	{
		ChanageState(PlayerState::JumpHold);
	}
}

void Player::JumpHoldUpdate(float _Delta)
{
	Gravity(_Delta);
	DirCheck();


	float Speed = GetJumpSpeed();
	float4 MovePos = float4::ZERO;
	float4 CheckPos = float4::ZERO;

	if (true == GameEngineInput::IsPress('A') && Dir == PlayerDir::Left)
	{
		CheckPos = LeftCheck;

		MovePos = { -Speed * _Delta, 0.0f };

		// unsigned int Color = GetGroundColor(RGB(255, 255, 255));

	}
	else if (true == GameEngineInput::IsPress('D') && Dir == PlayerDir::Right)
	{
		CheckPos = RightCheck;

		MovePos = { Speed * _Delta, 0.0f };
	}



	{
		unsigned int Color = GetGroundColor(RGB(255, 255, 255), CheckPos);

		if (Color == RGB(255, 255, 255))
		{
			// MovePos를 바꿔버리는 방법이 있을것이고.

			if (RGB(255, 255, 255) == GetGroundColor(RGB(255, 255, 255), MovePos))
			{
				float4 XPos = float4::ZERO;
				float4 Dir = MovePos.X <= 0.0f ? float4::RIGHT : float4::LEFT;

				while (RGB(136, 136, 136) != GetGroundColor(RGB(255, 255, 255), MovePos + XPos))
				{
					XPos += Dir;

					if (abs(XPos.X) > 50.0f)
					{
						break;
					}
				}

				float4 YPos = float4::ZERO;
				while (RGB(136, 136, 136) != GetGroundColor(RGB(255, 255, 255), MovePos + YPos))
				{
					YPos.Y += 1;

					if (YPos.Y > 60.0f)
					{
						break;
					}
				}

				if (abs(XPos.X) >= YPos.Y)
				{
					while (RGB(136, 136, 136) != GetGroundColor(RGB(255, 255, 255), MovePos))
					{
						MovePos.Y += 1;
					}
				}

			}

			// 내가 움직이려는 
			// GetGroundColor(RGB(255, 255, 255), MovePos);

			AddPos(MovePos);
			//GetLevel()->GetMainCamera()->AddPos(MovePos);
			//원래는 Play_State에서 카메라 화면 이동했는데 카메라 오버 막는 기능 추가하면서 PlayActor로 기능이 이동함
		}
	}



	//AddPos(MovePos);
	{
		unsigned int Color = GetGroundColor(RGB(255, 255, 255), DownCheck);
		if (RGB(255, 255, 255) != Color)
		{
			ChanageState(PlayerState::Idle);
			return;
		}
	}
}


void Player::SprintUpdate(float _Delta)
{

	// 중력 적용 
	{
		unsigned int Color = GetGroundColor(RGB(255, 255, 255), DownCheck);
		if (RGB(255, 255, 255) == Color)
		{
			Gravity(_Delta);
		}
		else
		{
			int a = 0;
			unsigned int CheckColor = GetGroundColor(RGB(255, 255, 255), DownCheck);

			while (CheckColor != RGB(255, 255, 255))
			{
				CheckColor = GetGroundColor(RGB(255, 255, 255), DownCheck);
				AddPos(float4::UP);
			}


			GravityReset();
		}

	}

	float Speed = 700.0f;

	float4 MovePos = float4::ZERO;
	float4 CheckPos = float4::ZERO;


	//달리면서 뛰기
	if (true == GameEngineInput::IsPress('A') && GameEngineInput::IsPress(VK_SPACE) && Dir == PlayerDir::Left)
	{
		
		SetJumpSpeed(800.0f);
		ChanageState(PlayerState::SprintJump);
	}
	else if (true == GameEngineInput::IsPress('D') && GameEngineInput::IsPress(VK_SPACE) && Dir == PlayerDir::Right)
	{
		
		SetJumpSpeed(800.0f);
		ChanageState(PlayerState::SprintJump);
	}



	if (true == GameEngineInput::IsPress('A') && Dir == PlayerDir::Left)
	{
		CheckPos = LeftCheck;

		MovePos = { -Speed * _Delta, 0.0f };

	}
	else if (true == GameEngineInput::IsPress('D') && Dir == PlayerDir::Right)
	{
		CheckPos = RightCheck;

		MovePos = { Speed * _Delta, 0.0f };
	}
	

	if (true == GameEngineInput::IsUp('J') )
	{
		DirCheck();
		ChanageState(PlayerState::Run);

		// unsigned int Color = GetGroundColor(RGB(255, 255, 255));

	}


	



	{
		unsigned int Color = GetGroundColor(RGB(255, 255, 255), CheckPos);

		if (Color == RGB(255, 255, 255))
		{
			// MovePos를 바꿔버리는 방법이 있을것이고.

			if (RGB(255, 255, 255) == GetGroundColor(RGB(255, 255, 255), MovePos))
			{
				float4 XPos = float4::ZERO;
				float4 Dir = MovePos.X <= 0.0f ? float4::RIGHT : float4::LEFT;

				while (RGB(136, 136, 136) != GetGroundColor(RGB(255, 255, 255), MovePos + XPos))
				{
					XPos += Dir;

					if (abs(XPos.X) > 50.0f)
					{
						break;
					}
				}

				float4 YPos = float4::ZERO;
				while (RGB(136, 136, 136) != GetGroundColor(RGB(255, 255, 255), MovePos + YPos))
				{
					YPos.Y += 1;

					if (YPos.Y > 60.0f)
					{
						break;
					}
				}

				if (abs(XPos.X) >= YPos.Y)
				{
					while (RGB(136, 136, 136) != GetGroundColor(RGB(255, 255, 255), MovePos))
					{
						MovePos.Y += 1;
					}
				}

			}

			// 내가 움직이려는 
			// GetGroundColor(RGB(255, 255, 255), MovePos);

			AddPos(MovePos);
		}
	}

	if (MovePos == float4::ZERO)
	{
		DirCheck();
		ChanageState(PlayerState::Idle);
	}
	//{
	//	unsigned int Color = GetGroundColor(RGB(255, 255, 255), CheckPos);

	//	if (Color == RGB(255, 255, 255))
	//	{
	//		AddPos(MovePos);
	//		GetLevel()->GetMainCamera()->AddPos(MovePos);
	//	}
	//}

}

void Player::SprintJumpUpdate(float _Delta)
{
	Gravity(_Delta);
	DirCheck();

	float Speed = GetJumpSpeed();
	float4 MovePos = float4::ZERO;
	float4 CheckPos = float4::ZERO;

	if (true == GameEngineInput::IsPress('A') && Dir == PlayerDir::Left)
	{
		CheckPos = LeftCheck;

		MovePos = { -Speed * _Delta, 0.0f };

		// unsigned int Color = GetGroundColor(RGB(255, 255, 255));

	}
	else if (true == GameEngineInput::IsPress('D') && Dir == PlayerDir::Right)
	{
		CheckPos = RightCheck;

		MovePos = { Speed * _Delta, 0.0f };
	}



	{
		unsigned int Color = GetGroundColor(RGB(255, 255, 255), CheckPos);

		if (Color == RGB(255, 255, 255))
		{
			// MovePos를 바꿔버리는 방법이 있을것이고.

			if (RGB(255, 255, 255) == GetGroundColor(RGB(255, 255, 255), MovePos))
			{
				float4 XPos = float4::ZERO;
				float4 Dir = MovePos.X <= 0.0f ? float4::RIGHT : float4::LEFT;

				while (RGB(136, 136, 136) != GetGroundColor(RGB(255, 255, 255), MovePos + XPos))
				{
					XPos += Dir;

					if (abs(XPos.X) > 50.0f)
					{
						break;
					}
				}

				float4 YPos = float4::ZERO;
				while (RGB(136, 136, 136) != GetGroundColor(RGB(255, 255, 255), MovePos + YPos))
				{
					YPos.Y += 1;

					if (YPos.Y > 60.0f)
					{
						break;
					}
				}

				if (abs(XPos.X) >= YPos.Y)
				{
					while (RGB(136, 136, 136) != GetGroundColor(RGB(255, 255, 255), MovePos))
					{
						MovePos.Y += 1;
					}
				}

			}

			// 내가 움직이려는 
			// GetGroundColor(RGB(255, 255, 255), MovePos);

			AddPos(MovePos);
			//GetLevel()->GetMainCamera()->AddPos(MovePos);
			//원래는 Play_State에서 카메라 화면 이동했는데 카메라 오버 막는 기능 추가하면서 PlayActor로 기능이 이동함
		}
	}



	//AddPos(MovePos);
	{
		unsigned int Color = GetGroundColor(RGB(255, 255, 255), DownCheck);
		if (RGB(255, 255, 255) != Color)
		{
			ChanageState(PlayerState::Idle);
			return;
		}
	}

}

void Player::LieUpdate(float _Delta)
{
	if (true == MainRenderer->IsAnimationEnd())
	{
		++TestValue;
	}

	MainRenderer->SetRenderPos(LieRenderPoint);


	{
		unsigned int Color = GetGroundColor(RGB(255, 255, 255), DownCheck);
		if (RGB(255, 255, 255) == Color)
		{
			Gravity(_Delta);
		}
		else
		{
			unsigned int CheckColor = GetGroundColor(RGB(255, 255, 255), float4::UP);

			while (CheckColor != RGB(255, 255, 255))
			{
				CheckColor = GetGroundColor(RGB(255, 255, 255), float4::UP);
				AddPos(float4::UP);
			}


			GravityReset();
		}
	}


	if (true == GameEngineInput::IsDown('A')
		|| true == GameEngineInput::IsDown('W')
		|| true == GameEngineInput::IsDown('S')
		|| true == GameEngineInput::IsDown('D'))
	{
		DirCheck();
		ChanageState(PlayerState::LieMove);
		return;
	}

	if (true == GameEngineInput::IsPress('A')
		|| true == GameEngineInput::IsPress('D'))
	{
		DirCheck();
		ChanageState(PlayerState::LieMove);
		return;
	}

	if (true == GameEngineInput::IsUp('S'))
	{
		DirCheck();

		MainRenderer->SetRenderPos({ 0,0 });
		SwitchToBodyCollsion();
		ChanageState(PlayerState::Idle);

	}
}

void Player::LieMoveUpdate(float _Delta)
{
	// 중력 적용 
	{
		unsigned int Color = GetGroundColor(RGB(255, 255, 255), DownCheck);
		if (RGB(255, 255, 255) == Color)
		{
			Gravity(_Delta);
		}
		else
		{
			unsigned int CheckColor = GetGroundColor(RGB(255, 255, 255), DownCheck);

			while (CheckColor != RGB(255, 255, 255))
			{
				CheckColor = GetGroundColor(RGB(255, 255, 255), DownCheck);
				AddPos(float4::UP);
			}

			GravityReset();
		}
	}

	DirCheck();

	float Speed = 350.0f;

	float4 MovePos = float4::ZERO;
	float4 CheckPos = float4::ZERO;


	////걸으면서 뛰기
	//if (true == GameEngineInput::IsPress('A') && GameEngineInput::IsPress(VK_SPACE) && Dir == PlayerDir::Left)
	//{
	//	DirCheck();
	//	SetJumpSpeed(450.0f);
	//	ChanageState(PlayerState::Jump);
	//}
	//else if (true == GameEngineInput::IsPress('D') && GameEngineInput::IsPress(VK_SPACE) && Dir == PlayerDir::Right)
	//{
	//	DirCheck();
	//	SetJumpSpeed(450.0f);
	//	ChanageState(PlayerState::Jump);
	//}


	if (true == GameEngineInput::IsPress('A') && Dir == PlayerDir::Left)
	{
		CheckPos = LeftCheck_Lie;

		MovePos = { -Speed * _Delta, 0.0f };

		// unsigned int Color = GetGroundColor(RGB(255, 255, 255));

	}
	else if (true == GameEngineInput::IsPress('D') && Dir == PlayerDir::Right)
	{
		CheckPos = RightCheck_Lie;

		MovePos = { Speed * _Delta, 0.0f };
	}


	if (true == GameEngineInput::IsUp('A') || true == GameEngineInput::IsUp('D'))
	{
		DirCheck();
		ChanageState(PlayerState::Lie);
	}
	////달리는 키 눌렀을때 상태전환
	//if (true == GameEngineInput::IsPress('A') && GameEngineInput::IsPress('J') && Dir == PlayerDir::Left)
	//{
	//	DirCheck();
	//	ChanageState(PlayerState::Sprint);
	//}
	//else if (true == GameEngineInput::IsPress('D') && GameEngineInput::IsPress('J') && Dir == PlayerDir::Right)
	//{
	//	DirCheck();
	//	ChanageState(PlayerState::Sprint);
	//}



	{
		unsigned int Color = GetGroundColor(RGB(255, 255, 255), CheckPos);

		if (Color == RGB(255, 255, 255))
		{
			// MovePos를 바꿔버리는 방법이 있을것이고.

			if (RGB(255, 255, 255) == GetGroundColor(RGB(255, 255, 255), MovePos))
			{
				float4 XPos = float4::ZERO;
				float4 Dir = MovePos.X <= 0.0f ? float4::RIGHT : float4::LEFT;

				while (RGB(136, 136, 136) != GetGroundColor(RGB(255, 255, 255), MovePos + XPos))
				{
					XPos += Dir;

					if (abs(XPos.X) > 50.0f)
					{
						break;
					}
				}

				float4 YPos = float4::ZERO;
				while (RGB(136, 136, 136) != GetGroundColor(RGB(255, 255, 255), MovePos + YPos))
				{
					YPos.Y += 1;

					if (YPos.Y > 60.0f)
					{
						break;
					}
				}

				if (abs(XPos.X) >= YPos.Y)
				{
					while (RGB(136, 136, 136) != GetGroundColor(RGB(255, 255, 255), MovePos))
					{
						MovePos.Y += 1;
					}
				}

			}

			// 내가 움직이려는 
			// GetGroundColor(RGB(255, 255, 255), MovePos);

			AddPos(MovePos);
		}
	}

	if (true == GameEngineInput::IsUp('S'))
	{
		DirCheck();
		MainRenderer->SetRenderPos({ 0,0 });
		SwitchToBodyCollsion();
		ChanageState(PlayerState::Idle);

	}

	if (MovePos == float4::ZERO)
	{
		DirCheck();
		ChanageState(PlayerState::Lie);
	}
}

void Player::VictoryUpdate(float _Delta)
{
	if (true == MainRenderer->IsAnimationEnd())
	{
		SetVictoryEnd(true);
	}
}

void Player::DieUpdate(float _Delta)
{


	float4 _Pos = float4::ZERO;
	float _ColDir = 0.0f;

	if (Dir == PlayerDir::Left)
	{
		_ColDir = 1.0f;
	}
	else if (Dir == PlayerDir::Right)
	{
		_ColDir = -1.0f;
	}
	else
	{
		MsgBoxAssert("죽기에서 방향을 모르겠습니다.");
	}


	//마지막 빛 애니메이션 연출일땐 위치이동 안함
	if (MainRenderer->GetCurAnimationName() != "RaymanDieLight")
	{
		if (4 > MainRenderer->GetCurFrame())
		{
			_Pos = { _ColDir * 3.0f, -3.0f };
			AddPos(_Pos);
		}
		else if (9 > MainRenderer->GetCurFrame())
		{
			_Pos = { _ColDir * 1.0f , 1.0f };
			AddPos(_Pos);
		}
	}

	//마지막 빛 애니메이션으로 전환
	if (MainRenderer->GetCurAnimationName() != "RaymanDieLight" && 9 == MainRenderer->GetCurFrame())
	{
		SetCameraMovement(false);
		MainRenderer->ChangeAnimation("RaymanDieLight");
	}

	//게임패배상태로 전환
	if (MainRenderer->GetCurAnimationName() == "RaymanDieLight" && true == MainRenderer->IsAnimationEnd())
	{
		SetLoseGame(true);
	}
}

void Player::DebugmodeStart()
{
	ChangeAnimationState("Idle");
	DebugMode = true;
}

void Player::DebugmodeUpdate(float _Delta)
{

	DirCheck();

	float4 MovePos = float4::ZERO;

	if (true == GameEngineInput::IsPress('W') && true == GameEngineInput::IsPress('A') )
	{
		MovePos = { -DebugSpeed * _Delta, -DebugSpeed * _Delta };
	}
	else if (true == GameEngineInput::IsPress('W') && true == GameEngineInput::IsPress('D') )
	{
		MovePos = { DebugSpeed * _Delta, -DebugSpeed * _Delta };
	}
	else if (true == GameEngineInput::IsPress('S') && true == GameEngineInput::IsPress('A'))
	{
		MovePos = { -DebugSpeed * _Delta, DebugSpeed * _Delta };
	}
	else if (true == GameEngineInput::IsPress('S') && true == GameEngineInput::IsPress('D'))
	{
		MovePos = { DebugSpeed * _Delta, DebugSpeed * _Delta };
	}
	else if (true == GameEngineInput::IsPress('A') && Dir == PlayerDir::Left)
	{
		MovePos = { -DebugSpeed * _Delta, 0.0f };
	}
	else if (true == GameEngineInput::IsPress('D') && Dir == PlayerDir::Right)
	{
		MovePos = { DebugSpeed * _Delta, 0.0f };
	}
	else if (true == GameEngineInput::IsPress('W'))
	{
		MovePos = { 0.0f, -DebugSpeed * _Delta };
	}
	else if (true == GameEngineInput::IsPress('S'))
	{
		MovePos = { 0.0f, DebugSpeed * _Delta };
	}

	//이동속도 늘이고 줄이기
	if (true == GameEngineInput::IsDown('1'))
	{
		DebugSpeed += 100.0f;
	}
	else if (true == GameEngineInput::IsDown('2'))
	{
		DebugSpeed -= 100.0f;
	}
	if (DebugSpeed < 0)
	{
		DebugSpeed = 0;
	}

	//목숨 추가, 목숨줄이기
	if (true == GameEngineInput::IsDown('3'))
	{
		if (0 < GetTotalLife())
		{
			AddTotalLife(-1);
		}
	}
	if (true == GameEngineInput::IsDown('4'))
	{
		if (9 > GetTotalLife())
		{
			AddTotalLife(1);
		}
	}

	

	/*if (true == GameEngineInput::IsPress('W') && true == GameEngineInput::IsPress('A'))
	{
		MovePos = { -Speed * _Delta, -Speed * _Delta };
	}
	else if (true == GameEngineInput::IsPress('W') && true == GameEngineInput::IsPress('D'))
	{
		MovePos = { Speed * _Delta, -Speed * _Delta };
	}*/

	//디버그 모드 종료
	//if (true == GameEngineInput::IsDown('V'))
	//{
	//	// GameEngineWindow::MainWindow.AddDoubleBufferingCopyScaleRatio(-1.0f * _Delta);
	//	DebugMode = false;
	//	GravityReset();
	//	ChanageState(PlayerState::JumpHold);
	//}

	AddPos(MovePos);
	GetLevel()->GetMainCamera()->AddPos(MovePos);


}
