// Fill out your copyright notice in the Description page of Project Settings.


#include "HoleTriggerSphere.h"
#include "GolfBallProjectile.h"
#include "Components/SphereComponent.h"

// Sets default values
AHoleTriggerSphere::AHoleTriggerSphere()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create the trigger sphere component
    TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
    TriggerSphere->InitSphereRadius(6.0f);
    RootComponent = TriggerSphere;

    // Bind the overlap event function
    TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &AHoleTriggerSphere::OnSphereOverlap);
}

// Called when the game starts or when spawned
void AHoleTriggerSphere::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHoleTriggerSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHoleTriggerSphere::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (HasAuthority())
    {
        AGolfBallProjectile* GolfBallProjectile = Cast<AGolfBallProjectile>(OtherActor);

        if (GolfBallProjectile != nullptr)
        {
            GolfBallProjectile->OnHoleBeginOverlap();
        }
    }
}
