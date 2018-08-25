#!/bin/python

import sys
import json

names = []
maxLength = 0

def writeObject(name, obj, objName):
    global output
    global maxLength
    string = json.dumps(obj, indent=4)
    output += "const char " + name + "[] PROGMEM =\n"

    tmpStr = "{\"" + objName + "\":\n" + string + "}"

    output += "R\"RAWSTR(" + tmpStr + ")RAWSTR\";\n\n"
    maxLength = max(maxLength, len(tmpStr) + 2)
    names.append(name)

if len(sys.argv) != 3 and len(sys.argv) != 1:
    print("Usage: ./jsonToHeader.py [input_json_file] [output_header_file]")
    exit()
    
input_filename = None
output_filename = None
stdio = False

if len(sys.argv) == 3:
    input_filename = sys.argv[1]
    output_filename = sys.argv[2]
else:
    input_filename = "stdin"
    output_filename = "stdout"
    stdio = True

print("\nExecuting jsonToHeader.py...\n")

print("input_file  = "+ input_filename)
print("output_file = "+ output_filename)
print("\nParsing the json...\n")

json_raw = None
if stdio:
    json_raw = sys.stdin.read()
else:
    json_raw = open(input_filename, 'r').read()

json_dict = json.loads(json_raw)

output = "#ifndef OMNI_ARDUINOJSONCONFIG_H\n#define OMNI_ARDUINOJSONCONFIG_H\n\n#include <Arduino.h>\n\n"
output += "namespace omni\n{\n"

if "NetworkReceiver" in json_dict:
    print("NetworkReceiver found")
    writeObject("NetworkReceiver", json_dict["NetworkReceiver"], "NetworkReceiver")

if "NetworkSender" in json_dict:
    print("NetworkSender found")
    writeObject("NetworkSender", json_dict["NetworkSender"], "NetworkSender")

if "CompositePeriphs" in json_dict:
    print("CompositePeriphs found")
    writeObject("CompositePeriphs", json_dict["CompositePeriphs"], "CompositePeriphs")

if "Devices" in json_dict:
    print("Devices found")

    count = 0
    for dev in json_dict["Devices"]:
        writeObject("Device_"+str(count), [dev], "Devices")
        count += 1

num_strings = len(names)
print("Total number of progmem strings: " + str(num_strings))

output += "\n\nconst char* const Config_Json_Strings[] PROGMEM = {\n"
for name in names:
    output += "        " + name + ",\n"
output = output[:-2] + "\n"
output += "};\n\n"

output += "const unsigned int Num_Json_Strings = " + str(num_strings) + ";\n\n"

output += "const unsigned int Max_Json_String_Length = " + str(maxLength) + ";\n\n"

output += "}\n\n#endif\n"

print("Max string length: " + str(maxLength))


print("\nWriting output to header file...\n")
if stdio:
    sys.stdout.write(output)
else:
    open(output_filename, "w").write(output)


