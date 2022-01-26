#include "AnimLibrary.h"
#include "Kismet/KismetMathLibrary.h"



FRotator UAnimLibrary::SmoothLookAt(const AActor* Actor, const FRotator CurrentRotation, const FRotator RotationTarget, const float DeltaSeconds, const float RotationLimit /*= 0.f*/, const float InterpSpeed /*= 3.f*/)
{
	FRotator ActorRotation = Actor->GetActorRotation();
	FRotator DesiredRotation = UKismetMathLibrary::NormalizedDeltaRotator(RotationTarget, ActorRotation);
	
	if (RotationLimit > 0)
	{
		DesiredRotation.Yaw = FMath::ClampAngle(DesiredRotation.Yaw, -RotationLimit, RotationLimit);
		DesiredRotation.Roll = FMath::ClampAngle(DesiredRotation.Roll, -RotationLimit, RotationLimit);
		DesiredRotation.Pitch = FMath::ClampAngle(DesiredRotation.Pitch, -RotationLimit, RotationLimit);
	}

	return FMath::RInterpTo(CurrentRotation, DesiredRotation, DeltaSeconds, InterpSpeed);
}
