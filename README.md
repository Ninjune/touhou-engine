
# Touhou Engine
Project for school. Allows you to make stages to make something akin to a Touhou game.

## To build:
- Open *.sln in visual studio 2022.
- Change to x86.
- Build at top.
- must build using C++17 or above.
- make sure the built solution is in the same folder with the textures folder, patterns folder, etc.

Textures are not mine:
- Everything except icons from the Touhou series. (th15)

## Controls:
### Warning
Do not change the window size.
### Stage loader:
- plus to play, edit to edit, plus at bottom to add stage.
- hit Escape in any state to return to the stage loader (will not save stage if you are editting one.)

### Play:
- arrow keys to move
- 'Z' to shoot
- shift to focus (see hitbox & move slower)
- you die if you get hit three times (stars in top right are hits, 2 seconds of immunity if you get hit)

### Editor:
- use m1 to navigate timeline, left and right also navigate it by one frame, hold shift + keys for 60 frames.
- use m1 to select tools, cursor to select & move enemies, pencil to add and edit enemy paths, duplicate to duplicate.
- use m2 with pencil to create a new enemy then you can increase the path length (max path length = path speed*60) using up and down arrow keys
- use m1 when have an enemy selected to draw the enemy on the playable area or above it (not to sides or below, you can move enemies to those areas though.)
- use backspace or delete while selecting an enemy to delete the enemy
- while selecting an enemy there is a menu on the left this is the pattern menu, you can add
and remove patterns to enemies using this (you must go to the frame you added the pattern to remove it).
- you can add patterns with the green plus at bottom & you can edit patterns using the edit button (click the button again to return to editor)
- in the pattern editor you can move the cursor around by clicking or left and right arrow keys. works similar to a normal cursor thing but not exactly.
- note that patterns are linked so if you delete a pattern it will be gone if you resart game
(however if you edit a pattern you need to remove it from any enemy that has it then add it again for
it to update in your current instance, if you restart then it will automatically do this however)

Todo:
- Score
- Bomb
- other characters
