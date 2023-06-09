#pragma once
#include "PlayActor.h"
#include <GameEnginePlatform/GameEngineSound.h>

#include "DarkRayman.h"

enum class PlayerState 
{
	Idle,
	Run,
	Jump,
	JumpHold,
	Sprint,
	SprintJump,
	Lie,
	LieMove,
	Victory,
	Die,
	Debugmode,
	Max, // 일반적으로 사용하지 않는 값.
};

enum class PlayerDir
{
	Right,
	Left,
	Max,
};

// 설명 :
class Player : public PlayActor
{
private:
	static Player* MainPlayer;

public:
	static Player* GetMainPlayer() 
	{
		return MainPlayer;
	}


public:
	// constrcuter destructer
	Player();
	~Player();

	// delete Function
	Player(const Player& _Other) = delete;
	Player(Player&& _Other) noexcept = delete;
	Player& operator=(const Player& _Other) = delete;
	Player& operator=(Player&& _Other) noexcept = delete;

	GameEngineRenderer* MainRenderer = nullptr;
	

	void SetJumpSpeed(float _Speed)
	{
		JumpSpeed = _Speed;
	}

	float GetJumpSpeed()
	{
		return JumpSpeed;
	}

	void Restart();
	

	float DebugSpeed = 300.0f;
	bool DebugMode = false;
	bool DebugModeText = false;
	float4 DebugStartPoint = float4::ZERO;
	bool DebugPointIsRender = false;
	bool DebugRaymanCollision = true;

	void SetTotalLife(int _Life)
	{
		TotalLife = _Life;
	}
	int GetTotalLife() const
	{
		return TotalLife;
	}
	void AddTotalLife(int _Life)
	{
		TotalLife += _Life;
	}

	void SetLoseGame(bool _bool)
	{
		LoseGame = _bool;
	}
	bool GetLoseGame() const
	{
		return LoseGame;
	}

	void SetDir(PlayerDir _Dir)
	{
		Dir = _Dir;
	}
	PlayerDir GetDir()
	{
		return Dir;
	}

	float4 GetLieRenderPoint()
	{
		return LieRenderPoint;
	}

	void SetVictoryEnd(bool _bool)
	{
		VictoryEnd = _bool;
	}
	bool GetVictoryEnd()
	{
		return VictoryEnd;
	}

	void SwitchToLieCollision();
	void SwitchToBodyCollsion();

	


	PlayerState State = PlayerState::Max;
	PlayerDir Dir = PlayerDir::Right;
	std::string CurState = "";
protected:
	void StateUpdate(float _Delta);

	void IdleStart();
	void IdleUpdate(float _Delta);


	void RunStart();
	void RunUpdate(float _Delta);

	void SprintStart();
	void SprintUpdate(float _Delta);

	void SprintJumpStart();
	void SprintJumpUpdate(float _Delta);

	void JumpStart();
	void JumpUpdate(float _Delta);

	void JumpHoldStart();
	void JumpHoldUpdate(float _Delta);

	void DebugmodeStart();
	void DebugmodeUpdate(float _Delta);

	void VictoryStart();
	void VictoryUpdate(float _Delta);

	void DieStart();
	void DieUpdate(float _Delta);

	void LieStart();
	void LieUpdate(float _Delta);

	void LieMoveStart();
	void LieMoveUpdate(float _Delta);

	void ChanageState(PlayerState State);

	
	//GameEngineRenderer* HPRender;

	int TestValue = 0;

	GameEngineCollision* BodyCollsion = nullptr;
	GameEngineCollision* LieCollsion = nullptr;


	void DirCheck();

	void ChangeAnimationState(const std::string& _StateName);


private:
	void LevelStart() override; 

	void Start() override;
	void Update(float _Delta) override;
	void Render(float _Delta) override;


	
	float JumpSpeed = 300.0f;
	float4 LieRenderPoint = { 0, 60 };

	int TotalLife = 5;
	bool LoseGame = false;  //목숨1번 잃었을때
	bool VictoryEnd = false;//표지판까지 도달했을때
	//bool VictoryGame = false;



	GameEngineSoundPlayer* BGMPlayerToPlayer = nullptr;  //PlayLevel에서 BGMPlayer 가져올 포인터. Player_State 디버그 모드때 이용.
	bool SoundPlaying = true;

	////////////////////// DebugValue
	float4 LeftCheck = { -50.0f, -50.0f };
	float4 RightCheck = { 50.0f, -50.0f };
	float4 DownCheck = { 0, 90.0f };

	float4 LeftCheck_Lie = { -50.0f, 20.0f };
	float4 RightCheck_Lie = { 50.0f, 20.0f };
};

