#include "CMovingPlatform.h"
#include "Global.h"
#include "Materials/MaterialInstanceConstant.h"

ACMovingPlatform::ACMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	UStaticMesh* meshAsset;
	CHelpers::GetAsset<UStaticMesh>(&meshAsset, "StaticMesh'/Game/Geometry/Meshes/1M_Cube_Chamfer.1M_Cube_Chamfer'");
	GetStaticMeshComponent()->SetStaticMesh(meshAsset);
	GetStaticMeshComponent()->SetRelativeScale3D(FVector(1.f, 1.f, 0.2f));

	UMaterialInstanceConstant* materialAsset;
	CHelpers::GetAsset<UMaterialInstanceConstant>(&materialAsset, "/Game/Platforms/MAT_MovingPlatform_Inst");
	GetStaticMeshComponent()->SetMaterial(0, materialAsset);

	SetMobility(EComponentMobility::Movable);
}

void ACMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	//������ ��ġ���� ���ø�����Ʈ
	if (HasAuthority() == true)
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}

	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
}

void ACMovingPlatform::IncreaseActive()
{
	Active++;
}

void ACMovingPlatform::DecreaseActive()
{
	if (Active > 0)
		Active--;
}

void ACMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Active�� 1�̻��� ��쿡��
	CheckFalse(Active > 0);

	//���������� �̵�
	if (HasAuthority() == true)
	{
		FVector location = GetActorLocation();

		float totalDistance = (GlobalStartLocation - GlobalTargetLocation).Size();
		float currentDistance = (location - GlobalStartLocation).Size();

		if (currentDistance >= totalDistance)
		{
			FVector temp = GlobalStartLocation;
			GlobalStartLocation = GlobalTargetLocation;
			GlobalTargetLocation = temp;
		}

		FVector direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
		location += direction * Speed * DeltaTime;
		SetActorLocation(location);
	}
}

