//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ShooterProjectile.h"
#include "AutoRangedAttackComponent.generated.h"

//---------------------------------------------------------------------------------------------------------------------

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WORKSPACE_API UAutoRangedAttackComponent : public USceneComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	AActor* m_Owner{nullptr};

	UPROPERTY()
	TObjectPtr<USphereComponent> m_DetectionSphere{nullptr};

	UPROPERTY()
	TObjectPtr<APawn> m_Target{nullptr};

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> m_TurretMesh{nullptr};
	
	float m_TimeUntilNextShot{0.0f};

protected:
	virtual void BeginPlay() override;

    UFUNCTION()
    void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
						int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    
	UFUNCTION()
    void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
					  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void RotateTowardsTarget(float deltaTime);
	bool IsTargetAligned() const;
	bool HasLineOfSightToTarget() const;
	void FireProjectile();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float DetectionRange{200.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection",
			  meta = (ClampMin = "0.0", ClampMax = "360.0", UIMin = "0.0", UIMax = "360.0"))
	float RotationSpeed{90.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float FireRate{0.5f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TSubclassOf<AShooterProjectile> ProjectileClass{nullptr};

public:	
	UAutoRangedAttackComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;		

	// Must be called after changing DetectionRange to update the sphere radius.
	UFUNCTION(BlueprintCallable, Category = "Detection")
	void UpdateDetectionRange();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void SetTurretMesh(UStaticMeshComponent* mesh);
};

//---------------------------------------------------------------------------------------------------------------------
