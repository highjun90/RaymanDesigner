#include "GoalPoint.h"
#include <GameEngineCore/ResourcesManager.h>
#include <GameEngineCore/GameEngineRenderer.h>

#include "ContentsEnum.h"

GoalPoint* GoalPoint::MainGoalPoint = nullptr;
bool GoalPoint::GoalPointIsRender = true;
GameEngineCollision* GoalPoint::GoalPointCollision = nullptr;

GoalPoint::GoalPoint()
{
}

GoalPoint::~GoalPoint()
{
}

void GoalPoint::Update(float _Delta)
{

	//�÷��̾ Ű�����鼭 �������ٲ����ְ� �س�
	bool CheckGoalPointUpdate = GoalPoint::GetGoalPointIsRender();
	if (CheckGoalPointUpdate == true)
	{
		GoalRenderer->On();
	}
	else if(CheckGoalPointUpdate == false)
	{
		GoalRenderer->Off();
	}
}

void GoalPoint::Start()
{
	float4 Pos = GetGoalPointPos();
	SetPos(Pos);

	if (false == ResourcesManager::GetInst().IsLoadTexture("GoalPoint.bmp"))
	{
		GameEnginePath FilePath;
		FilePath.SetCurrentPath();
		FilePath.MoveParentToExistsChild("ContentsResources");
		FilePath.MoveChild("ContentsResources\\Texture\\Player\\");
		ResourcesManager::GetInst().TextureLoad(FilePath.PlusFilePath("GoalPoint.bmp"));
	}

	{
		GoalRenderer = CreateRenderer("GoalPoint.bmp", RenderOrder::Play);
	}

	//��ó���� ����, �浹����
	GoalRenderer->On();

	GoalPointCollision = CreateCollision(CollisionOrder::GoalPoint);
	GoalPointCollision->SetCollisionScale({ 10, 2 });
	GoalPointCollision->SetCollisionType(CollisionType::Rect);

	GoalPointCollision->On();
}