# Initial Goals for Audio

The audio subsystem only needed to support a small set of features; namely, the ability to load sounds ahead of time and cache the data for future `PlaySound` calls. And like all of our subsystems, we wanted clear paths of integration into our memory allocators for the best runtime performance. 

Our two contenders were [WWise](https://www.audiokinetic.com/library/edge/?source=SDK&id=index.html) and [FMOD](https://www.fmod.com/resources/documentation-api?version=1.10&page=content/generated/common/introduction_web.html) SDKs - both popular in the games industry and had integration guides for C++ projects. FMOD took the edge by striking the balance between industry-wide use and having a wealth of great tutorials by both the community and the developers on how to configure it for an existing game engine project.

# Problems and Solutions

## Installing FMOD for Starlight

FMOD is effectively split into a core API and a studio API, and the C++ code exposes each of the libraries by initializing `System` objects. The studio API is tuned for the Events and Sound Banks workflow, which we knew we would be avoiding in favor of faster implementation time and lower learning curve. Instead, most of our code uses the core API that exposes lower-level constructs like channels and `FMOD::Sound*` objects to be loaded into those channels.

As budding C++ Windows developers, we wrestled a bit to get FMOD's dynamic libraries recognized by our executable at runtime. One solution is to hand-copy over the .dll files once before running the engine - but this solution isn't nearly as elegant for onboarding new developers. We saw that a common solution was to add a Post-Build step to Visual Studio's compilation process (Project Properties -> Build Events -> Post-Build Event) that would run Windows' `xcopy`. Finding the right flags to use involved a ton of trial-and-error, especially as `xcopy` will (as far as we could tell) silently fail when incorrect arguments are supplied. 

Here is the full `xcopy` command that Starlight uses as a Visual Studio post-build step to make the necessary FMOD core libraries visible to the engine executable:

```
xcopy /y /d "$(SolutionDir)starlight\Dependencies\FMOD\core\lib\x86\*.dll "$(OutDir)"
```

## Efficient String Lookups

The ideal usage for Audio was to have a gameplay programmer refer to each sound resource by its file name each time they wanted to load, play, or unload the sound. This was the simplest handle to expose to a programmer without passing around something like lightweight audio clip objects. Temporary string allocations are expensive at runtime and the tech debt would only grow when doing string comparisons to find resources in a data structure. We needed to find a lightweight, easy-to-integrate solution for string hashing to unsigned integers that could happen at compile time with the help of `constexpr`.

After a few unsuccessful leads on string hashing solutions, we were lucky to find [Allen Chou's open source String ID GitHub repository](https://github.com/TheAllenChou/string-id) that checked all the boxes we needed and took an hour or less to get running. Finding this library allowed us to use string hashing for unique component IDs in our ECS.

# Deliverable

The Audio subsystem is essentially a pared-down wrapper for FMOD that is incredibly easy to use for quick prototyping, but does not support the Events workflow that many audio designers love to use to make immersive soundscapes. Programmers use the statically accessible `AudioEngine` interface to load, play, unload, and control channels hidden within the `FMODModule` implementation layer.

Adding features such as 3D audio and looping sound clips required far less code than we imagined, so Starlight has support for both of these. Given more time, we would have loved to have abstracted out concepts such as [AudioSource](https://docs.unity3d.com/ScriptReference/AudioSource.html), [AudioClip](https://docs.unity3d.com/ScriptReference/AudioClip.html), and [AudioListener](https://docs.unity3d.com/ScriptReference/AudioListener.html) like Unity does as Components in our ECS. As it stands, a Starlight audio source in 3D is coupled with the audio clip that it is playing, and the player's camera is always the audio listener. This setup likely works for most games, especially early prototypes, but may not scale well for commercially viable sound design practices.

We stand by the choice that FMOD was the right library to use even when not using all of its features, especially when the learning curve is eased by great public tutorials and an active official FMOD message board that had answers to most questions that we had. FMOD even allowed for an initial memory allocation to be done at startup with our own pool allocator using `FMOD::Memory_Initialize`. Later, it was bug-free to subsequently deallocate FMOD's audio memory through `MemMgr::Free`.

### Helpful Resources

- [Setting Up Xcode and Visual Studio for FMOD Development - Cody Claborn](https://codyclaborn.me/tutorials/setting-up-xcode-and-visual-studio-for-fmod-development/)

- [Making a Basic FMOD Audio Engine in C++ - Cody Claborn](https://codyclaborn.me/tutorials/making-a-basic-fmod-audio-engine-in-c/)

- [String ID - Allen Chou](https://github.com/TheAllenChou/string-id)

# Documentation