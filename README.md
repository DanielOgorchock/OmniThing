# OmniThing
A cross-platform successor to ST_Anything
## Design
The goal of OmniThing to to create a modular home automation device solution for several platforms. OmniThing currently supports ESP8266, ESP32, Raspberry Pi, Linux computers, and Windows computers. It is possible that other embedded Linux platforms will be supported in the future (i.e. Beaglebone).

One of the biggest limitations of the ST_Anything library is that its devices are rather rigid, not giving users a simple way to combine capabilities without needing to create custom classes. Furthermore, it has little to no concept of sensors with the same *capability*. *Capabilities* are to be a major focus of the OmniThing library. 

An example of the potential benefits of using a capability-based architecture can be seen in the following hypothetical device: an auto-irrigation controller. Such a device would need to collect soil moisture readings and react accordingly by opening a water valve when the soil is too dry. There are myriad moisture sensors, and likely even more ways to control the flow of water. With *capabilities*, the auto-irrigation controller could simply be composed of a numerical value reader (to get moisture levels) and an actuator (to provide water flow). The specifics of what components are being used are abstracted, making the irrigation device far more flexible out of the box.

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

  
