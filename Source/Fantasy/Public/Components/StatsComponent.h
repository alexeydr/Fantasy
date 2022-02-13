#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatsComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamage, float, Damage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoneyChanged, float, NewMoney);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FANTASY_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UStatsComponent();

	UFUNCTION(BlueprintCallable)
	void DoDamage(float Damage);

	UFUNCTION(BlueprintCallable)
	void AddHealth(float Param);

	UFUNCTION(BlueprintCallable)
	void AddMoney(float Param);

	UFUNCTION(BlueprintCallable)
	void SubstractMoney(float Param);

	UFUNCTION(BlueprintPure)
	float GetHealth() const { return Health; };

	UFUNCTION(BlueprintPure)
	float GetMoney() const { return Money; };

	bool CheckCanBuyItem(float Price)
	{
		return (Money - Price >= 0);
	};

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly)
	FOnHealthChanged OnHealthChangedDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly)
	FOnMoneyChanged OnMoneyChanged;

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly)
	FOnDamage OnDamageDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly)
	FOnDeath OnDeathDelegate;

protected:

	bool CheckCanLife();

	void Dying();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
	float Health = 100.f;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "10000", UIMin = "0", UIMax = "10000"))
	float Money = 100.f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
