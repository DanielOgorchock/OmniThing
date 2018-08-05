# OmniThing
A cross-platform successor to ST_Anything

ESP8266, ESP32, Raspberry Pi, Linux, Windows

## State of the Project
OmniThing is currently in Alpha. Please keep in mind that at the time of writing this it has only been used by one person, so any bug reports are appreciated.

Contributions to the project are encouraged. A guide to contributing can be found toward the bottom of this README.

People trying out OmniThing coming from ST_Anything may note that fewer Arduino devices are supported. This is due to memory restraints. OmniThing uses much more RAM than ST_Anything due to it using a json configuration. Due to this, OmniThing is not a total replacement of ST_Anything functionality, so people using Arduino Unos and Megas won't have much luck with OmniThing (Mega support could potentially come in the future, but Uno has no chance).

Additionally, OmniThing is still in early development, so it may be less stable than ST_Anything in addition to missing some of the ST_Anything device types at this time.[

Goals for Beta:
* Web-based configuration interface so users don't have to manually edit json
  * If anyone loves web development and has a bunch of spare time, feel free to give this a shot. I've been putting it off.
  * Note: The config directory has json files describing all the json parameters for the various types. The plan is to use these files as input for both documentation and the web configuration page. They are already being used for the documentation in this README.
  * I intend to make the web application have the option of auto-generating the arduino sketch, so people targeting ESP32/ESP8266 don't have to bother with installing CMake and Python.
* I2C support
  * I'd like to have an elegant solution for supporting I2C devices. Not much thought has been put into this yet, but right now I'm thinking it will be somewhat similar to the existing CompositePeripheral framework.
* Better operating system integration
  * For example: an output type that elegantly controls daemons running on a linux system.
  * Pretty much all functionality is possible right now through the CommandExecuter, but that is not an elegant way to achieve certain tasks.
* SPI (maybe?)
* Support for buttons
* Provide installable package for raspberry pi, rather than making everyone build from source (can be slow on older pis)
* Google Home TTS
  * Could be nice to integrate this directly with OmniThing
* Open to further suggestions

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
Network Receivers are how OmniThing receives HTTP messages from the hub.

### Network Senders
Network Senders are how OmniThing sends HTTP messages to a home automation platform (i.e. SmartThings, Hubitat).

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
  
## Build Instructions
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

### Arduino (ESP8266/ESP32)
* Open a terminal and navigate to the cloned repository.
* Run: cmake . -DBUILD_TARGET=arduino -DARDUINO_CONFIG="path/to/your/config.json"
* NOTE: If you have not yet created a configuration file, look at the Json Configuration documentation lower in the README.
* NOTE: Python is required for the above command to work correctly. The json file is converted to a c++ header by a python script.
* Now, the arduino_build directory should have an OmniThing sketch in it (if everything worked correctly).
* Open the sketch in the Arduino IDE and build as normal.

### Raspberry Pi
* Open a terminal and navigate to the cloned repository
* Run: cmake . -DBUILD_TARGET=rpi
* Run: make
* To run OmniThing: ./OmniThing ./path/to/your/config.json

### Linux Computer
* Open a terminal and navigate to the cloned repository
* Run: cmake . -DBUILD_TARGET=linux
* Run: make
* To run OmniThing: ./OmniThing ./path/to/your/config.json

### Windows Computer
* Open Visual Studio.
* Select open folder, and select this repository's folder.
* You should be able to build using Visual Studio's built-in CMake integration.

## SmartThings Setup
NOTE: Right now these instructions are basically ripped directly from ST_Anything, so the pictures are slightly wrong.

- Create an account and/or log into the SmartThings Developers Web IDE.
- Click on "My Device Handlers" from the navigation menu.
- Click on "Settings" from the menu and add my GitHub Repository to your account
  - Owner:  DanielOgorchock
  - Name:   OmniThing
  - Branch: master
- Click on "Update From Repo" from the menu
- Select "ST_Anything (master)" from the list
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
Below is the documentation for everything configured in the json file.

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

### Devices
* ContactSensor
  * Parameters
    * input
      * type: InputBool
      * required: True
      * description: How the device determines contact (i.e. a gpio input pin)
    * invert
      * type: bool
      * required: True
      * description: Whether to invert open/closed
    * constantPoll
      * type: bool
      * required: True
      * description: Whether to constantly poll the input source (usually should be true)
  * Events
    * open
    * closed
    * changed
  * Commands
    * poll
  * Supported Platforms
    * all
* Momentary
  * Parameters
    * output
      * type: OutputVoid
      * required: True
      * description: What the device actuates (i.e. a gpio toggler)
  * Events
    * pushed
  * Commands
    * poll
    * push
  * Supported Platforms
    * all
* MotionSensor
  * Parameters
    * input
      * type: InputBool
      * required: True
      * description: How the device determines motion (i.e. a gpio input pin)
    * invert
      * type: bool
      * required: True
      * description: Whether to invert active/inactive
    * constantPoll
      * type: bool
      * required: True
      * description: Whether to constantly poll the input source (usually should be true)
  * Events
    * active
    * inactive
    * changed
  * Commands
    * poll
  * Supported Platforms
    * all
* RelativeHumidityMeasurement
  * Parameters
    * input
      * type: InputFloat
      * required: True
      * description: How the device determines humidity (i.e. analog input pin)
  * Events
  * Commands
    * poll
  * Supported Platforms
    * all
* Switch
  * Parameters
    * output
      * type: OutputBool
      * required: True
      * description: What the device sets to true/false (i.e. digital output pin)
    * invert
      * type: bool
      * required: True
      * description: Whether or not to invert output
    * initial
      * type: bool
      * required: True
      * description: Initial value to set the switch to at startup
  * Events
    * on
    * off
    * changed
  * Commands
    * poll
    * on
    * off
    * toggle
  * Supported Platforms
    * all
* TemperatureMeasurement
  * Parameters
    * input
      * type: InputFloat
      * required: True
      * description: How the device determines temperature (i.e. analog input pin)
  * Events
  * Commands
    * poll
  * Supported Platforms
    * all
* VoltageMeasurement
  * Parameters
    * input
      * type: InputFloat
      * required: True
      * description: How the device determines voltage (i.e. analog input pin)
  * Events
  * Commands
    * poll
  * Supported Platforms
    * all
### Composite Peripherals
* DhtReader
  * Parameters
    * pin
      * type: uint
      * required: True
      * description: Which pin the dht is connected to
    * pullup
      * type: bool
      * required: False
      * default: False
      * description: Whether to enable internal pullup on pin
    * dht11
      * type: bool
      * required: False
      * default: False
      * description: If you are using a dht11, set this to true
  * Supported Platforms
    * rpi
    * arduino
### Network Receivers
* NetworkManagerEsp32
  * Parameters
    * port
      * type: uint
      * required: True
      * description: Port on which to have http server listen
    * ssid
      * type: string
      * required: False
      * description: Access point ssid, if not given here, specify for NetworkSender
    * password
      * type: string
      * required: False
      * description: Access point password, if not given here, specify for NetworkSender
  * Supported Platforms
    * esp32
* NetworkManagerEsp8266
  * Parameters
    * port
      * type: uint
      * required: True
      * description: Port on which to have http server listen
    * ssid
      * type: string
      * required: False
      * description: Access point ssid, if not given here, specify for NetworkSender
    * password
      * type: string
      * required: False
      * description: Access point password, if not given here, specify for NetworkSender
  * Supported Platforms
    * esp8266
* NetworkReceiverHttpLib
  * Parameters
    * port
      * type: uint
      * required: True
      * description: Port on which to have http server listen
    * ip
      * type: string
      * required: True
      * description: IP for server to listen on (should be this computer's IP)
  * Supported Platforms
    * linux
    * windows
    * rpi
### Network Senders
* NetworkManagerEsp32
  * Parameters
    * port
      * type: uint
      * required: True
      * description: Port on which to send messages to hub
    * ip
      * type: string
      * required: True
      * description: IP of the hub
    * ssid
      * type: string
      * required: False
      * description: Access point ssid, if not given here, specify for NetworkReceiver
    * password
      * type: string
      * required: False
      * description: Access point password, if not given here, specify for NetworkReceiver
  * Supported Platforms
    * esp32
* NetworkManagerEsp8266
  * Parameters
    * port
      * type: uint
      * required: True
      * description: Port on which to send messages to hub
    * ip
      * type: string
      * required: True
      * description: IP of the hub
    * ssid
      * type: string
      * required: False
      * description: Access point ssid, if not given here, specify for NetworkReceiver
    * password
      * type: string
      * required: False
      * description: Access point password, if not given here, specify for NetworkReceiver
  * Supported Platforms
    * esp8266
* NetworkSenderHttpLib
  * Parameters
    * ip
      * type: string
      * required: True
      * description: Hub's IP address
    * port
      * type: uint
      * required: True
      * description: Hub's port
  * Supported Platforms
    * linux
    * windows
    * rpi
### InputBools
* InputBoolRef
  * Parameters
    * compositeName
      * type: string
      * required: True
      * description: Name of the composite peripheral being referenced
    * paramName
      * type: string
      * required: True
      * description: Name of the composite peripheral's bool attribute being referenced
  * Supported Platforms
    * all
* DigitalInputPinArduino
  * Parameters
    * pin
      * type: uint
      * required: True
      * description: Which input pin to use
    * invert
      * type: bool
      * required: True
      * description: Whether or not to invert the pin value
    * pullup
      * type: bool
      * required: True
      * description: Whether or not to enable internal pullup
  * Supported Platforms
    * arduino
* DigitalInputPinRaspberryPi
  * Parameters
    * pin
      * type: uint
      * required: True
      * description: Which input pin to use
    * invert
      * type: bool
      * required: True
      * description: Whether or not to invert the pin value
    * pinMode
      * type: enum
      * required: True
      * description: Whether or not to enable internal pullup/pulldown
  * Supported Platforms
    * rpi
### InputFloats
* InputFloatRef
  * Parameters
    * compositeName
      * type: string
      * required: True
      * description: Name of the composite peripheral being referenced
    * paramName
      * type: string
      * required: True
      * description: Name of the composite peripheral's float attribute being referenced
  * Supported Platforms
    * all
* AnalogInputPinArduino
  * Parameters
    * pin
      * type: uint
      * required: True
      * description: Which analog input pin to use
    * minVoltage
      * type: float
      * required: True
      * description: Minimum voltage to map to
    * maxVoltage
      * type: float
      * required: True
      * description: Maximum voltage to map to
  * Supported Platforms
    * arduino
### InputUInts
* InputUIntRef
  * Parameters
    * compositeName
      * type: string
      * required: True
      * description: Name of the composite peripheral being referenced
    * paramName
      * type: string
      * required: True
      * description: Name of the composite peripheral's uint attribute being referenced
  * Supported Platforms
    * all
### OutputVoids
* TimedOutputBool
  * Parameters
    * output
      * type: OutputBool
      * required: True
      * description: The output bool to turn on for a timed duration
    * duration
      * type: uint
      * required: True
      * description: How long to leave output on before turning back off
  * Supported Platforms
    * all
* DigitalOutputPinArduino
  * Parameters
    * pin
      * type: uint
      * required: true
      * description: Which pin to use for output
    * initial
      * type: bool
      * required: True
      * description: Initial output value on startup
    * invert
      * type: bool
      * required: True
      * description: Whether or not to invert the pin output
  * Supported Platforms
    * arduino
* DigitalOutputPinRaspberryPi
  * Parameters
    * pin
      * type: uint
      * required: true
      * description: Which pin to use for output
    * initial
      * type: bool
      * required: True
      * description: Initial output value on startup
    * invert
      * type: bool
      * required: True
      * description: Whether or not to invert the pin output
  * Supported Platforms
    * rpi
* CommandExecuter
  * Parameters
    * command
      * type: string
      * required: True
      * description: The command to run in the shell (i.e. "echo Hello")
    * mulithread
      * type: bool
      * required: False
      * description: Set true to run the command in a new thread (NOTE: this is buggy right now)
  * Supported Platforms
    * linux
    * windows
    * rpi
### OutputBools
* OutputBoolToFloat
  * Parameters
    * output
      * type: OutputFloat
      * required: True
      * description: The output float to map this bool to
    * trueVal
      * type: float
      * required: True
      * description: The float value mapped to true
    * falseVal
      * type: float
      * required: True
      * description: The float value mapped to false
  * Supported Platforms
    * all
* DigitalOutputPinArduino
  * Parameters
    * pin
      * type: uint
      * required: true
      * description: Which pin to use for output
    * initial
      * type: bool
      * required: True
      * description: Initial output value on startup
    * invert
      * type: bool
      * required: True
      * description: Whether or not to invert the pin output
  * Supported Platforms
    * arduino
* DigitalOutputPinRaspberryPi
  * Parameters
    * pin
      * type: uint
      * required: true
      * description: Which pin to use for output
    * initial
      * type: bool
      * required: True
      * description: Initial output value on startup
    * invert
      * type: bool
      * required: True
      * description: Whether or not to invert the pin output
  * Supported Platforms
    * rpi
* CommandExecuterSwitch
  * Parameters
    * commandOn
      * type: string
      * required: True
      * description: The command to run in the for true shell (i.e. "echo On")
    * commandOff
      * type: string
      * required: True
      * description: The command to run in the shell for false (i.e. "echo Off")
    * mulithread
      * type: bool
      * required: False
      * description: Set true to run the command in a new thread (NOTE: this is buggy right now)
  * Supported Platforms
    * linux
    * windows
    * rpi
### OutputFloats
* ServoMotor
  * Parameters
    * pin
      * type: uint
      * required: True
      * description: Servo control pin
    * revertTime
      * type: uint
      * required: False
      * description: If set, the servo will revert to initial value after given milliseconds
    * shutoffTime
      * type: uint
      * required: False
      * description: If set, the servo will be detached given milliseconds after being set to a new percent
    * initialPercent
      * type: float
      * required: False
      * default: 50.0
      * description: Initial percentage at startup
    * noStartup
      * type: bool
      * required: False
      * default: False
      * description: If true, the servo will not be initialized at startup
    * minPulse
      * type: uint
      * required: False
      * default: 544
      * description: Servo pulse length associated with 0 percent
    * maxPulse
      * type: uint
      * required: False
      * default: 2400
      * description: Servo pulse length associated with 100 percent
  * Supported Platforms
    * rpi
    * arduino
### OutputStrings
NONE YET

## Contributing to OmniThing
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


  
