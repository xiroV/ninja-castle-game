# Ninja Castle
## Introduction
Assignment for DM842 Computer Game Programming.

The goal is to make a King of the Hill game, 3D platform style.

The game is written in C++, and all 3D files have been made using
the Open Source 3D suite [Blender][https://www.blender.org/].

The current status:

 - [x] Player Movement
 - [x] Character Graphics
 - [ ] Character Animation
 - [x] Character Attack (collision detection)
 - [x] AI
 - [x] World/Level Graphics
 - [x] King of the Hill Mechanics (score, etc.)


## Goals
The game is a variation of the King of the Hill children game. The game
takes place in a castle-like arena with a center platform and a surrounding moat.

The center platform is connected by bridges to the rest of the castle, and the two
players will spawn in opposite ends of the castle.

The player now have to control this center platform in order to win the game, while
trying to keep the AI-controlled player away from the platform, e.g. by pushing him
out into the moat.

A player on the platform will gain 1 point for every second on the center platform,
and anyone who falls down the moat will respawn after 3 seconds.

The player who reaches 500 points first, will win the match, and the game will restart.


## Installation
Tested on Linux only, with the following dependencies
 - OpenGL 4.3
 - Glew
 - Glut
 - GLM

To install, in a terminal emulator, enter
```
cd src
make
```

To run, simply run the resulting `game` binary, located in the `bin` directory:
```
./bin/game
```

## Controls
 - Move around using the arrow keys
 - Jump by pressing `Z`
 - Press `ESC` to exit
