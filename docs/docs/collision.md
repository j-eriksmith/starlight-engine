# Initial Goals for Collision
We started building the Collision System near the end of the Fall 2019 term, and the work overflowed into the beginning of Winter 2020. Collisions are an integral part of any game engine, but since we left it for last, we decided to make it as simple as possible to leave time for debugging and clean-up. As a result, our goal was to have a simple, functional collision detection and resolution system that gave developers some flexibility and power to do cool things with their game entities.

## Creating an ECS-Friendly Collision System
Resolving collisions between two entities often requires updating a component that is unique to one entity or the other. However, every ECS system in our engine operates on the same subset of components between entities, which means that we cannot access components that are unique to certain entities. Below I describe a challenge I faced when implementing the collision resolution system that illustrates this issue.

In our test game, we wanted to simulate throwing darts at a target, which involved resolving collisions between these two entities. Initially, we modelled our dart entity as a collection of Renderable, Collision, and Dart components. Below is a snippet of the dart component code.

    class DartComponent : public Component
    {
    public:
        explicit DartComponent()
            : Component(UniqueID),
            Velocity(0,0,0)
            Lifespan(10.f) {}

        DartComponent(const float& lifespan)
            : Component(UniqueID),
            Lifespan(lifespan) {},
            Velocity(0,0,0)

        float Lifespan;
        // The field to update during collision resolution
        Vector3 Velocity;
        static constexpr ComponentID UniqueID = StringIdHash("DartComponent");
        static unsigned int EngineMemoryID;
    };

Since the player could launch darts in different directions, it made sense to store each dart's velocity in the DartComponent. Our target entities contained Renderable and Collision components, but no 'Target Component' because they were simple, stationary objects.

We wanted to resolve collisions between dart and target entities by setting the dart's velocity to 0, to simulate the dart being tacked onto the target's surface. This would involve reaching into the dart entity's DartComponent, which breaks ECS's rule of only acting on entities' shared subset of components. To resolve this issue, we decided to abstract velocity into a completely separate "MovementComponent", which we attached to both dart and target entities. Now, we can resolve collisions while conforming to ECS, and as a side effect we gave ourselves the option to give movement values to targets as well (in case we wanted to present the player with more of a challenge).

The concept of systems acting on entities with similar components was foreign, considering the only other paradigm we had as a reference was object oriented programming (OOP). In OOP, objects are modeled much more explicitly, where each object has data members and methods, and can inherit from classes with more data members and methods. Additionally, there are no restrictions on which data members you can alter, and you can arbitrarily compare objects from different classes with no repurcussions. However, ECS is better organized in the sense that each 'object' is partitioned into components that can be more descriptive about that entity's functionality and purpose. Additionally, ECS is supposed to be much more hardware-efficient (see the ECS section for more), and we wanted to maximize our engine's performance.

# Documentation

Thus, creating an entity with a collidable bounding box would look like this:


    // Init the Renderable component using the model of your choice.
	RenderableComponentPtr renderableComp( ModelLoadingSystem::LoadModel( *model file path* ) ); 

    // Init a Shader Component which enables Starlight to draw models using OpenGL.
	ShaderComponentPtr modelShader( ShaderSystem::CreateShaderComponent( *shader file path* ) );

    // Init a TransformComponent to keep track of the entity's position in 3D space
    TransformComponent model(Vector3( 1.0f, 0.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ), Vector3( 0.0f, 0.0f, 1.0f ), Vector3( 0.0f, 0.0f, 0.0f ));

    // Init Collision Component using the RenderableComponent.
    CollisionComponent* collisionComp( CollisionSystem::GetCollisionComponent( renderableComp ) );

    // Denote the entity's collision type.
	t1c->collidableType = CollisionComponent::CollidableType::Structure;
    // By default the TransformComp starts at (0,0,0), so we must transform the origin to the model's newly calculated staring point
	model.Data.Origin = collisionComp->origin;

    // Create entity using an existing Engine object
    Entity* entity = e.CreateEntity();
    // Attach components to entity
    RenderableComponent* r = entity->AddComponent<RenderableComponent>();
    ShaderComponent* sh = entity->AddComponent<ShaderComponent>();
    TransformComponent * t = entity->AddComponent<TransformComponent>();
    CollisionComponent * c = entity->AddComponent<CollisionComponent>();

    // Transfer data from your manually constructed components to the components attached to your entity.
    RenderingSystem::TransferData(renderableComp.get(), r);
    t->Data = model.Data;
    ShaderSystem::TransferData(modelShader.get(), sh);
    CollisionSystem::TransferData(collisionComp, c);

Note how the CollisionSystem currently relies on RenderableComponent information to create a CollisionComponent that accurately represents the parent entity's bounding box. 

### Collidable Types

Starlight's Collision System also enables developers to create custom collision 'Types' using CollisionComponent's CollidableType enum:

		static enum class CollidableType {
		Friendly = 0,
		Enemy = 1,
		Projectile = 2,
		Structure = 3
	};

Each frame, the Collision System detects and resolves collisions between each unique pair of entities that contain collision components. However, the system's methods for resolving collisions is mostly empty, so the developer must decide how entities of each type should interact with each other.

#### Creating new CollidableTypes

Before the Collision System can resolve a collision between two entities, it must first determine each entity's Collidable Type. As a result, when adding new CollidableTypes, the developer must add to the Collision System's boilerplate code for detecting CollidableTypes. Make sure to add register your new type in this method:

    void ResolveLhsCollidableType(ComponentTuple* lhs, ComponentTuple* rhs);

and to add template-specialized methods for your new CollidableType for these methods:

	template<>
	void CallResolveCollision<CollidableType::YourNewType>(ComponentTuple* lhs, ComponentTuple* rhs);

    template<>
	static void ResolveCollision<CollidableType::YourNewType, CollidableType::OtherCollidableType>(ComponentTuple* lhs, ComponentTuple* rhs);
	template<>
	static void ResolveCollision<CollidableType::OtherCollidableType, CollidableType::YourNewType>(ComponentTuple* lhs, ComponentTuple* rhs);

Note: You will need to add ResolveCollision methods for each CollidableType you want your new type to interact with.
