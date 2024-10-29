#pragma once


UENUM(BlueprintType)
enum class ECharectarState :uint8
{
	ECS_Unequipped UMETA(DisplayName = "UnEquipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One Handed Weapon"),
	ECS_EquippedRwoHandedWeapon UMETA(DisplayName = "Equipped Two Handed Weapon"),	
};

UENUM(BlueprintType)
enum class EActionState :uint8
{
	EAS_UnOccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_ArmDisarm UMETA(DisplayName = "Arm Disarm"),
};

