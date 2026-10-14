//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include <random>
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

//---------------------------------------------------------------------------------------------------------------------

UCLASS()
class WORKSPACE_API AEnemy : public AActor
{
	GENERATED_BODY()

protected:
	std::mt19937 m_generator{std::random_device{}()};
 
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy")
	int32 Experience{1};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy")
	FString Name{TEXT("Enemy")};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy")
	int32 Health{10};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy")
	int32 Armour{3};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy")
	int32 Damage{5};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	AEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void SetParams(int32 _health, int32 _armour, int32 _damage, int32 _experience = 0, FString _name = TEXT("Enemy"));

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void SetRandomParams();

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void CalculateExperience(float factor = 1.0f);
};

//---------------------------------------------------------------------------------------------------------------------
