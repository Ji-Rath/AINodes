#include "SoldierAI.h"

DEFINE_LOG_CATEGORY(SoldierAI);

#define LOCTEXT_NAMESPACE "FSoldierAI"

void FSoldierAI::StartupModule()
{
	UE_LOG(SoldierAI, Warning, TEXT("SoldierAI module has been loaded"));
}

void FSoldierAI::ShutdownModule()
{
	UE_LOG(SoldierAI, Warning, TEXT("SoldierAI module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSoldierAI, SoldierAI)