# Tank Game - A really Simple Puzzle Game

Tank Game is a really simple puzzle game that I wrote as a quick project a little while ago. At the time, I was working on some computer science project from home for school and I was really bored. So, I decided to do something a little more interesting.

## Requirements

You **must** have *libsdl2* installed to play and its associated headers present to compile. If you do not, running will fail, as it cannot dynamic link and compilation will fail as it cannot find a declaration of all the SDL symbols.

## Playing the game

Run ``make`` in the root of the project. This will compile and create en executable in the root of the project. You can now run this executable to play the game.

## FAQs

### One or more missing textures? Check your game installation?

1. Make sure you are running the game from the directory you installed it. If you do not do this, you will get this error
1. Make sure you have not deleted the *res* folder or anything contained within it
1. Make sure you have permissions to access the game resources folder

### Invalid level file?

1. Please see advice 1/2 for textures, but for the *levels* folder
1. The game is in development. If I occasionally push a broken change for a level file, I'll try and fix it as soon as possible

### *libsdl2.so* no such file or directory?

Please see the advice on the requirement for libsdl2 to be installed

### SDL2/SDL.h no such file or directory?

Please see the advice on the requirement for the development headers for libsdl2

### Abort? Corrupted heap? Invalid last size? Double free?

Please ignore these errors. They, generally, only appear on the program's exit and are caused by some problem in SDL that hasn't yet been found. It is caused by a problem when freeing the SDL texture surfaces.
