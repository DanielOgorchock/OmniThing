# OmniThing
A cross-platform successor to ST_Anything
## Design
The goal of OmniThing to to create a modular home automation device solution for several platforms. The plan is to eventually support Arduino, Linux (including embedded Linux devices such as Raspberry Pi and Beaglebone), and Windows.

One of the biggest limitations of the ST_Anything library is that its devices are rather rigid, not giving users a simple way to combine capabilities without needing to create custom classes. Furthermore, it has little to no concept of sensors with the same *capability*. *Capabilities* are to be a major focus of the OmniThing library. 

An example of the potential benefits of using a capability-based architecture can be seen in the following hypothetical device: an auto-irigation controller. Such a device would need to collect soil moisture readings and react accordingly by opening a water valve when the soil is too dry. There are myriad moisture sensors, and likely even more ways to control the flow of water. With *capabilities*, the auto-irrigation controller could simply be composed of a numerical value reader (to get moisture levels) and an actuator (to provide water flow). The specifics of what devices are being used are abstracted behind this layer of abstraction, making the devices far more flexible out of the box.

### Input Value Types
The following list shows the current planned input value types. These are the most fundamental level of values from which sensors will operate.
* Bool
  * Simple True/False
  * Example: A digital GPIO input
* Float
  * Floating point value
  * Example: Analog voltage reading
* Integer
  * Undecided whether to split this up further (i.e. byte, long, signed, unsigned, etc.)
 

