# Gravity Gun Test

[![Gravity Gun prototype video](http://img.youtube.com/vi/-xHItnCfwwE/0.jpg)](http://www.youtube.com/watch?v=-xHItnCfwwE)

## Overview
This project is primarily created for prototyping the gravity gun from Half Life 2 and testing it out in a range. The program is intended to be structured in an extensible manner. I have used Doxygen style comments wherever possible.
The Gravity Gun is defined by the BP_GravityGun blueprint, which is derived from the AGravityGun C++ class. The AGravityGun class holds most of the logic that the involves the gameplay for the gun and the audio cues and particle effects for it. The AGravityGun is derived from AWeapon class. The AWeapon class is designed to be the base class of all weapons in this test bed and can be picked up or dripped by the player character BP_GordonFreeman.
I have implemented an EventHandler (UGlobalEventHandler) class and the GameSingleton pattern afforded by Unreal Engine 4 so that I can use events. These are currently what control the state of the FPS reticle.

## Features:
- Support for gamepad and keyboard, both
- The gun is detachable and is derived from an extensible AWeapon class, in case more weapons need to be added.

## Directions to use:
- WASD controls to move.
- E or Y-button (XBox controller) to pick up the gun. Please note that you have to within range and the crosshair on the gun, to be able to pick it up.
- Left click or Right trigger to shoot gravity beam.
- Right click or left trigger to pull object, if in range. Click or press button again to drop.
