# REFix
An REFramework plugin that fixes the mouse input in RE2R. This plugin is intended to be used by those playing with a mouse and keyboard. Controller players may
not find it suitable.

## Compatible Games/Versions
This mod was created for the DX11 non-raytraced version of the Resident Evil 2 Remake. It may work on other versions of the game or even other games in the
same engine but this has not been tested. Better compatibility is planned in future releases.

## Features

### Removes horizontal mouse speed scaling relative to pitch
For some reason, the horizontal mouse speed in this game is highest when looking at the horizon and decreases linearly as the player looks up or down. This
functionality is removed by the plugin and the mouse speed is the same no matter the vertical look angle.

### Removes input damping
By default, it actually takes a fraction of a second for the mouse speed to reach maximum from a "standstill" (i.e. not moving the mouse)[^1], causing the
player to experience so-called negative mouse acceleration. The plugin removes this behavior, allowing the mouse to move at full speed at all times.

### FOV-dependent sensitivity
The mouse sensitivity decreases when the FOV is lower and vice versa. This is most noticeable while aiming, and is intended to make hitting precise shots easier.

### Removes dynamic difficulty
Normally, certain actions in the game suck as firing a weapon, damaging and enemy, or taking damage yourself will cause the difficulty scale of the game to be adjusted slightly. This adds up over the course of a playthrough, resulting in unfairly tanky enemies if you play too well; therefore, the best strategy is to periodically take intentional damage to keep the game's difficulty low. This is undesireable because it actually rewards the player for playing worse, and lowers the skill ceiling by not allowing the player to get a feel for how many hits it takes to defeat each enemy. This mod removes this feature, although scripted difficulty changes are still present. For example, the difficult setting starts out low and then increases upon entering RPD.

## Installation
### Prerequisites
* A compatible game (see above)
* [REFramework](https://github.com/praydog/REFramework)

Install REFramework and run the game once. Download the version of the plugin you want from the Releases page and place REFix.dll into the reframework\plugins
directory located in the game directory.

[^1]: It's actually a little more complicated than this. All mouse movement is scaled by a factor that changes dynamically based on how much the mouse is
already moving. This plugin removes this behavior entirely by making it so that the mouse speed scaling factor is effectively constant.
