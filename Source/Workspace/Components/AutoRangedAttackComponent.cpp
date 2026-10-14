//---------------------------------------------------------------------------------------------------------------------

#include "Components/AutoRangedAttackComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

//---------------------------------------------------------------------------------------------------------------------

UAutoRangedAttackComponent::UAutoRangedAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAutoRangedAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	m_Owner = GetOwner();

	if (IsValid(m_Owner))
	{
		m_DetectionSphere = NewObject<USphereComponent>(m_Owner, TEXT("DetectionSphere"));
		if (m_DetectionSphere)
		{
			m_Owner->AddInstanceComponent(m_DetectionSphere);
			m_DetectionSphere->SetupAttachment(this);

			m_DetectionSphere->SetSphereRadius(FMath::Max(0.0f, DetectionRange));

			m_DetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			m_DetectionSphere->SetCollisionObjectType(ECC_WorldDynamic);
			m_DetectionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
			m_DetectionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
			m_DetectionSphere->SetGenerateOverlapEvents(true);

			m_DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &UAutoRangedAttackComponent::OnBeginOverlap);
			m_DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &UAutoRangedAttackComponent::OnEndOverlap);

			m_DetectionSphere->RegisterComponent();
		}
	}
}

void UAutoRangedAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType,
											   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_TimeUntilNextShot > 0)
		m_TimeUntilNextShot -= DeltaTime;

	if (IsValid(m_Target))
	{
		if (!IsTargetAligned())
			RotateTowardsTarget(DeltaTime);
		else if ((m_TimeUntilNextShot <= 0.0f) && (FireRate > 0.0f) && HasLineOfSightToTarget())
		{
			FireProjectile();
			m_TimeUntilNextShot = 1.0f / FireRate;
		}
	}
}

void UAutoRangedAttackComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
												UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
												bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (IsValid(OtherActor) && IsValid(PlayerPawn) && (OtherActor == PlayerPawn))
	{
		m_Target = PlayerPawn;
		//UE_LOG(LogTemp, Log, TEXT("AWT: Player entered detection range: %s"), *OtherActor->GetName());		
	}
}

void UAutoRangedAttackComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
											  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(OtherActor) && IsValid(m_Target) && (OtherActor == m_Target))
	{
		m_Target = nullptr;
		//UE_LOG(LogTemp, Log, TEXT("AWT: Player left detection range: %s"), *OtherActor->GetName());
	}
}

void UAutoRangedAttackComponent::UpdateDetectionRange()
{
	DetectionRange = FMath::Max(0.0f, DetectionRange);
	if (m_DetectionSphere)
		m_DetectionSphere->SetSphereRadius(DetectionRange);
}

void UAutoRangedAttackComponent::SetTurretMesh(UStaticMeshComponent* mesh)
{
	if (IsValid(mesh))
		m_TurretMesh = mesh;
	else
	{
		m_TurretMesh = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("AWT: TurretMesh is not valid"));
	}	
}

void UAutoRangedAttackComponent::RotateTowardsTarget(float deltaTime)
{
	if (IsValid(m_TurretMesh) && IsValid(m_Target))
	{
		const FRotator currentRotation = m_TurretMesh->GetComponentRotation();
		const FVector directionToTarget = m_Target->GetActorLocation() - m_TurretMesh->GetComponentLocation();
		const FRotator rotationToTarget = directionToTarget.Rotation();
		const FRotator rotation = FMath::RInterpConstantTo(currentRotation, rotationToTarget, deltaTime, RotationSpeed);
		m_TurretMesh->SetWorldRotation(rotation);
	}
}

bool UAutoRangedAttackComponent::IsTargetAligned() const
{
	bool isAligned = false;

	if (IsValid(m_TurretMesh) && IsValid(m_Target))
	{
		const FRotator currentRotation = m_TurretMesh->GetComponentRotation();
		const FVector directionToTarget = m_Target->GetActorLocation() - m_TurretMesh->GetComponentLocation();
		const FRotator rotationToTarget = directionToTarget.Rotation();

		isAligned = currentRotation.Equals(rotationToTarget, 2.0f);
	}

	return isAligned;
}

bool UAutoRangedAttackComponent::HasLineOfSightToTarget() const
{
	bool hasLineOfSight = false;

	if (IsValid(m_Owner) && IsValid(m_Target) && IsValid(m_TurretMesh))
	{
		const FVector start = m_TurretMesh->GetComponentLocation();
		const FVector end = m_Target->GetActorLocation();

		FHitResult hitResult;
		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(m_Owner);

		const bool hasHit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, queryParams);

		hasLineOfSight = hasHit && (hitResult.GetActor() == m_Target);

		DrawDebugLine(GetWorld(), start, end, hasLineOfSight ? FColor::Emerald : FColor::Blue, false, 0.3f, 0, 2.0f);
	}

	return hasLineOfSight;
}

void UAutoRangedAttackComponent::FireProjectile()
{
	if (IsValid(m_Owner) && IsValid(m_Target) && IsValid(ProjectileClass) && IsValid(m_TurretMesh))
	{
		UE_LOG(LogTemp, Log, TEXT("AWT: FIRE!"));

		const FVector location = m_TurretMesh->GetComponentLocation() + m_TurretMesh->GetForwardVector() * 20.0f;
		const FRotator rotation = (m_Target->GetActorLocation() - location).Rotation();
		FActorSpawnParameters parameters;
		parameters.Owner = m_Owner;
		parameters.Instigator = Cast<APawn>(m_Owner);

		GetWorld()->SpawnActor<AActor>(ProjectileClass, location, rotation, parameters);
	}
	else
	{
		if (!IsValid(m_Owner))
			UE_LOG(LogTemp, Warning, TEXT("AWT: MISFIRE Owner is not valid"));
		if (!IsValid(m_Target))
			UE_LOG(LogTemp, Warning, TEXT("AWT: MISFIRE Target is not valid"));
		if (!IsValid(ProjectileClass))
			UE_LOG(LogTemp, Warning, TEXT("AWT: MISFIRE ProjectileClass is not valid"));
		if (!IsValid(m_TurretMesh))
			UE_LOG(LogTemp, Warning, TEXT("AWT: MISFIRE TurretMesh is not valid"));
	}
}

//---------------------------------------------------------------------------------------------------------------------
