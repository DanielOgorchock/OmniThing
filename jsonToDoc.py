#!/bin/python

import sys
import json

def writeElement(element):
    global output
    if not "type" in element:
        print("ERROR: no type found in element")
        return

    output += "### " + element['type'] + "\n"
    if "parameters" in element:
        output += "#### Parameters\n"
        for param in element['parameters']:
            output += "    * " + param['name'] + "\n"
            output += "      * type: " + param['type'] + "\n"
            output += "      * required: " + str(param['required']) + "\n"
            if "default" in param:
                output += "      * default: " + str(param['default']) + "\n"
            output += "      * description: " + param['description'] + "\n"
    
    if "events" in element:
        output += "#### Events\n"
        for event in element['events']:
            output += "    * " + event + "\n"

    if "commands" in element:
        output += "#### Commands\n"
        for cmd in element['commands']:
            output += "    * " + cmd + "\n"

    if "bools" in element:
        output += "#### Bool Attributes\n"
        for attr in element['bools']:
            output+= "    * " + attr + "\n"

    if "floats" in element:
        output += "#### Float Attributes\n"
        for attr in element['floats']:
            output+= "    * " + attr + "\n"

    if "uints" in element:
        output += "#### UInt Attributes\n"
        for attr in element['uints']:
            output+= "    * " + attr + "\n"

    output += "#### Supported Platforms\n"
    for plat in element['supported_platforms']:
        output += "    * " + plat + "\n"

def writeElements(elements):
    for e in elements:
        try:
            writeElement(e)
        except:
            print(str(e))

output = ""


output += "# Parameter Documentation\n"
output += "See below for a list of all parameters for everything in the config file\n\n";

json_raw = open('config/devices.json', 'r').read()
devices = json.loads(json_raw)

json_raw = open('config/composite_peripherals.json', 'r').read()
compositePeriphs = json.loads(json_raw)

json_raw = open('config/network_receivers.json', 'r').read()
networkReceivers = json.loads(json_raw)

json_raw = open('config/network_senders.json', 'r').read()
networkSenders = json.loads(json_raw)

json_raw = open('config/input_bools.json', 'r').read()
inputBools = json.loads(json_raw)

json_raw = open('config/input_floats.json', 'r').read()
inputFloats = json.loads(json_raw)

json_raw = open('config/input_uints.json', 'r').read()
inputUInts = json.loads(json_raw)

json_raw = open('config/output_voids.json', 'r').read()
outputVoids = json.loads(json_raw)

json_raw = open('config/output_bools.json', 'r').read()
outputBools = json.loads(json_raw)

json_raw = open('config/output_floats.json', 'r').read()
outputFloats = json.loads(json_raw)

json_raw = open('config/output_strings.json', 'r').read()
outputStrings = json.loads(json_raw)

output += "## Devices\n"
writeElements(devices)

output += "## Composite Peripherals\n"
writeElements(compositePeriphs)

output += "## Network Receivers\n"
writeElements(networkReceivers)

output += "## Network Senders\n"
writeElements(networkSenders)

output += "## InputBools\n"
writeElements(inputBools)

output += "## InputFloats\n"
writeElements(inputFloats)

output += "## InputUInts\n"
writeElements(inputUInts)

output += "## OutputVoids\n"
writeElements(outputVoids)

output += "## OutputBools\n"
writeElements(outputBools)

output += "## OutputFloats\n"
writeElements(outputFloats)

output += "## OutputStrings\n"
writeElements(outputStrings)

print(output)






