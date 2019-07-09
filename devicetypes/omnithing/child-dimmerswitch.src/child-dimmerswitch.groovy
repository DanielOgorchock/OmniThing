/**
 *  Child Dimmer Switch
 *
 *  Copyright 2019 Daniel J. Ogorchock
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
 *    2019-07-08  D.J.O.         Creation
 * 
 */
metadata {
	definition (name: "child_DimmerSwitch", namespace: "OmniThing", author: "Daniel Ogorchock") {
		capability "Switch"
		capability "Switch Level"
		capability "Actuator"
		capability "Sensor"

		attribute "lastUpdated", "String"
	}

	simulator {

	}

	tiles(scale: 2) {
		multiAttributeTile(name:"switch", type: "lighting", width: 3, height: 4, canChangeIcon: true){
			tileAttribute ("device.switch", key: "PRIMARY_CONTROL") {
				attributeState "off", label: '${name}', action: "switch.on", icon: "st.switches.switch.off", backgroundColor: "#ffffff", nextState:"turningOn"
				attributeState "on", label: '${name}', action: "switch.off", icon: "st.switches.switch.on", backgroundColor: "#00A0DC", nextState:"turningOff"
				attributeState "turningOn", label:'${name}', action:"switch.off", icon:"st.switches.switch.on", backgroundColor:"#00A0DC", nextState:"turningOff"
				attributeState "turningOff", label:'${name}', action:"switch.on", icon:"st.switches.switch.off", backgroundColor:"#ffffff", nextState:"turningOn"
			}
			tileAttribute ("device.level", key: "SLIDER_CONTROL") {
				attributeState "level", action:"switch level.setLevel"
			}
 			tileAttribute("device.lastUpdated", key: "SECONDARY_CONTROL") {
    				attributeState("default", label:'    Last updated ${currentValue}',icon: "st.Health & Wellness.health9")
            }
		}
	}
}

def setLevel(def level) {
    def child_name 	= device.deviceNetworkId.substring(device.deviceNetworkId.indexOf('_') + 1)
    def payload         = [name: child_name, cmd: "setLevel", "level": level]
    parent.sendData(new groovy.json.JsonBuilder(payload).toString())
}

def on() {
    sendData("on")
}

def off() {
    sendData("off")
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
    def nowDay = new Date().format("MMM dd", location.timeZone)
    def nowTime = new Date().format("h:mm a", location.timeZone)
    sendEvent(name: "lastUpdated", value: nowDay + " at " + nowTime, displayed: false)
}

def installed() {
}
