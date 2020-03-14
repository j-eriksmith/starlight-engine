# Initial Goals for Math Library

The decision to make our own math library came from a desire to learn more about the nebulous constructs imperative to math in games like quaternions and the myriad ways to represent transforms. We knew that performance would not rival that of AAA studios and that available features would not rival open-source libraries that existed in the wild. However, it would have been futile to know which features our library needed that other libraries easily offered before building out more features. Since math was one of the earliest subsystems to get built, we had to make educated guesses on which features would be critical to Starlight's MVP and hope that there would be time to fill in the blanks as the engine developed. 

# Problems and Solutions

## Problem - What's in a Modern Math Library for Games?

SIMD, quaternions, 4x3 matrices. 

The term "SIMD" is commonly used in articles discussing performant vector libraries - like [Gustavo Oliveira's introduction found here](https://www.gamasutra.com/view/feature/132636/designing_fast_crossplatform_simd_.php). SIMD instructions are the crux of modern GPU architectures that allow for extremely fast hardware processing of similarly laid out data such as rows in a matrix. SIMD briefly provided an interesting albeit unexpected learning opportunity for the math subsystem.

Quaternions are also an essential tool for game programmers for their flexibility and efficiency. As argued by [Nick Bobic](https://www.gamasutra.com/view/feature/131686/rotating_objects_using_quaternions.php), reducing the number of floats needed to represent rotations from 9 to 4 per entity is an enormous win for memory usage. We did not have prior experience working with a quaternion interface, but it presented an exciting learning avenue.

Finally, Jason Gregory in Game Engine Architecture hinted that AAA-grade game engines represented transforms as 4x3 matricies instead of 4x4 matricies with the insight that the bottom row (or column) would always be [0.0, 0.0, 0.0, 1.0]. Since most open source math libraries used a full 4x4 matrix for transforms, we were excited to implement a feature that would prepare us for industry standards.

## Solution - Slim Feature Set

To align with our subsystem goal of elucidating math concepts instead of squeezing out maximum performance, we shied away from diving deep into a SIMD implementation. Dissapointingly, due to time constraints and a working axis-angle interface for rotations, we shied away from implementing quaternions into the library after extensive learning on what they were and how to use them. Additionally, little documentation was found on how to compose an interface for transforms once rotation and scale were separated from their traditional 3x3 matrix formulation. We did keep the insight of implementing transforms as 4x3 matricies to save ourselves 4 floats per transform where possible (more on this with GLM discussion in Deliverable section).

## Problem - Unit Tests

Both of us knew that writing tests was a critical part in building scalable, rigorous software - a skill we wanted to train through Starlight from the beginning. The math subsystem seemed like a prime candidate to establish practices that could be applied to more complex subsystems later with clear answers to check against and clear operations with each function call. 

## Solution - Static Tests 

The math library has a static function RunTests that runs a series of unit tests that pass or fail via assertions. This was helpful for catching bugs with more complex operations like rotations and SLerps, but it was difficult to maintain a quality set of unit tests for each math construct as development went on. For every useful test that caught a bug or gave us new insight into how code should be organized, it felt like there were ten tests that needed to be written for complete coverage, even if those tested fairly routine operations like a vector dot product. 

# Deliverable

In the end, I don't think that writing our own math library was a good use of time, especially for an entire quarter. Most OpenGL tutorials referenced make heavy use of the [GLM math library](https://glm.g-truc.net/0.9.9/index.html), so most of the innards of our rendering subsystem use GLM. But since it was never an intention to build a math library with as many features as GLM, it was no surprise by the end of development we had to rely on powerful functions such as glm::lookAt. In ugly cases, vectors and matricies would go from Starlight --> GLM --> Starlight to conform to parts of code that inflexibly relied on Starlight's math objects. 

That being said, building a math library from scratch becomes a great survey of the underlying cost of every math function that you use as a gameplay programmer, from cross products to spherical interpolations. And where other libraries steer left, you can steer right; for example, I used the intuition that struct members would be laid out sequentially in memory to avoid use of arrays in vectors (to contain floats) and arrays in matricies (to contain vectors).

There were times when this freedom came back to bite us. Choosing our transform representation to effectively be a 4x3 matrix by using two data members (a 3x3 matrix and a Vector3) meant that transforms couldn't leverage easy contatenation via multiplication. On top of that, we had to build out a 4x4 matrix class anyways to support intermediate GLM conversions for the rendering backend. 

Since transform concatenation wasn't built into the subsystem, it should come to no surprise that decision led to further debt when transform hierarchies were considered. Preliminary support for transform hierarchies were built out, but since our engine priorities never had to shift towards representing objects in a formal scene graph, the feature is untested.

# Documentation