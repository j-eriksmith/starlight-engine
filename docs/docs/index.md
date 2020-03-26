# What is Starlight?

Starlight is a 3D game engine written in C++ planned, written, and finished over the course of a year by two UC Irvine Computer Science undergrads. We were hugely inspired to make our own 3D game engine by the work on the [Isetta Engine](https://isetta.io/) by Carnegie Mellon University students.

Starlight is an exploratory research project. It is the first large-scale C++ codebase that either of us have written from start to finish. While every decision we made was thoroughly researched against modern developments in game engines, not every decision we made turned out to be the right one. We believe it is a responsibility for programmers of high performance software to thoroughly understand their platform -  engine and target hardware included. This documentation serves to candidly share our triumphs and failures in order to encourage and embolden the aspiring gameplay, engine, and C++ programmers of tomorrow. 

# What is Starlight not?

Starlight is not ready to ship commercial games nor compete with popular engines such as Unity or Unreal Engine 4. Starlight does not a user-friendly editor and interface to build a game with.  Starlight is not the most performant engine on the market; however, it was built with sound architectural choices such as [ECS](ecs.md) that would allow the engine to scale given more optimizations. 

# Why We Chose to Build a 3D Game Engine in a Year

## Practice C++ Programming

Both of us saw the long-term value in mastering the language that dominates performance-sensitive computing applications, and AAA video games are no exception. After taking a quarter to teach each other lessons from Scott Myers' _Effective C++_ and _Effective Modern C++_, we knew that we needed to quickly put what we learned into practice lest we forget.

## Improve software development habits

While internships and class projects are certainly valuable, neither of us had experience developing codebases meant to scale, designing APIs to be used extensively and easily by others, or thoroughly maintaining older code to support new demands. These were critical skills for software engineers to learn and a year-long collaborate coding project could be the perfect space to do this.

## Make a functioning game engine capable of supporting first-person experiences

Developing an engine without defining the type of game it is best geared for is a mistake, especially for a first game engine. We chose to support first-person experiences because of our mutual passion for the genre with games that we love to play like Counter-Strike and Overwatch. From there, every milestone and subsystem that we decide to build can be weighed against the question of "How would this fit into a first-person experience?"

## Provide substantial learning resources for aspiring engine developers

This engine was only possible because of the freely available resources online, from YouTube tutorials and conference talks to the blogs of former AAA game developers. We want our development logs for this engine to be a small but immensely useful drop in the massive (and often hard to navigate) pool of available learning materials. 

# Initial Timeline
|           | March 19 | April 19 | May 19 | June 19 | July 19 | Aug 19 | Sept 19 | Oct 19 | Nov 19 | Dec 19 | Jan 20 | Feb 20 | March 20 |
|-----------|----------|----------|--------|---------|---------|--------|---------|--------|--------|--------|--------|--------|----------|
| Jake      | Math     |  File I/O|File I/O|Internship|Internship|Internship|ECS  |ECS     |ECS     |ECS     | Audio  |Audio   |Polish    |
| Alejandro | Memory Manager | Rendering, Input, UI |Rendering, Input, UI | Internship | Internship | Internship | Rendering, Networking| Rendering, Networking | Networking| Collision Detection |Collision Detection| Animation |Polish|

Check out the [Conclusions](conclusions.md) page to see how the timeline evolved by the end of the project.