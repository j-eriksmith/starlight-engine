# Initial Goals for File I/O

File I/O was the first obvious place to try multithreading and all of the standard library's support for it since C++11. We initially thought that the entire game loop (input, behavior updates, output graphics and sound) could be multithreaded, so File I/O became a proving ground for figuring out usable APIs for parallelism and the best tools for mutual exclusion. 

We took inspiration from modern open worlds built with an incredible amount of background asset streaming to support the creative vision and a seamless player experience, like God of War (2018), Spider-Man (2018), and Red Dead Redemption II. We figured that given these trends, asynchronous file streaming was an essential construct to learn for aspiring engine developers.

# Problems and Solutions

## Problem - Platform File APIs

Most file APIs for a game engine have a platform-independent interface layer that hides platform-specific code behind a public API. While we would have loved to have had the time to investigate and implement libraries for all three major platforms (Windows, Mac, Linux), we simplified a standard platform for all libraries going forward in the engine to target Windows first. However, we tried to write the code as if there could be an interface layer easily built on top of platform-specific code. 

The Windows File API is intimidating at first. For example, [CreateFileA](https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea) seems impenetrable at first before noticing how helpful Microsoft's long-winded but eventually clear documentation is, especially when explaining the valid flags for parameters. 

## Problem - Asynchronous File I/O

We knew that File I/O requests had to be handled by a separate `std::thread` object - potentially many in a worker thread pool. To accommodate for the possibility of the latter, work began on a thread-safe fixed-size ring buffer/circular buffer. The idea was that work on any engine thread could queue up a `Request` object into the ring buffer to be handled asynchronously. Once the worker thread loaded the resource, it could call a function pointer passed into the initial `Request` by the client. We started with text file requests (for items like a configuration file) and texture files (for object rendering). 

This initial attempt posed some problems down the line; for example, manipulating the client's object state through a function pointer called by the worker thread. This would be partially remediated by looking towards an interface that relied on `std::future` and `std::promise` objects akin to asynchronous interfaces seen in popular front ends languages like Javascript's `async/await`.

## Problem - Resource Manager

One of the larger byproducts of using OpenGL instead of an existing graphics framework like Ogre3D and Horde3D was that we no longer had free access to a resource manager; that is, an entity to ensure that multiple copies of the same resource were not active at runtime. 

Once the insight was made that callback function pointers were too strict for the behaviors needed, `std::future` and `std::promise` objects became the backbone of the resource manager. The implementation was simple enough; at its core, a dictionary of lookup keys (like a hashed string) to a resource value pointer (like a texture in a byte buffer). Once a client thread asks the resource manager for a resource, the resource manager will create the request for the I/O thread and populate the `std::future` object with the requested data. 

# Deliverable

Our File I/O API comprises of the Resource Manager and its interface to asynchronous File I/O using `std::thread` backed by the Windows file API. While the setback to create our own resource manager was annoying at first, it was a great foundation for the programming paradigm employed many other systems down the line that rely on cached results to avoid expensive runtime lookups, like our Audio subsystem.

### Helpful Resources

- [Creating a Circular Buffer in C and C++ - Phillip Johnston](https://embeddedartistry.com/blog/2017/05/17/creating-a-circular-buffer-in-c-and-c/)

- [A Resource Manager for Game Assets (and comments) - Gamedev.net](https://www.gamedev.net/articles/programming/general-and-gameplay-programming/a-resource-manager-for-game-assets-r3807/)

- [Stackoverflow: What is std::promise?](https://stackoverflow.com/questions/11004273/what-is-stdpromise/12335206#12335206)
# Documentation