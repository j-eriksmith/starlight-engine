# Initial Goals for Graphics
Our expectations for the Rendering System was a simple API that would enable devs to spin up basic 3D scenes and provide basic GUI editor tools. Since our main goals were to prioritize the multithreaded game loop and networked multiplayer, we wanted to integrate a 3rd party rendering engine like [Ogre3D](https://www.ogre3d.org/) or [Horde3D](http://www.horde3d.org/) to perform the heavy lifting for us. These engines have all the functionality we need - and much, much more - out of the box, so we were hoping to integrate one of them quickly to save development time.

# Problems and Solutions

## Out with the Old
After tinkering with the Ogre3D engine for about a week, we decided that the setup and integration of an old, massive graphics engine was more trouble that it was worth. After all, our engine's graphics requirements were so low that we probably would not use 90% of any built-in graphics engine's potential. The next logical step was to look into working directly with the graphics specification, and since OpenGL was the most intuitive for novice graphics programmers to pick up, that was our first choice. However, since neither of us had much knowledge in the graphics domain, we spent a week or so tinkering with [Learn OpenGL](learnopengl.com) and [The Cherno](https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2) tutorials to see if working with OpenGL was a viable option. After building a few basic 3D scenes, we decided to move forward with OpenGL and recalibrated our expectations accordingly. Mainly, we decided to reconsider the scene graph visualization and editor functionality as nice-to-have's so that we could focus on developing other core subsystems rather than developing peripheral features from scratch.

In August, after about a month of work, we had a basic rendering system that could render 3D models with textures. Since we had this subsystem in a good place, we moved our focus elsewhere during the Fall 2019 term.



# Takeaways
The biggest triumph of this system was making a decision in the face of uncertainty and seeing it out until completion. Attempting to build a rendering API from scratch with little graphics domain knowledge and a strict time constraint was risky because, for all we knew, every day we spent learning about OpenGL was a day we could have spent integrating a 3rd party graphics engine that would have saved us time and effort. However, one of the main lessons we learned is that making a well-researched decision that turns out to be mediocre is better than being too scared to take any risks, because even if you spend some time doing down the wrong path you come out the other side with more insight into the problem at hand. In our case, after building our rendering API, we learned OpenGL's strengths, such as a powerful shader integration and an intuitive state-machine design, as well as its drawbacks, like its lack of model loading, weak debugging support and limited GUI development. In our future iterations of Starlight, we will surely take these into consideration if we decide to revamp the rendering API.

# Deliverable
The end result was a system that could render skyboxes and 3D models with diffuse textures.

Features included:

- Custom shader support

- Easy to use 3D model loading system

- Skybox rendering functionality

- No external graphics engines used

Left for future iterations:

- Support for specular texture support

- Support for models with multiple textures

# Documentation

## Model Loading and Drawing
We used the [assimp](https://github.com/assimp/assimp) library for loading models, which supports a variety of file formats. 
To create a Renderable Component, follow the example below:

    // Load the model into memory.
    RenderableComponentPtr renderableComp( ModelLoadingSystem::LoadModel( ModelPath ) );
    // The transform component will keep track of the model's position in the 3D scene.
    TransformComponent model( Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f) );
    // The shader component will enable OpenGL to display the model on the screen.
    ShaderComponentPtr shaderComp(ShaderSystem::CreateShaderComponent("Basic.shader"));

    // Create your entity.
    Entity* entity = e.CreateEntity();

    // Add empty components the entity.
    RenderableComponent* r = entity->AddComponent<RenderableComponent>();
    TransformComponent * t = entity->AddComponent<TransformComponent>();
    ShaderComponent* sh = entity->AddComponent<ShaderComponent>();

    // Transfer relevant data into the entity's components.
    t->Data = model.Data;
    RenderingSystem::TransferData(t1.get(), r);
    ShaderSystem::TransferData(shaderComp.get(), s);

Every frame, the RenderingSystem will perform its Update method, iterating over and drawing every entity that contains a RenderableComponent. Therefore, after writing this code, the game loop should take care of bringing your model to the screen.