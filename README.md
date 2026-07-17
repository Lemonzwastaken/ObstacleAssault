# Obstacle Assault — Feature Showcase Build

> **Status:** Pre-Pre-Alpha / Internal Showcase
> This is a **development build** intended to demonstrate individual movement mechanics in isolation. It is **not** representative of final game, it exists purely to showcase how each system currently functions.

---

## About The Game

<img width="113" height="134" alt="image" src="https://github.com/user-attachments/assets/4b5060b5-ef74-4855-9cb0-4b152f7696b2" />

Obstacle Assault is a physics and movement-based parkour rage game inspired by games like Getting Over It or Chained Together

---


## Controls (Default Bindings)

| Action | Input |
|---|---|
| Move | WASD |
| Jump | Space |
| Slide |SHIFT|
| Dash | SHIFT (In air) |
| Wall Run | Hold SHIFT when near a wall runnable wall |
| Grind | Automatic on approach while falling |
| ZipLine| F after jumping under a zipline| 
---

## Current Features

The game was built using Unreal Engine's 3rd person movement system. However the game uses a custom movement system built on top of the current movement system to incorporate the extra features for the game
Here are some of the features made in C++ and Blueprints

1) WallRunning System: A wallrunning system that allows the player character to adjust to the wall of any angle. The wall needs to be a BP_WallRunnableWall and has the WallRunnableInterface
2) Grinding System: A grinding system inspired by Ghost Runner and other fun parkour games. Grinds on Splines that are only available to create in BP_GrindingPlatform
3) Dashing System: A dashing system ALSO inspired by Ghost Runner.
4) Spinni Platform: Spinning platform that rotates and moves, and does both
(All of the above are made using C++ only)
5) Sliding System: Slides, there's nothing much to say
6) ZipLine Feature: The Thingamajig corporation introduces no animation ziplines

<img width="536" height="571" alt="image" src="https://github.com/user-attachments/assets/dcb9cba7-361d-4a17-8b99-760f30246948" />

7) Automatic door: Opens by itself and plays a nice sound

## Purpose of this release

Through this release, I want to show how certain features of the game work. The release is a development build and not a final build, so you can see how different traces work for different types of sorts. Also, there might be performance issues in that case if you wish to check out the game, watch the YouTube video below

[![Watch the video](https://youtube.com)](https://youtu.be/TqL-6thSbOI)


The game has a lot of debugging features left over, so you can take a peek behind the game development curtain :D (this is totally not me being lazy)

<img width="643" height="659" alt="image" src="https://github.com/user-attachments/assets/c8297ff5-a4a1-4b78-9c66-65dc70df0ca4" />



## Future for this game

I will continue to work on this game for the time being. Will add new stuff like voice lines and a proper map, more optimisations, etc.
This is a rough build through which I wanna showcase my first time working with C++, albeit through tutorial hell.

It was a fun journey, and I hope to get back to this project soon <3

## AI DECLARATION

AI has been used in this game to find errors in code, which I was unable to find, and make small modifications like print checks, etc. All mechanisms have been written by me only and only small changes as well as fixing errors have been done using AI

## Liscense

Feel free to use the project code as you like. there are no assets tho so that would be a problem rest all the blueprints and C++ code is there :D

*Built in Unreal Engine 5.7.*
