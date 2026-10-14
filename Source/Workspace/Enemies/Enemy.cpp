//---------------------------------------------------------------------------------------------------------------------

#include "Enemies/Enemy.h"

//---------------------------------------------------------------------------------------------------------------------
// Sets default values

AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

//---------------------------------------------------------------------------------------------------------------------
// Called when the game starts or when spawned

void AEnemy::BeginPlay()
{
	Super::BeginPlay();	
}

//---------------------------------------------------------------------------------------------------------------------
// Called every frame

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//---------------------------------------------------------------------------------------------------------------------
// Set the base parameters

void AEnemy::SetParams(int32 _health, int32 _armour, int32 _damage, int32 _experience, FString _name)
{
	Health = _health;
	Armour = _armour;
	Damage = _damage;

	if (_experience != 0)
		Experience = _experience;
	else
		CalculateExperience();

	Name = _name;
}

//---------------------------------------------------------------------------------------------------------------------
// Determining the base parameters based on the default values or values specified in the class or by Unreal Engine

void AEnemy::SetRandomParams()
{
	std::uniform_int_distribution<int32> distHealth(1, Health), distArmour(0, Armour), distDamage(0, Damage);

	Health = distHealth(m_generator);
	Armour = distArmour(m_generator);
	Damage = distDamage(m_generator);
}

//---------------------------------------------------------------------------------------------------------------------
// Determine the experience gained for defeating an enemy.
// The "factor" parameter allows the value to be adjusted based on the enemy's level, etc.

void AEnemy::CalculateExperience(float factor)
{
	Experience = ceil(factor * (float)((3 * Damage) + (2 * Health) + Armour));
}

//---------------------------------------------------------------------------------------------------------------------
