Generating documentation from json
### Devices
* ContactSensor
  * Parameters
    * name: input
    * type: InputBool
    * required: True
    * description: How the device determines contact (i.e. a gpio input pin)
    * name: invert
    * type: bool
    * required: True
    * description: Whether to invert open/closed
    * name: constantPoll
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
    * name: output
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
    * name: input
    * type: InputBool
    * required: True
    * description: How the device determines motion (i.e. a gpio input pin)
    * name: invert
    * type: bool
    * required: True
    * description: Whether to invert active/inactive
    * name: constantPoll
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
    * name: input
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
    * name: output
    * type: OutputBool
    * required: True
    * description: What the device sets to true/false (i.e. digital output pin)
    * name: invert
    * type: bool
    * required: True
    * description: Whether or not to invert output
    * name: initial
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
    * name: input
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
    * name: input
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
    * name: pin
    * type: uint
    * required: True
    * description: Which pin the dht is connected to
    * name: pullup
    * type: bool
    * required: False
    * default: False
    * description: Whether to enable internal pullup on pin
    * name: dht11
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
    * name: port
    * type: uint
    * required: True
    * description: Port on which to have http server listen
    * name: ssid
    * type: string
    * required: False
    * description: Access point ssid, if not given here, specify for NetworkSender
    * name: password
    * type: string
    * required: False
    * description: Access point password, if not given here, specify for NetworkSender
  * Supported Platforms
    * esp32
* NetworkManagerEsp8266
  * Parameters
    * name: port
    * type: uint
    * required: True
    * description: Port on which to have http server listen
    * name: ssid
    * type: string
    * required: False
    * description: Access point ssid, if not given here, specify for NetworkSender
    * name: password
    * type: string
    * required: False
    * description: Access point password, if not given here, specify for NetworkSender
  * Supported Platforms
    * esp8266
* NetworkReceiverHttpLib
  * Parameters
    * name: port
    * type: uint
    * required: True
    * description: Port on which to have http server listen
    * name: ip
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
    * name: port
    * type: uint
    * required: True
    * description: Port on which to send messages to hub
    * name: ip
    * type: string
    * required: True
    * description: IP of the hub
    * name: ssid
    * type: string
    * required: False
    * description: Access point ssid, if not given here, specify for NetworkReceiver
    * name: password
    * type: string
    * required: False
    * description: Access point password, if not given here, specify for NetworkReceiver
  * Supported Platforms
    * esp32
* NetworkManagerEsp8266
  * Parameters
    * name: port
    * type: uint
    * required: True
    * description: Port on which to send messages to hub
    * name: ip
    * type: string
    * required: True
    * description: IP of the hub
    * name: ssid
    * type: string
    * required: False
    * description: Access point ssid, if not given here, specify for NetworkReceiver
    * name: password
    * type: string
    * required: False
    * description: Access point password, if not given here, specify for NetworkReceiver
  * Supported Platforms
    * esp8266
* NetworkSenderHttpLib
  * Parameters
    * name: ip
    * type: string
    * required: True
    * description: Hub's IP address
    * name: port
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
    * name: compositeName
    * type: string
    * required: True
    * description: Name of the composite peripheral being referenced
    * name: paramName
    * type: string
    * required: True
    * description: Name of the composite peripheral's bool attribute being referenced
  * Supported Platforms
    * all
* DigitalInputPinArduino
  * Parameters
    * name: pin
    * type: uint
    * required: True
    * description: Which input pin to use
    * name: invert
    * type: bool
    * required: True
    * description: Whether or not to invert the pin value
    * name: pullup
    * type: bool
    * required: True
    * description: Whether or not to enable internal pullup
  * Supported Platforms
    * arduino
* DigitalInputPinRaspberryPi
  * Parameters
    * name: pin
    * type: uint
    * required: True
    * description: Which input pin to use
    * name: invert
    * type: bool
    * required: True
    * description: Whether or not to invert the pin value
    * name: pinMode
    * type: enum
    * required: True
    * description: Whether or not to enable internal pullup/pulldown
  * Supported Platforms
    * rpi
### InputFloats
* InputFloatRef
  * Parameters
    * name: compositeName
    * type: string
    * required: True
    * description: Name of the composite peripheral being referenced
    * name: paramName
    * type: string
    * required: True
    * description: Name of the composite peripheral's float attribute being referenced
  * Supported Platforms
    * all
* AnalogInputPinArduino
  * Parameters
    * name: pin
    * type: uint
    * required: True
    * description: Which analog input pin to use
    * name: minVoltage
    * type: float
    * required: True
    * description: Minimum voltage to map to
    * name: maxVoltage
    * type: float
    * required: True
    * description: Maximum voltage to map to
  * Supported Platforms
    * arduino
### InputUInts
* InputUIntRef
  * Parameters
    * name: compositeName
    * type: string
    * required: True
    * description: Name of the composite peripheral being referenced
    * name: paramName
    * type: string
    * required: True
    * description: Name of the composite peripheral's uint attribute being referenced
  * Supported Platforms
    * all
### OutputVoids
* TimedOutputBool
  * Parameters
    * name: output
    * type: OutputBool
    * required: True
    * description: The output bool to turn on for a timed duration
    * name: duration
    * type: uint
    * required: True
    * description: How long to leave output on before turning back off
  * Supported Platforms
    * all
* DigitalOutputPinArduino
  * Parameters
    * name: pin
    * type: uint
    * required: true
    * description: Which pin to use for output
    * name: initial
    * type: bool
    * required: True
    * description: Initial output value on startup
    * name: invert
    * type: bool
    * required: True
    * description: Whether or not to invert the pin output
  * Supported Platforms
    * arduino
* DigitalOutputPinRaspberryPi
  * Parameters
    * name: pin
    * type: uint
    * required: true
    * description: Which pin to use for output
    * name: initial
    * type: bool
    * required: True
    * description: Initial output value on startup
    * name: invert
    * type: bool
    * required: True
    * description: Whether or not to invert the pin output
  * Supported Platforms
    * rpi
* CommandExecuter
  * Parameters
    * name: command
    * type: string
    * required: True
    * description: The command to run in the shell (i.e. "echo Hello")
    * name: mulithread
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
    * name: output
    * type: OutputFloat
    * required: True
    * description: The output float to map this bool to
    * name: trueVal
    * type: float
    * required: True
    * description: The float value mapped to true
    * name: falseVal
    * type: float
    * required: True
    * description: The float value mapped to false
  * Supported Platforms
    * all
* DigitalOutputPinArduino
  * Parameters
    * name: pin
    * type: uint
    * required: true
    * description: Which pin to use for output
    * name: initial
    * type: bool
    * required: True
    * description: Initial output value on startup
    * name: invert
    * type: bool
    * required: True
    * description: Whether or not to invert the pin output
  * Supported Platforms
    * arduino
* DigitalOutputPinRaspberryPi
  * Parameters
    * name: pin
    * type: uint
    * required: true
    * description: Which pin to use for output
    * name: initial
    * type: bool
    * required: True
    * description: Initial output value on startup
    * name: invert
    * type: bool
    * required: True
    * description: Whether or not to invert the pin output
  * Supported Platforms
    * rpi
* CommandExecuterSwitch
  * Parameters
    * name: commandOn
    * type: string
    * required: True
    * description: The command to run in the for true shell (i.e. "echo On")
    * name: commandOff
    * type: string
    * required: True
    * description: The command to run in the shell for false (i.e. "echo Off")
    * name: mulithread
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
    * name: pin
    * type: uint
    * required: True
    * description: Servo control pin
    * name: revertTime
    * type: uint
    * required: False
    * description: If set, the servo will revert to initial value after given milliseconds
    * name: shutoffTime
    * type: uint
    * required: False
    * description: If set, the servo will be detached given milliseconds after being set to a new percent
    * name: initialPercent
    * type: float
    * required: False
    * default: 50.0
    * description: Initial percentage at startup
    * name: noStartup
    * type: bool
    * required: False
    * default: False
    * description: If true, the servo will not be initialized at startup
    * name: minPulse
    * type: uint
    * required: False
    * default: 544
    * description: Servo pulse length associated with 0 percent
    * name: maxPulse
    * type: uint
    * required: False
    * default: 2400
    * description: Servo pulse length associated with 100 percent
  * Supported Platforms
    * rpi
    * arduino
### OutputStrings

