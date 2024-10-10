#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include "Runtime/Core/Public/HAL/ThreadSafeBool.h"
#include "ABoidComponent.generated.h"

USTRUCT(BlueprintType)
struct FlockMemberData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    int instanceIndex = 0;

    UPROPERTY()
    FVector velocity = FVector::ZeroVector;

    UPROPERTY()
    FVector wanderPosition = FVector::ZeroVector;

    UPROPERTY()
    FTransform transform;

    UPROPERTY()
    float elapsedTimeSinceLastWander = 0.f;

    UPROPERTY()
    bool bIsFlockLeader = false;

    UPROPERTY()
    TArray<AActor*> attackedActors;

    FlockMemberData()
    {
        instanceIndex = 0;
        velocity = FVector::ZeroVector;
        elapsedTimeSinceLastWander = 0.0f;
        wanderPosition = FVector::ZeroVector;
        bIsFlockLeader = false;
    };
};

USTRUCT(BlueprintType)
struct FlockMembersArrays
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    int instanceIndex = 0;

    UPROPERTY()
    TArray<FlockMemberData> flockMembersArr;

    FlockMembersArrays()
    {
        instanceIndex = 0;
        flockMembersArr.Empty();
    };

    void AddData(int index, TArray<FlockMemberData> newFlockData)
    {
        instanceIndex = index;
        flockMembersArr = newFlockData;
    };
};

UENUM(BlueprintType)
enum class EPriority : uint8
{
    Normal UMETA(DisplayName = "Normal"),
    AboveNormal UMETA(DisplayName = "AboveNormal"),
    BelowNormal UMETA(DisplayName = "BelowNormal"),
    Highest UMETA(DisplayName = "Highest"),
    Lowest UMETA(DisplayName = "Lowest"),
    SlightlyBelowNormal UMETA(DisplayName = "SlightlyBelowNormal"),
    TimeCritical UMETA(DisplayName = "TimeCritical")
};

USTRUCT(BlueprintType)
struct FlockMemberParameters
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Spawn")
    EPriority threadPriority = EPriority::Normal;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Spawn")
    bool bUseOneLeader = false;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Spawn")
    AActor* followActor = nullptr;
    // Use for only more flock mates!
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Spawn")
    bool ExperimentalOptimization = false;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Spawn")
    bool bUseAquarium = true;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Spawn")
    float flockWanderInRandomRadius = 3000.0f;
    // If you want add all not mobile actors for avoidance. 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced Flock Parameters")
    bool bAutoAddComponentsInArray = true;
    // If need react on pawn.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced Flock Parameters")
    bool bReactOnPawn = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced Flock Parameters")
    bool bFollowToPawn = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced Flock Parameters")
    bool bCanAttackPawn = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced Flock Parameters")
    float attackRadius = 100.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced Fly Insects Parameters")
    float damageValue = 0.001f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced Fly Insects Parameters")
    TSubclassOf<class UDamageType> damageType;

    UPROPERTY()
    float attackRadiusSquared = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Spawn")
    bool bUseMaxHeight = false;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Spawn")
    float maxHeight = 0.f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Spawn")
    float moveSpeedInterpInThread = 5.f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Parameters")
    float flockMaxSpeed = 40.0f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Parameters")
    float flockOffsetSpeed = 5.0f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Parameters")
    float escapeMaxSpeedMultiply = 2.0f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Parameters")
    float followScale = 1.0f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Parameters")
    float flockMateAwarenessRadius = 400.0f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Parameters")
    float alignScale = 0.4f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Parameters")
    float cohesionScale = 0.6f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Parameters")
    float separationScale = 5.0f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Parameters")
    float fleeScale = 10.0f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Parameters")
    float fleeScaleAvoidance = 10.0f;

    float avoidancePrimitiveDistance = 50.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Parameters")
    float separationRadius = 6.0f;
    // Delay before leader find new wander location.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Parameters")
    float flockWanderUpdateRate = 2.0f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Parameters")
    float flockMinWanderDistance = 50.0f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Parameters")
    float flockMateRotationRate = 0.6f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Parameters")
    float escapeMateRotationRate = 10.f;

    float flockMaxSteeringForce = 100.0f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Parameters")
    float flockEnemyAwarenessRadius = 200.0f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Parameters")
    float followPawnAwarenessRadius = 500.0f;

    float fleeScaleAquarium = 10.0f;
    float strengthAquariumOffsetValue = 100.f;


    FlockMemberParameters()
    {
        attackRadiusSquared = FMath::Square(attackRadius);
    };
};

UCLASS()
class ADVANCEDBOIDSYSTEM_API ABoidComponent : public UActorComponent
{
    GENERATED_BODY()

public:


    // constructor
    ABoidComponent();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    //************************************************************************
    // Component                                                                  
    //************************************************************************

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
    class USphereComponent* SphereComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
    class UBoxComponent* BoxComponent;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
    class UInstancedStaticMeshComponent* StaticMeshInstanceComponent;

    //************************************************************************

    void GenerateFlockThread();

    void AddAvoidanceComponentsTimer();

    // Add an instance to this component. Transform is given in world space. 
    void AddFlockMemberWorldSpace(const FTransform& WorldTransform);

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Spawn")
    UStaticMesh* StaticMesh;
    // Recommended - 1 Thread = (2000 - 2500) mates. 
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Spawn", meta = (ClampMin = "1", ClampMax = "32"))
    int maxUseThreads = 1;
    // Recommended - 1 Thread = (2000 - 2500) mates.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Spawn")
    int32 flockMateInstances = 1000;
    // Random mesh scale.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Spawn")
    float minMeshScale = 1.f;
    // Random mesh scale.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Spawn")
    float maxMeshScale = 2.f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Parameters")
    float interpMoveAnimRate = 200.f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Parameters")
    float interpRotateAnimRate = 25.f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced Flock Parameters")
    FlockMemberParameters flockParameters;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced Flock Parameters")
    TArray<AActor*> avoidanceActorRootArr;
    UPROPERTY(BlueprintReadOnly, Category = "Advanced Flock Parameters")
    TArray<FlockMemberData> mFlockMemberData;


    int32 numFlock;

    UPROPERTY()
    TArray<UPrimitiveComponent*> allOverlappingComponentsArr;

    TArray<FlockMembersArrays> allFlockMembersArrays;

    void DivideFlockArrayForThreads();

    UPROPERTY()
    TArray<AActor*> dangerActors;

protected:

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    virtual void BeginDestroy() override;

    FTimerHandle addAvoidanceActor_Timer;

private:

    TArray<class FlockThread*> FlockActorPoolThreadArr;

};

// Thread
class FlockThread : public FRunnable
{
public:

    //================================= THREAD =====================================

    //Constructor
    FlockThread(AActor* newActor);
    //Destructor
    virtual ~FlockThread() override;

    //Use this method to kill the thread!!
    void EnsureCompletion();
    //Pause the thread 
    void PauseThread();
    //Continue/UnPause the thread
    void ContinueThread();
    //FRunnable interface.
    virtual bool Init() override;
    virtual uint32 Run() override;
    virtual void Stop() override;
    bool IsThreadPaused() const;

    //================================= FLOCK =====================================
    class UBoxComponent* BoxComponentRef;

    TArray<FlockMemberData> GetFlockMembersData();

    void InitFlockParameters(TArray<FlockMemberData> setFlockMembersArr, FlockMemberParameters newParameters, UBoxComponent* SetBoxComponent);

    void SetOverlappingComponents(TArray<UPrimitiveComponent*> overlappingComponentsArr, TArray<AActor*> dangerActors);
    void SetAvoidanceActor(TArray<AActor*> avoidanceActorRootArr);

    FVector SteeringAquarium(const FlockMemberData& flockMember) const;
    FVector SteeringAvoidanceComponent(const FlockMemberData& flockMember) const;
    FVector SteeringWander(FlockMemberData& flockMember) const;
    FVector GetRandomWanderLocation() const;
    FVector SteeringFollow(FlockMemberData& flockMember, int32 flockLeader);
    TArray<int32> GetNearbyFlockMates(int32 flockMember);
    FVector SteeringAlign(FlockMemberData& flockMember, TArray<int32>& flockMates);
    FVector SteeringSeparate(const FlockMemberData& flockMember, TArray<int32>& flockMates);
    FVector SteeringCohesion(const FlockMemberData& flockMember, TArray<int32>& flockMates);
    FVector SteeringFlee(const FlockMemberData& flockMember);
    FVector SteeringAvoidance(const FlockMemberData& flockMember) const;
    FVector SteeringMaxHeight(const FlockMemberData& flockMember) const;
    FVector SteeringFollowPawn(FlockMemberData& flockMember);

    void InitFlockLeader();

    TArray<FlockMemberData> flockThreadMembersArr;
    FlockMemberParameters flockParametersTHR;
    TArray<UPrimitiveComponent*> allOverlappingComponentsArrTHR;
    TArray<AActor*> dangerActorsTHR;
    TArray<AActor*> avoidanceActorRootArrTHR;

    float threadDeltaTime = 0.f;
    float lastFrameTime = 0.f;
    //================================= FLOCK =====================================

    void SetPoolThread(TArray<class FlockThread*> setPoolThreadArr);

    AActor* ownerActor;


private:

    //Thread to run the worker FRunnable on
    FRunnableThread* Thread;

    FCriticalSection m_mutex;
    FEvent* m_semaphore;

    int m_chunkCount;
    int m_amount;
    int m_MinInt;
    int m_MaxInt;

    float threadSleepTime = 0.01f;

    //As the name states those members are Thread safe
    FThreadSafeBool m_Kill;
    FThreadSafeBool m_Pause;

    TArray<class FlockThread*> PoolThreadArr;
};
