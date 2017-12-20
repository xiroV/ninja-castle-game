# Ninja Castle
Assignment for DM842 Computer Game Programming

Goal is to make a King of the Hill game, 3D platform style.

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


## Controls
 - Move around using the arrow keys
 - Jump by pressing "Z"
