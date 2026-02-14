#include "QuicksandVolume.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AQuicksandVolume::AQuicksandVolume()
{
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("QuicksandBox"));
	SetRootComponent(Box);

	// Vehicles often aren't Pawn channel. Overlap everything is simplest + reliable for this case.
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box->SetCollisionObjectType(ECC_WorldDynamic);
	Box->SetCollisionResponseToAllChannels(ECR_Overlap);
	Box->SetGenerateOverlapEvents(true);

	Box->InitBoxExtent(FVector(200.f, 200.f, 100.f));
}

void AQuicksandVolume::BeginPlay()
{
	Super::BeginPlay();

	Box->OnComponentBeginOverlap.AddDynamic(this, &AQuicksandVolume::OnBoxBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &AQuicksandVolume::OnBoxEndOverlap);
}

void AQuicksandVolume::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	for (auto It = Victims.CreateIterator(); It; ++It)
	{
		AActor* Victim = It.Key().Get();
		if (!Victim)
		{
			It.RemoveCurrent();
			continue;
		}

		FQuicksandVictimState& State = It.Value();
		if (State.bKilled)
			continue;

		ApplyQuicksandToVictim(Victim, State, DeltaSeconds);
	}
}

void AQuicksandVolume::OnBoxBeginOverlap(UPrimitiveComponent* Overlapped, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Sweep)
{
	if (!OtherActor || OtherActor == this)
		return;

	UPrimitiveComponent* RootPrim = GetRootPrimitive(OtherActor);
	if (!RootPrim)
		return;

	if (Victims.Contains(OtherActor))
		return;

	FQuicksandVictimState State;
	State.RootPrim = RootPrim;
	State.Sink01 = 0.f;
	State.bKilled = false;

	// capture original damping and then override
	State.OriginalLinearDamping = RootPrim->GetLinearDamping();
	State.OriginalAngularDamping = RootPrim->GetAngularDamping();
	State.bHasOriginalDamping = true;

	RootPrim->SetLinearDamping(LinearDampingInSand);
	RootPrim->SetAngularDamping(AngularDampingInSand);

	Victims.Add(OtherActor, State);
}

void AQuicksandVolume::OnBoxEndOverlap(UPrimitiveComponent* Overlapped, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor)
		return;

	FQuicksandVictimState* State = Victims.Find(OtherActor);
	if (!State)
		return;

	RestoreIfLeaving(*State);
	Victims.Remove(OtherActor);
}

UPrimitiveComponent* AQuicksandVolume::GetRootPrimitive(AActor* Actor)
{
	if (!Actor) return nullptr;

	if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(Actor->GetRootComponent()))
		return Prim;

	TArray<UPrimitiveComponent*> Prims;
	Actor->GetComponents<UPrimitiveComponent>(Prims);

	for (UPrimitiveComponent* P : Prims)
	{
		if (P && P->IsSimulatingPhysics())
			return P;
	}

	return nullptr;
}

void AQuicksandVolume::ApplyQuicksandToVictim(AActor* Victim, FQuicksandVictimState& State, float Dt)
{
	UPrimitiveComponent* RootPrim = State.RootPrim.Get();
	if (!RootPrim)
		return;

	// --- 1) Nonlinear drag (quadratic, feels “exponential”) ---
	const FVector V = RootPrim->GetComponentVelocity();
	const float Speed = V.Size();

	if (Speed > 1.f)
	{
		const float SinkMult = 1.f + (State.Sink01 * DragBySinkMultiplier);
		const FVector DragForce = -V.GetSafeNormal() * (DragK * SinkMult * Speed * Speed);

		// AccelChange=true => mass independent (more consistent between pawn types)
		RootPrim->AddForce(DragForce, NAME_None, true);
	}

	// --- 2) Sink progression + downward pull (NO TELEPORTING) ---
	State.Sink01 = FMath::Clamp(State.Sink01 + SinkRate01PerSec * Dt, 0.f, 1.f);

	const float ForceMult = 0.25f + 0.75f * State.Sink01;
	RootPrim->AddForce(FVector(0, 0, -DownForceStrength * ForceMult), NAME_None, true);

	// Anti-yeet: cap upward speed while inside the box
	FVector Vel = RootPrim->GetComponentVelocity();
	if (Vel.Z > MaxUpwardSpeedInSand)
	{
		Vel.Z = MaxUpwardSpeedInSand;
		RootPrim->SetPhysicsLinearVelocity(Vel, false);
	}

	// --- 3) Kill check (sink-based, deterministic) ---
	if (State.Sink01 >= KillSink01)
	{
		float MaxHealth = 100.f;

		if (Victim->GetClass()->ImplementsInterface(UQuicksandHealthProvider::StaticClass()))
		{
			MaxHealth = IQuicksandHealthProvider::Execute_QS_GetMaxHealth(Victim);
			if (MaxHealth <= 0.f) MaxHealth = 100.f;
		}

		UGameplayStatics::ApplyDamage(
			Victim,
			MaxHealth,
			nullptr,
			this,
			DamageType ? *DamageType : UDamageType::StaticClass()
		);

		State.bKilled = true;
	}
}

void AQuicksandVolume::RestoreIfLeaving(FQuicksandVictimState& State)
{
	if (UPrimitiveComponent* RootPrim = State.RootPrim.Get())
	{
		if (State.bHasOriginalDamping)
		{
			RootPrim->SetLinearDamping(State.OriginalLinearDamping);
			RootPrim->SetAngularDamping(State.OriginalAngularDamping);
		}
	}
}
