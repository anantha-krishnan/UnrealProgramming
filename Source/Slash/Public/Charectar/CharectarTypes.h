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

UENUM(BlueprintType)
enum class EDeathPose :uint8
{
	EDP_Alive UMETA(DisplayName = "Alive"),
	EDP_Dead1 UMETA(DisplayName = "Dead1"),
	EDP_Dead2 UMETA(DisplayName = "Dead2"),
	EDP_Dead3 UMETA(DisplayName = "Dead3"),
	EDP_Dead4 UMETA(DisplayName = "Dead4"),
	EDP_Dead5 UMETA(DisplayName = "Dead5"),
	EDP_Dead6 UMETA(DisplayName = "Dead6"),
};

