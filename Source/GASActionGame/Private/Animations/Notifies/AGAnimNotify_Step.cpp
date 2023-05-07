
#include "Animations/Notifies/AGAnimNotify_Step.h"
#include "GASActionGameCharacter.h"
#include "Components/AGFootstepsComponent.h"

void UAGAnimNotify_Step::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	check (MeshComp);
	
	if (const AGASActionGameCharacter* Character = MeshComp->GetOwner<AGASActionGameCharacter>())
	{
		if (const UAGFootstepsComponent* FootstepsComponent = Character->GetFootstepsComponent())
		{
			FootstepsComponent->HandleFootstep(Foot);
		}
	}
}
