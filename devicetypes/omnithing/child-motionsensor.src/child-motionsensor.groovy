/**
 *  Child Motion Sensor
 *
 *  Copyright 2018 Daniel Ogorchock
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
 *    2017-04-10  Dan Ogorchock  Original Creation
 *    2017-08-23  Allan (vseven) Added a generateEvent routine that gets info from the parent device.  This routine runs each time the value is updated which can lead to other modifications of the device.
 *    2018-06-02  Dan Ogorchock  Revised/Simplified for Hubitat Composite Driver Model
 *    2018-06-04  D.J.O			 OmniThing support
 * 
 */
metadata {
	definition (name: "child_MotionSensor", namespace: "OmniThing", author: "Daniel Ogorchock") {
		capability "Motion Sensor"
		capability "Sensor"

		attribute "lastUpdated", "String"
	}

	tiles(scale: 2) {
		multiAttributeTile(name:"motion", type: "generic"){
			tileAttribute ("device.motion", key: "PRIMARY_CONTROL") {
				attributeState "active", label:'${name}', icon:"st.motion.motion.active", backgroundColor:"#00A0DC"
				attributeState "inactive", label:'${name}', icon:"st.motion.motion.inactive", backgroundColor:"#ffffff"
            }
 			tileAttribute("device.lastUpdated", key: "SECONDARY_CONTROL") {
    				attributeState("default", label:'    Last updated ${currentValue}',icon: "st.Health & Wellness.health9")
            }
		}
			
	}

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