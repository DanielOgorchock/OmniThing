# OmniThing
**A cross-platform successor to ST_Anything**

**ESP8266, ESP32, Raspberry Pi, Linux, Windows**

OmniThing allows you to integrate your computer or microcontroller with SmartThings. Changing functionality is as simple as editing a json configuration file. Take a look at this README to learn how to get started.

Check out the [OmniThing Website](http://omnithing.net). It is home to a configuration tool that makes setting up OmniThing much easier.

## State of the Project
OmniThing is currently in Alpha. Please keep in mind that at the time of writing this it has only been used by one person, so any bug reports are appreciated.

Contributions to the project are encouraged. A guide to contributing can be found toward the bottom of this README.

People trying out OmniThing coming from ST_Anything may note that fewer Arduino devices are supported. This is due to memory restraints. OmniThing uses much more RAM than ST_Anything due to it using a json configuration. Due to this, OmniThing is not a total replacement of ST_Anything functionality, so people using Arduino Unos and Megas won't have much luck with OmniThing (Mega support could potentially come in the future, but Uno has no chance).

Additionally, OmniThing is still in early development, so it may be less stable than ST_Anything in addition to missing some of the ST_Anything device types at this time.

Finally, unlike ST_Anything, OmniThing does not require the end user to edit actual source code. Instead of adding stuff to an Arduino sketch, everything is done through the OmniThing json configuration file. OmniThing parses this file and dynamically creates the desired devices at startup.

Goals for Beta:
* ~~Web-based configuration interface so users don't have to manually edit json~~
  * ~~If anyone loves web development and has a bunch of spare time, feel free to give this a shot. I've been putting it off.~~
  * ~~Note: The config directory has json files describing all the json parameters for the various types. The plan is to use these files as input for both documentation and the web configuration page. They are already being used for the documentation in this README.~~
  * ~~I intend to make the web application have the option of auto-generating the arduino sketch, so people targeting ESP32/ESP8266 don't have to bother with installing CMake and Python.~~
  * ~~Support in-place configuration editing on Linux-based platforms such as raspberry pi~~
* I2C support
  * I'd like to have an elegant solution for supporting I2C devices. Not much thought has been put into this yet, but right now I'm thinking it will be somewhat similar to the existing CompositePeripheral framework.
* Better operating system integration
  * For example: an output type that elegantly controls daemons running on a linux system.
  * Pretty much all functionality is possible right now through the CommandExecuter, but that is not an elegant way to achieve certain tasks.
* SPI (maybe?)
* Support for buttons
* ~~PWM support~~
  * ~~ESP32 PWM support~~
* New Devices
  * ~~SwitchLevel~~
  * ~~DimmerSwitch~~
  * DoorControl
* ~~Give Switch device an optional input parameter~~
* Give parent device handler the presence capability, signaling whether the OmniThing device is online or not
* New Inputs
  * InputFloatToBool (InputBool)
  * ~~Process return code (InputUInt/InputBool)~~
* New Outputs
  * CommandExecuter that accepts input arguments or stdin
    * Have to be careful about vulnerabilities with this one though
* SSDP support
  * Make OmniThing discoverable, eliminating the need to manually add a new device in the ST IDE
* ~~Provide installable package for raspberry pi, rather than making everyone build from source (can be slow on older pis)~~
* Google Home TTS
  * Could be nice to integrate this directly with OmniThing
  * Might not really be necessary though, since Google Assistant Relay can already do this. Maybe the way to go is to have OmniThing use Google Assistant Relay.
* Hubitat support
  * ~~I think only the parent device handler needs to be updated. Since I don't have a hubitat, this sounds likes a job for Ogiewon.~~
  * Investigate merging the ST and Hubitat parent device handlers (may be tricky)
* Open to further suggestions

### Version History
#### Alpha
- 0.8.2
  - Add optional InputFloat to SwitchLevel and DimmerSwitch
- 0.8.1
  - Fix segfault caused by SwitchLevel and DimmerSwitch dereferencing null pointer
- 0.8.0
  - Added CommandExecuterFloat OutputFloat type
    - sets the environment variable "OMNITHING_FLOAT_VAL" to the float value prior to running command
  - Added DimmerSwitch device type and handler
- 0.7.3
  - Fix apt warnings due to OmniThing rpi repo missing files
- 0.7.2
  - Fix parsing of device names with underscores
- 0.7.1
  - Fix parsing of device names with spaces
- 0.7.0
  - Add CommandParser InputFloat
- 0.6.1
  - Fix compilation errors on Arduino platforms
- 0.6.0
  - Added InputDebouncer
  - Fixed bug involving sending floats with too many digits in json strings
  - Added temperature conversion options from the ST_Anything groovy code
- 0.5.1
  - Fixed nasty bug when using a CommandExecuter that could result in writing bogus to logs indefinitely.
- 0.5
  - No longer need to specify ip address in network receiver for windows, linux, and rpi
  - Parse json with a more efficient method that also fixes a buffer overflow bug
  - Added optional input parameter for the Switch device. If provided, the switch will report its current state to SmartThings based on this input rather than what it most recently set the switch output to.
  - Added optional ignoreRedundant parameter to the switch device. If set to true, the switch will ignore commands that will not change the current state of the switch (i.e. an on command when the switch is currently on).
  - Made configuration webpage have the navbar fixed to the top for better usability
  - Added CommandReturnCode InputBool type. Provide a command to run in the shell. A return code of 0 evaluates to true. Any other return code evaluates to false.
  - Hubitat support
  - Added PwmOutputEsp32 which adds pwm support on esp32 using the ledc library.
- 0.4
  - Allow for self-hosted web server configuration tool on linux platforms
  - Cmake install target support on linux systems
  - Created package repo for installing/updating omnithing with apt-get on raspberry pi 
  - OmniThing and its webtool run as systemd services on linux targets
- 0.3
  - Created the OmniThing web configuration tool
  - Fixed several json parsing bugs
- 0.2
  - Added SwitchLevel Device type
  - Added PwmOutput OutputFloat type
  - Fixed build errors on windows
- 0.1
  - Initial release to the public

## Design
The goal of OmniThing to to create a modular home automation device solution for several platforms. OmniThing currently supports ESP8266, ESP32, Raspberry Pi, Linux computers, and Windows computers. It is possible that other embedded Linux platforms will be supported in the future (i.e. Beaglebone).

One of the biggest limitations of the ST_Anything library is that its devices are rather rigid, not giving users a simple way to combine capabilities without needing to create custom classes. Furthermore, it has little to no concept of sensors with the same *capability*. *Capabilities* are to be a major focus of the OmniThing library. 

An example of the potential benefits of using a capability-based architecture can be seen in the following hypothetical device: an auto-irrigation controller. Such a device would need to collect soil moisture readings and react accordingly by opening a water valve when the soil is too dry. There are myriad moisture sensors, and likely even more ways to control the flow of water. With *capabilities*, the auto-irrigation controller could simply be composed of a numerical value reader (to get moisture levels) and an actuator (to provide water flow). The specifics of what components are being used are abstracted, making the irrigation device far more flexible out of the box.

OmniThing is configured entirely through editing a json file. Example configurations can be found in the example_configs directory of this repository. This README includes documentation on all the possible parameters for the config file.

### Devices
Devices correspond to actual SmartThings devices (they will show up in the phone app).

OmniThing is designed to make the Devices as generic as possible. A Switch device knows nothing about GPIO pins. Instead, its configuration requires it be given an OutputBool. It doesn't care if that OutputBool is a GPIO output or something that runs a shell script to turn off a web server. In this way, the framework is very flexible to the specific needs of the user.

### Composite Peripherals
Composite Peripherals provide interfaces to sensors/actuators that are composed of multiple attributes. A perfect example is a DHT22, which provides both temperature and humidity. A TemperatureMeasurement device can reference the temperature attribute of a DHT22 Composite Peripheral. A RelativeHumidityMeasurement can do likewise with humidity.

### Network Receivers
Network Receivers are how OmniThing receives HTTP messages from the hub. Typically this is where you specify things like the port and ip to have the HTTP server listen on.

NOTE: Make sure that the OmniThing device's IP is static. You can typically do this in your router's DHCP server configuration.

### Network Senders
Network Senders are how OmniThing sends HTTP messages to a home automation platform (i.e. SmartThings, Hubitat). Typically this will require you to specify an ip address and port for the hub.

NOTE: Make sure that your hub's IP is static. You can typically do this in your router's DHCP server configuration.

### Input Value Types
The following list shows the current input value types.
* Bool
  * Simple True/False
  * Example: A digital GPIO input
  * Class: InputBool
* Float
  * Floating point value
  * Example: Analog voltage reading
  * Class: InputFloat
* Unsigned Integer
  * Unsigned integer
  * Class: InputUInt 
 
### Output Value Types
The following list shows the current output value types.
* Void
  * Doesn't require any value to execute an action
  * Example: toggle GPIO pin
  * Class: OutputVoid
* Bool
  * Simple True/False
  * Example: A digital GPIO output
  * Class: OutputBool
* Float
  * Floating point value
  * Example: Analog voltage output
  * Class: OutputFloat
* String
  * String output value
  * Example: Run a bash command on Linux computer
  * Class: OutputString
  
## SmartThings Setup
NOTE: Right now these instructions are basically ripped directly from ST_Anything, so the pictures are slightly wrong.

- Create an account and/or log into the SmartThings Developers Web IDE.
- Click on "My Device Handlers" from the navigation menu.
- Click on "Settings" from the menu and add my GitHub Repository to your account
  - Owner:  DanielOgorchock
  - Name:   OmniThing
  - Branch: master
- Click on "Update From Repo" from the menu
- Select "OmniThing (master)" from the list
- Select all of the Parent and Child Device Handlers
- Check the "Publish" check box and click "Execute Update"
- You should now have all of the necessary Device Handlers added to your account

Note: If desired, you can still create all of the Device Handlers manually by copying and pasting code from the GitHub repository files into your ST IDE.  Trust me, the Github integration in SmartThings is so much easier!  And, you will know when new versions of the DHs are available based on the color of each DH in your list of Device Handlers in the IDE.

### Adding a New OmniThing Device
- Click on My Devices from navigation menu
- Click the "+ New Device" button from the menu
- Enter in the following REQUIRED fields
    - Name: anything you want (tip: keep it short)
    - Label: anything you want (tip: keep it short)
    - Device Network ID: any unique name (this will be overwritten with your device's MAC address automatically)
    - Type: "omni_parent"
    - Version: "Self Published"
    - Location: your location (required!)
    - Hub: your hub (required!)

Your screen should look like the following image (but with omni_parent, not PARENT_ST_ANYTHING_ETHERNET):

![screenshot](https://cloud.githubusercontent.com/assets/5206084/25320377/883e2d54-2874-11e7-9d7a-26774894f2c3.PNG)

- Click the Create button at the bottom of the screen
- On your phone's SmartThings app, select Things view, find and select your New Device
  - You may receive a "Device not fully configured" pop-up with the ST app.  We're about to take care of that!
- In the Arduino Device, click the "Gear Icon" in the top right of the screen
- Enter the folowing data from your device
  - IP Address:  must match what you put in your configuration file
  - Port: must match what you put in your configuration file
  - MAC Address: must match your device's mac address
Note:  If you visit the "Recently" page of your Parent Device in your ST App on your phone, you may get an annoying warning that the setup is not complete.  If you've entered all of the required data above, you can safely ignore this message.  Once it scrolls off the 'Recently' list, the pop-ups will stop.

Your screen should look like the following image:

![screenshot](https://cloud.githubusercontent.com/assets/5206084/25320424/f0a8f84c-2874-11e7-820e-98e2cd0ef9d2.PNG)

- Click "Done" at the top right
- Wait a few seconds (you can watch Live Logging in the ST IDE if you'd like) while all of the Child Devices are being automagically created, assuming your device is powered up, running OmniThing,  and connected to your home network 
- Pull/Drag down on the screen to refresh the page which should now have all of your child devices created!

## Hubitat Setup
TODO: Actually make the parent device handler support Hubitat

## Installation Instructions
OmniThing is easy to install on raspberry pi, linux platforms, and arduino platforms.

### Arduino (ESP8266/ESP32)
Just use the OmniThing [website](http://omnithing.net). The configuration tool allows you to download a ready-to-build arduino sketch.

### Raspberry Pi
It is easy to install OmniThing on a Raspberry Pi and keep it updated. You can directly use the apt package manager like you would for any other raspbian package. You just need to add the OmniThing repository.
#### Add the OmniThing repository
`sudo sh -c 'echo "deb http://omnithing.net/repository/rpi ./" >> /etc/apt/sources.list'`
#### Add the omnithing gpg key
`curl http://omnithing.net/repository/rpi/KEY.gpg | sudo apt-key add`
#### Install the omnithing package
`sudo apt-get update && sudo apt-get install omnithing`
#### Updating OmniThing
If a new version of OmniThing is released, you can update to it with the command below:  
`sudo apt-get update && sudo apt-get upgrade`
#### Editing Configuration
You can edit the current OmniThing configuration using the self-hosted webserver running on port 3333. In your web browser, navigate to http://replace_with_your_pi_ip:3333

### Other Linux Platforms
#### Building and Installing
`cd`  
`git clone https://github.com/DanielOgorchock/OmniThing.git`  
`cd OmniThing`  
`cmake . -DBUILD_TARGET=linux -DWEB_CONFIG=webpage/config_linux.json`  
`sudo make install`  
#### Starting Services
`sudo systemctl enable omnithing`  
`sudo systemctl enable omnithing-webserver`  
`sudo systemctl start omnithing`  
`sudo systemctl start omnithing-webserver`  
#### Editing Configuration
You can edit the current OmniThing configuration using the self-hosted webserver running on port 3333.
  
## Build Instructions
The build instructions can be ignored for linux, raspberry pi, and arduino platforms if you only want to use OmniThing. They are only needed if you want to directly edit OmniThing source code. Refer to the installation instructions for those platforms if you don't need to edit source files.
### Prerequisites
* CMake is required to build OmniThing.
  * Windows: [Download Here](https://cmake.org/download/)
  * Raspberry Pi: sudo apt-get install cmake
* If you are building for a Raspberry Pi, OmniThing is dependent on the [pigpio library](http://abyz.me.uk/rpi/pigpio/download.html)
  * If running raspbian, I think you can just 'sudo apt-get install pigpio', but I'm not running raspbian so I don't know for sure.
* If you are building for Arduino (ESP8266/ESP32), the OmniThing build process requires Python to be installed.
* Arduino also obviously requires the Arduino IDE.
* If you are building for Linux, gcc is required.
* If you are building for Windows (to actually run on windows, not to build for Arduino), download [Visual Studio](https://visualstudio.microsoft.com/vs/community/)
  * Note: Be sure to select the CMake integration when installing.

Now, clone this repository and follow the proper instructions below for your target platform.
```bash
git clone https://github.com/DanielOgorchock/OmniThing.git
```

### Arduino (ESP8266/ESP32)
* Note: You can avoid all this for arduino-based platforms by using the configuration tool at [omnithing.net](http://omnithing.net). It allows you to download a complete sketch with your desired configuration.
* Open a terminal and navigate to the cloned repository.
  * On windows run cmd to open a terminal.
  * CMake also has a GUI you can use. I haven't used it myself, but it may prove more user friendly to those who don't like messing around in terminals.
* Run: cmake . -DBUILD_TARGET=arduino -DARDUINO_CONFIG="path/to/your/config.json"
* NOTE: If you have not yet created a configuration file, look at the Json Configuration documentation lower in the README. Also, take a look at the examples in the example_configs directory.
* NOTE: Python is required for the above command to work correctly. The json file is converted to a c++ header by a python script.
* Now, the arduino_build directory should have an OmniThing sketch in it (if everything worked correctly).
* Open the sketch in the Arduino IDE and build as normal.

### Raspberry Pi
* Open a terminal and navigate to the cloned repository
* Run: cmake . -DBUILD_TARGET=rpi
* Run: make
* To run OmniThing: sudo ./OmniThing ./path/to/your/config.json

### Linux Computer
* Open a terminal and navigate to the cloned repository
* Run: cmake . -DBUILD_TARGET=linux
* Run: make
* To run OmniThing: ./OmniThing ./path/to/your/config.json

### Windows Computer
* Open Visual Studio.
* Select open folder, and select this repository's folder.
* You should be able to build using Visual Studio's built-in CMake integration.

## Board Pinouts
  Reference the images below to determine which pin numbers to use for your board.
  Note: Use the numbers following the "GPIO" labels for the ESPs, not the absolute pin position.
  
  ### ESP8266
  ![alt text](https://user-images.githubusercontent.com/5153370/43681147-b835fdf8-9811-11e8-81de-7de1de9099e2.png "ESP8266")
  
  ### ESP32
  ![alt text](https://user-images.githubusercontent.com/5153370/43681164-0e28798e-9812-11e8-87da-94005a30af2f.png "ESP32")
  
  ### Raspberry Pi
  Be sure to pay attention to whether you have a raspberry pi with 26 or 40 pins.
  ![alt text](https://user-images.githubusercontent.com/5153370/43681179-a30f6328-9812-11e8-978a-ce2b810afd14.png "Raspberry Pi")
  ![alt text](https://user-images.githubusercontent.com/5153370/43681196-e757ed66-9812-11e8-9253-eab34e639ede.png "Raspberry Pi")


## Json Parameter Documentation
It is recommended that you create your json configuration with the web-based tool found at [omnithing.net](http://omnithing.net). There's no need to edit any text files yourself unless you really want to.

Below is the documentation for everything configured in the json file.

If you have never used json before, don't worry. It's pretty simple. Take a look [here](https://www.json.org/) to see how the format works.

### Empty Json File
This is the barebones framework for the config file.

Example configuration files can be found in the example_configs directory.
```json
{
    "NetworkReceiver": {

    },

    "NetworkSender": {

    },

    "CompositePeriphs": [
       
    ],

    "Devices": [
        
    ]
}
```

### Shared Device Parameters
Every Device (NOTE: just the devices, not the various inputs/outputs, composites, etc) below also have the following parameters.

#### Name
* A unique identifier for the device
* This is required
```json
{ "name": "thisIsAUniqueName"}
```

#### Triggers
* Every device can have an optional array called "triggers". 
* Triggers will fire periodically based on the provided "interval" of milliseconds. 
* Triggers also require a "command". The device will be given the specified command every time it triggers. The supported commands for each device are listed in their descriptions.
* Additionally, a trigger can have an optional "offset" in milliseconds. This will delay the first time the trigger activates by that many ms.
```json
{
    "triggers": [
        {
            "interval": 10000,
            "command": "poll",
            "offset": 5000
        },
        {
            "interval": 10000,
            "command": "toggle"
        }
    ]
}
```

#### Subscriptions
* Every device can also have an optional array called "subscriptions"
* Subscriptions are similar to triggers, except that they fire based on events from other devices rather than a time interval.
* Requires a "source", which is the name of the device to subscribe to.
* Also requires an "event", which is the name of the emitted event from the "source" we want to react to.
* Finally, requires a "command", which will be executed for this device when the event has been emitted.
* The events each device type emits is documented below.
```json
{
    "subscriptions": [
        {
            "source": "name1",
            "event": "changed",
            "command": "toggle"
        },
        {
            "source": "name2",
            "event": "on",
            "command": "poll"
        }
    ]
}
```

#### The "poll" command
* Pretty much every device should implement the poll command.
* When received, the device will read its current state and send it to the hub.
* Any devices you would like to poll at a set interval should be given a trigger with "command": "poll"

### Parameter Documentation
Click [here](https://github.com/DanielOgorchock/OmniThing/blob/master/doc/params.md) to see all the supported parameters, commands, and events. Use this as a reference when editing your configuration file.

## Contributing to OmniThing
NOTE: Before you read further, I apologize to the current state of documentation within the project's source files (as in, there basically is none). Hopefully it will be improved in the future. At least the directory structure is fairly straight-forward.

OmniThing's modularity makes it pretty easy to add funcionality. If you intend to add something to OmniThing, it is important to keep a few things in mind:

### Only Add Devices if Required
Say you are intending to add support for a new temperature sensor. It may be tempting to create a new Device. This is a bad idea. Devices are meant to be very generic. Instead, a temperature sensor could be implemented as an InputFloat, which would allow the existing TemperatureMeasurement device to utilize it.

### Put the Code in the Proper Folder
OmniThing's directory layout is organized based on platform.
* core - Included for every platform
* arduino - Only used for ESP32/ESP8266
* raspberry_pi - Only used for rpi
* embedded_common - Used by rpi and arduino
* os_common - Used by windows, linux, and rpi

### Use Abstractions when Possible
Try to avoid using platform specific code. OmniThing has many abstractions to aid with this.

* LOG << F("Print output with the LOG macro. It works on all platforms!") << Logger::endl;
  * Try to remember to use the F() macro with LOG. It reduces memory usage on Arduino platforms.
* If you need to use GPIOs, use the ones found in embedded_common if possible. This will work on both arduino and raspberry pi.
  * The DhtReader class is a good example for abstracted GPIO usage.

Also, try not to use String classes unless your code is only ever intended to work on a particular platform. If you want it to run on everything, use good old c strings.

### Update Config
Add your new Device/CompositePeriph/Input/Output/etc to the appropriate json file in the config directory.

### Follow Device Naming Scheme
If your are adding a device (Note: Device, not input/output/compositePeriph) that pretty directly corresponds to a capability in the SmartThings [documentation](https://docs.smartthings.com/en/latest/capabilities-reference.html), please name it after its name there (just like the rest of the current devices).
  
### Avoid Dynamic Memory Allocation
Since OmniThing runs on embedded systems, limit dynamic memory allocation to initialization only if possible (for example: when parsing json configuration). This makes OmniThing stable and less prone to memory leaks and heap fragmentation.
  
### Adding Support for a New Platform
The following is a general checklist for adding a new platform to OmniThing.

OmniThing uses very little platform-dependent code, so there are only a handful of things that need to be implemented for a new platform.

#### Linux-based
This should be pretty easy. All that really needs to be implemented is the GPIO related stuff (digital input, digital output, analog input, pwm output, servo, etc.), which is nicely abstracted for the most part. Take a look in the raspberry_pi code to see how to add support for GPIO.

#### Other
* Network Receiver
* Network Sender
* GPIO Stuff
* Good luck
