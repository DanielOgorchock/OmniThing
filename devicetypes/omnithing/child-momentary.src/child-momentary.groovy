/**
 *  Child Momentary
 *
 *  Copyright 2017 Daniel Ogorchock
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
 *  in compliance with the License. You may obtain a copy of the License at:
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software distributed under the License is distributed
 *  on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License
 *  for the specific language governing permissions and limitations under the License.
 *
 *  Change History:
 *
 *    Date        Who            What
 *    ----        ---            ----
 *    2018-06-04  D.J.O.         initial creation
 * 
 */
metadata {
	definition (name: "child_Momentary", namespace: "OmniThing", author: "Daniel Ogorchock") {
		capability "Actuator"
		capability "Momentary"
        capability "Switch"
        capability "Sensor"
	}

	simulator {

	}

	tiles(scale: 2) {
    	standardTile("button", "device.button", width: 6, height: 4) {
			state "default", label: "push", backgroundColor: "#ffffff", action: "push"
		}
	}
}

def on() {
    push()
}

def off() {
    push
}

def push() {
    sendData("push")
}

def sendData(def value) {
    def child_name 	= device.deviceNetworkId.substring(device.deviceNetworkId.indexOf('_') + 1)
    def payload 	= [name: child_name, cmd:value]  
    parent.sendData(new groovy.json.JsonBuilder(payload).toString())  
}

def parse(def update) {
	log.debug "parsing ${update}"
	for( e in update)
    {
    	if(e.key != "name" && e.key != "type")
        {
        	log.debug "sending event: name=${e.key} value=${e.value}"
            sendEvent(name: e.key, value: e.value)
        }
    }
}

def installed() {
}
