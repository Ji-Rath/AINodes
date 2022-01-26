#include "BasicNodes.h"

DEFINE_LOG_CATEGORY(BasicNodes);

#define LOCTEXT_NAMESPACE "FBasicNodes"

void FBasicNodes::StartupModule()
{
	UE_LOG(BasicNodes, Warning, TEXT("BasicNodes module has been loaded"));
}

void FBasicNodes::ShutdownModule()
{
	UE_LOG(BasicNodes, Warning, TEXT("BasicNodes module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FBasicNodes, BasicNodes)