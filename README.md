# VR_FlyingMount
A simple Flying Mount system in Unreal Engine for VR.

The Unreal project contains a new native (C++) actor class named FlyingMount_Base that handles steering and flying, in a networkable manner.

There is also a simple VR Pawn blueprint that demonstrates handling the interaction system, and a blueprint sub-class of FlyingMount_Base.

For demo purposes the visualization of the mount is a vacuum cleaner, but this system is equally usable for something like a flying carpet, or perhaps another floor cleaning implement.

[Video Demo Here](https://youtu.be/ec53YkPCquo)

Controls are as follows:
- Left controller thumbstick handles throttle.
- Grip (Valve Index Controller) or Grip Button (Oculus Touch, Windows MR Controller) to grab the handle of the mount. Steer by moving whichever hand is holding the handle to point where you want to go.
- Hold either trigger to correct roll (Roll can also be disabled entirely in the defaults of the FlyingMount BP, if that's you're preference)
- Releasing the handle will return the mount to level (no pitch, no roll).

Current TODOs:
- Haptics to signal when in the grab radius of the handle and for collision feedback.
- Rolling based on lean.
- Do ownership assignment properly when running while networked.

