﻿// This file is covered by the LICENSE file in the root of this plugin.

#pragma once

#include "CoreMinimal.h"
#include "VRBPDatatypes.h"
#include "Grippables/GrippableActor.h"
#include "Engine/StaticMeshActor.h"
#include "TVRWeaponAttachment.generated.h"

UENUM(BlueprintType)
enum class ERailType : uint8
{
	Picatinny,
	MLok UMETA(DisplayName="M-LOK"),
	Keymod,
	Basis,
	Custom
};

UCLASS()
class TACTICALVRCORE_API ATVRWeaponAttachment : public AActor
{
	GENERATED_BODY()

	UPROPERTY(Category = "Weapon Attachment", BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;
	
	UPROPERTY(Category = "Weapon Attachment", BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootScene;

	
public:
	static FName StaticMeshComponentName;

public:
	// Sets default values for this actor's properties
	ATVRWeaponAttachment(const FObjectInitializer& OI);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void FindAttachPointAndAttach();
	
	UFUNCTION(Category = "Weapon Attachment", BlueprintCallable)
    virtual void AttachToWeapon(class UTVRAttachmentPoint* AttachPoint);

	UFUNCTION(Category= "Weapon Attachment", BlueprintCallable)
    virtual class ATVRGunBase* GetGunOwner() const;

	class UStaticMeshComponent* GetStaticMeshComponent() const { return Mesh;}

	virtual void SetCollisionProfile(FName NewProfile);
	
	virtual bool GetGripSlot(
		const FVector& WorldLocation,
		class UGripMotionControllerComponent* CallingController,
		FTransform& OutTransform,
		FName& OutSlotName
	) const {return false;}

	virtual FName GetPrefixedSocketName(USceneComponent* SocketComp) const;

	UFUNCTION(Category = "Weapon Attachment", BlueprintCallable)
	void SetVariant(uint8 Variant);
	UFUNCTION(Category = "Weapon Attachment", BlueprintImplementableEvent)
	void OnVariantChanged(uint8 Variant);

	/**
	 * Allows to pick another class to replace this attachment altogether for different rail types.
	 * This can be useful in case for example the picatinny and mlok variant have differences are too
	 * complex to handle in the OnRailTypeChanged function. It will be a class replacement, so stats would also
	 * be changed. Good practice in order to retain the stats of the original is to make the replacement a child
	 * @param RailType Type of rail usually of the attachment point
	 * @param CustomType Additionl byte to implement Custom Types or other functionality.
	 *
	 * @returns A WeaponAttachment class that should replace this one under the given arguments.
	 */
	UFUNCTION(Category = "Weapon Attachment", BlueprintNativeEvent)
	TSubclassOf<ATVRWeaponAttachment> GetReplacementClass(ERailType RailType, uint8 CustomType = 0) const;
	virtual TSubclassOf<ATVRWeaponAttachment> GetReplacementClass_Implementation(ERailType RailType, uint8 CustomType = 0) const;
	
	UFUNCTION(Category = "Weapon Attachment", BlueprintCallable)
	void SetRailType(ERailType RailType, uint8 CustomType = 0);
	UFUNCTION(Category = "Weapon Attachment", BlueprintImplementableEvent)
	void OnRailTypeChanged(ERailType RailType, uint8 CustomType = 0);

	UFUNCTION(Category = "Weapon Attachment", BlueprintCallable)
	const FText& GetWeaponAttachmentName() const {return WeaponAttachmentName;} 
	
protected:
	TSubclassOf<UStaticMeshComponent> StaticMeshClass;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOwnerGripped(class UGripMotionControllerComponent* GrippingHand, const struct FBPActorGripInformation& GripInfo);
	virtual void OnOwnerGripped_Implementation(class UGripMotionControllerComponent* GrippingHand, const struct FBPActorGripInformation& GripInfo) {}
	UFUNCTION()
	void OnOwnerDropped(class UGripMotionControllerComponent* GrippingHand, const struct FBPActorGripInformation& GripInfo, bool bSocketed);
	virtual void OnOwnerDropped_Implementation(class UGripMotionControllerComponent* GrippingHand, const struct FBPActorGripInformation& GripInfo, bool bSocketed) {}

protected:
	UPROPERTY()
	TArray<class UStaticMeshComponent*> AttachmentMeshes;

	UPROPERTY()
	class UTVRAttachmentPoint* AttachmentPoint;
	
	UPROPERTY(Category="Weapon Attachment", BlueprintReadOnly)
	uint8 SelectedVariant;
	
	UPROPERTY(Category="Weapon Attachment", EditDefaultsOnly)
	FText WeaponAttachmentName;
	
};
