# Initial Goals for Memory Management
We wanted Starlight to be as memory-efficient as possible, so creating a memory management system that supports faster dynamic allocation than calling 'new' and 'delete' was a must. Since memory management is the foundation of any game engine, we decided to build this subsystem first.

The next steps were to understand the different types of allocators game engines typically use, deciding which ones satisfy Starlight's needs, and building prototypes. Since we had no knowledge of game engine allocators, we took to Jason Gregory's [Game Engine Architecture](https://www.gameenginebook.com/) to explore the topic. This book led us to choosing the stack allocator and pool allocator designs, the former for large or variable-sized data like renderable models and the latter for smaller items with similar sizes, like particles. After sifting through books, online articles, forum posts, and Youtube videos for interesting memory management techniques, we found [this article]( https://www.thinkmind.org/download.php?articleid=computation_tools_2012_1_10_80006) detailing an efficient pool allocation system that can initialize memory blocks only as they are needed. After running with this design and going through several iterations of prototypes, we finally reached a sufficiently fast and accurate pool allocator!

The next step was to implement the stack allocator. During this process, we revisited Starlight's memory needs and decided to create 4 distinct categories of memory:
1. Global - Remains in memory for the duration of the game
2. Level - Loads in at the beginning of a level, freed after level completes
3. Frame - Stored only for the current frame
4. Pool - Used for large batches of smaller items

Organizing our memory this way allows for clarity when other subsystems are performing allocations and gives the developer more freedom when determining the allocator size for each of these memory types.

From this point, implementing the Stack allocator was straightforward. After writing some unit tests for both allocators, we were confident to start using Memory Manager calls in other subsystems!
# Problems and Solutions

## Research
The Memory Manager was my (Alejandro) first time trying to solve such an open-ended software development task, so I learned a lot about software research in the process of building this system. The main issue was that I felt like I was not progressing in my tasks. It was difficult for me to quantify my tasks with concrete metrics other than "hours of reading / whiteboarding completed", and even after several weeks of research I felt like I was far from a solution. One of my main takeaways from this experience was that I should be prototyping while I carry out my research. Instead of figuring out the perfect solution for a given task before touching my keyboard, trying out different potential solutions earlier on in the process is a more effective way to gain insight into the problem at hand, even if it means failing many more times.

# Documentation
The Memory Manager is a singleton object in the MemMgr class, so all calls to MemMgr will be to static methods.

## Initialization
To initialize the Memory Manager, call

    MemMgr::Create( totalSpace );

Note that totalSpace is in bytes. If you want to change the proportions of the frameData, poolData, globalData, or levelData allocators, do so in MemMgr's constructor.

## Allocating and Freeing Memory
To allocate a chunk of memory, call

    MemMgr::Alloc( resourceSize, allocatorType );

where resourceSize is the number of bytes you want allocated, and allocatorType is an enum within the MemMgr class denoting the type of allocator you want to reserve the memory. Note that when allocating a chunk of memory for class types that are part of an inheritance hierarchy, you should use the placement new operation to ensure that the object's vtable information is stored with the object. Below is a usage example:

    Object* newObj = new (MemMgr::Alloc(sizeof Object, MemMgr::AllocatorType::LevelData)) Object();

To free an object's memory, call

    MemMgr::Free( totalSpace, addr );

where addr is the pointer to the object you've allocated space for.