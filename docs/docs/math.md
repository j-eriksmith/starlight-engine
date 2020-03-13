# Initial Goals for Math Library

Learning over performance - except for learning about 4x3 matricies popular in gamedev. in hindsight, not a good tradeoff (glm fine for our purposes, linear algebra recap not the focus)

# Problems and Solutions

## Problem 1 - Modern Math Libraries

## Solution 1 - Not Using SIMD

## Problem 3 - Unit Tests

## Solution - Static Tests function, not a more rigorous framework

# Deliverable

Math library that is not comparable to glm, but does a few things that I liked better (exploiting linear struct member packing vs. explicitly declaring arrays)

Good survey of math tools

Shy away from 4x4 matricies, but implemented anyways

Completely forgot about transform parenting, causing the transform framework to go haywire very quickly (applying transform to transform harder).

# Documentation