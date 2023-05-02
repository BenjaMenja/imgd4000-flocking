# Boids Algorithm

Written by Ben Gelinas

This repository contains an implementation of the Boids flocking simulation algorithm, following the pseudocode described at http://www.kfish.org/boids/pseudocode.html.

Contains all three rules required for flocking behavior, including:

* Boids try to fly towards the centre of mass of neighbouring boids
* Boids try to keep a small distance away from other objects (including other boids)
* Boids try to match velocity with near boids

The following additional rules were also implemented:

* Limiting maximum speed of the boids
* Bounding the position of the boids within a box

A demo video can be found here: https://youtu.be/tCG8TukMVhY



