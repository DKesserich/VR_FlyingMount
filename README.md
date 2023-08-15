# VR_FlyingMount
A simple network friendly Flying Mount system for VR as an Unreal Engine plugin.

The Plugin contains a native Actor class (AMountActor_Base), a base BP sub-class (BP_FlyingMount_Base) that implements a Blueprint Interface (BPI_VR_Interaction), some Unreal Enhanced Input input classes, a basic VR pawn that utilizes the Enhanced input system and implements the VR_Interaction interface. There is also a demo blueprint sub-class of BP_FlyingMount_Base with a render mesh (BP_FlyingMount_Demo). 

Using the plugin requires configuring a new Actor collision channel named "VRInteraction" and a new collision preset named "VRInteractor". 

Any render mesh should have a socket named "Handle_Socket" for the handle collision to attach to.

For demo purposes the visualization of the mount is a vacuum cleaner, but this system is equally usable for something like a flying carpet, or perhaps another floor cleaning implement.

[Video Demo Here](https://youtu.be/ec53YkPCquo)

Controls are as follows:
- Left controller thumbstick handles throttle.
- Grip (Valve Index Controller) or Grip Button (Oculus Touch, Windows MR Controller) to grab the handle of the mount. Steer by moving whichever hand is holding the handle to point where you want to go. Roll by leaning left or right
- Hold either trigger to correct roll (Roll can also be disabled entirely in the defaults of the FlyingMount BP, if that's your preference)
- Releasing the handle will return the mount to level (no pitch, no roll).

Current TODOs:
- Haptics to signal when in the grab radius of the handle and for collision feedback.
