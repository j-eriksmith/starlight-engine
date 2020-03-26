# Initial Goals for Input 

Our Input subsystem takes heavy inspiration from [Unity's easy-to-use and easy-to-access static interface](https://docs.unity3d.com/ScriptReference/Input.html). We also wanted to use the GLFW input backend that had already been incorporated into the engine for testing out OpelGL, but hide library calls behind a more client-friendly way for developers going forward.

While we considered a more advanced event and callback system, we elected to implement a solution that optimized between developer ease of use and short time to implement.

# Problems and Solutions

## Emulating Unity's GetKeyDown

GLFW came with just about everything we needed out-of-the-box except for emulation of Unity's extremely convenient [GetKeyDown](https://docs.unity3d.com/ScriptReference/Input.GetKeyDown.html) that ensures that input events that are only meant to be processed once per press do just that.

The cleanest way that we arrived at was with a double buffered array of key state, where each index is a Starlight key code. Before any game behavior is updated, GLFW iterates through all supported Starlight keys and determines if the key was pressed for the frame, writing the results into one of the two state buffers. If the result was was Pressed this frame and Not Pressed the previous frame, we've detected a `GetKeyDown` event. Inversely, if the result was Not Pressed this frame and Pressed the previous frame, we get `GetKeyUp` support for free. After a frame finishes processing, we swap buffers so that the next frame can overwrite input data from two frames ago (now useless) while referencing input data from one frame ago in the other buffer.

We needed to create our own GLFW to Starlight key code translation scheme (aka a giant switch statement) because we had no use for many of GLFW's keys as we targeted American Windows keyboards. Additionally, many enum values in GLFW's key codes are outright skipped likely due to unused keys being deprecated from the enum over time. Left untranslated, our two state arrays would be about 3x larger than necessary!

# Deliverable

Our Input subsystem is essentially a GLFW wrapper with some usability niceties supporting keyboard and mouse inputs along with mouse movement. Should developers have need for higher-level abstractions (and less if-statements), the subsystem can easily extend to push event notifications to subscribed gameplay systems to handle.

# Documentation