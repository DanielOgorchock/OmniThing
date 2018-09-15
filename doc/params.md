# Parameter Documentation
See below for a list of all parameters for everything in the config file

## Devices
### ContactSensor
##### Parameters
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
##### Events
* open
* closed
* changed
##### Commands
* poll
##### Supported Platforms
* all
### Momentary
##### Parameters
* output
  * type: OutputVoid
  * required: True
  * description: What the device actuates (i.e. a gpio toggler)
##### Events
* pushed
##### Commands
* poll
* push
##### Supported Platforms
* all
### MotionSensor
##### Parameters
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
##### Events
* active
* inactive
* changed
##### Commands
* poll
##### Supported Platforms
* all
### RelativeHumidityMeasurement
##### Parameters
* input
  * type: InputFloat
  * required: True
  * description: How the device determines humidity (i.e. analog input pin)
##### Events
##### Commands
* poll
##### Supported Platforms
* all
### Switch
##### Parameters
* output
  * type: OutputBool
  * required: True
  * description: What the device sets to true/false (i.e. digital output pin)
* input
  * type: InputBool
  * required: False
  * description: If this OPTIONAL input parameter is created, the switch device will not "trust" the value it wrote to its output. Instead, it will report the value it reads from this input bool to SmartThings. (i.e. a switch that can be manually turned off/on)
* invert
  * type: bool
  * required: True
  * description: Whether or not to invert output
* initial
  * type: bool
  * required: True
  * description: Initial value to set the switch to at startup
* ignoreRedundant
  * type: bool
  * required: False
  * default: False
  * description: If set to true, the switch will ignore commands to change its state if it already believes it is in that state. For example, it would ignore an on command if it deems itself to already be on.
##### Events
* on
* off
* changed
##### Commands
* poll
* on
* off
* toggle
##### Supported Platforms
* all
### TemperatureMeasurement
##### Parameters
* input
  * type: InputFloat
  * required: True
  * description: How the device determines temperature (i.e. analog input pin)
##### Events
##### Commands
* poll
##### Supported Platforms
* all
### VoltageMeasurement
##### Parameters
* input
  * type: InputFloat
  * required: True
  * description: How the device determines voltage (i.e. analog input pin)
##### Events
##### Commands
* poll
##### Supported Platforms
* all
### SwitchLevel
##### Parameters
* output
  * type: OutputFloat
  * required: True
  * description: The output device
* initial
  * type: float
  * required: True
  * description: Initial percent level value (0, 100)
##### Events
* changed
##### Commands
* poll
* setLevel
##### Supported Platforms
* all
## Composite Peripherals
### DhtReader
##### Parameters
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
  * description: If you are using a dht11, set this to true (any other dht = false)
##### Bool Attributes
##### Float Attributes
* temperature
* humidity
##### UInt Attributes
##### Supported Platforms
* rpi
* arduino
## Network Receivers
### NetworkManagerEsp32
##### Parameters
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
##### Supported Platforms
* esp32
### NetworkManagerEsp8266
##### Parameters
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
##### Supported Platforms
* esp8266
### NetworkReceiverHttpLib
##### Parameters
* port
  * type: uint
  * required: True
  * description: Port on which to have http server listen
##### Supported Platforms
* linux
* windows
* rpi
## Network Senders
### NetworkManagerEsp32
##### Parameters
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
##### Supported Platforms
* esp32
### NetworkManagerEsp8266
##### Parameters
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
##### Supported Platforms
* esp8266
### NetworkSenderHttpLib
##### Parameters
* ip
  * type: string
  * required: True
  * description: Hub's IP address
* port
  * type: uint
  * required: True
  * description: Hub's port
##### Supported Platforms
* linux
* windows
* rpi
## InputBools
### InputBoolRef
##### Parameters
* compositeName
  * type: string
  * required: True
  * description: Name of the composite peripheral being referenced
* paramName
  * type: string
  * required: True
  * description: Name of the composite peripheral's bool attribute being referenced
##### Supported Platforms
* all
### DigitalInputPinArduino
##### Parameters
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
##### Supported Platforms
* arduino
### DigitalInputPinRaspberryPi
##### Parameters
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
##### Supported Platforms
* rpi
### CommandReturnCode
##### Parameters
* command
  * type: string
  * required: true
  * description: Command to run and check return code of. A return code of 0 evaluates to true. All other return codes evaluate to false.
##### Supported Platforms
* linux
* rpi
* windows
## InputFloats
### InputFloatRef
##### Parameters
* compositeName
  * type: string
  * required: True
  * description: Name of the composite peripheral being referenced
* paramName
  * type: string
  * required: True
  * description: Name of the composite peripheral's float attribute being referenced
##### Supported Platforms
* all
### AnalogInputPinArduino
##### Parameters
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
##### Supported Platforms
* arduino
## InputUInts
### InputUIntRef
##### Parameters
* compositeName
  * type: string
  * required: True
  * description: Name of the composite peripheral being referenced
* paramName
  * type: string
  * required: True
  * description: Name of the composite peripheral's uint attribute being referenced
##### Supported Platforms
* all
## OutputVoids
### TimedOutputBool
##### Parameters
* output
  * type: OutputBool
  * required: True
  * description: The output bool to turn on for a timed duration
* duration
  * type: uint
  * required: True
  * description: How long to leave output on before turning back off
##### Supported Platforms
* all
### DigitalOutputPinArduino
##### Parameters
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
##### Supported Platforms
* arduino
### DigitalOutputPinRaspberryPi
##### Parameters
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
##### Supported Platforms
* rpi
### CommandExecuter
##### Parameters
* command
  * type: string
  * required: True
  * description: The command to run in the shell (i.e. "echo Hello")
* multithread
  * type: bool
  * required: False
  * description: Set true to run the command in a new thread (NOTE: this is buggy right now)
##### Supported Platforms
* linux
* windows
* rpi
## OutputBools
### OutputBoolToFloat
##### Parameters
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
##### Supported Platforms
* all
### DigitalOutputPinArduino
##### Parameters
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
##### Supported Platforms
* arduino
### DigitalOutputPinRaspberryPi
##### Parameters
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
##### Supported Platforms
* rpi
### CommandExecuterSwitch
##### Parameters
* commandOn
  * type: string
  * required: True
  * description: The command to run in the shell for true (i.e. "echo On")
* commandOff
  * type: string
  * required: True
  * description: The command to run in the shell for false (i.e. "echo Off")
* multithread
  * type: bool
  * required: False
  * description: Set true to run the command in a new thread (NOTE: this is buggy right now)
##### Supported Platforms
* linux
* windows
* rpi
## OutputFloats
### ServoMotor
##### Parameters
* pin
  * type: uint
  * required: True
  * description: Servo control pin
* revertTime
  * type: uint
  * required: False
  * description: If set, the servo will revert to initial value after given milliseconds. Leave this blank to never revert.
* shutoffTime
  * type: uint
  * required: False
  * description: If set, the servo will be detached given milliseconds after being set to a new percent. Leave this blank to never shutoff.
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
##### Supported Platforms
* rpi
* arduino
### PwmOutput
##### Parameters
* pin
  * type: uint
  * required: True
  * description: PWM pin
##### Supported Platforms
* rpi
* esp8266
## OutputStrings

