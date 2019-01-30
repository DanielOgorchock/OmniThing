/**
 *  Child Relative Humidity Sensor
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
 *    2017-04-19  Dan Ogorchock  Original Creation
 *    2017-08-23  Allan (vseven) Added a generateEvent routine that gets info from the parent device.  This routine runs each time the value is updated which can lead to other modifications of the device.
 *    2018-06-02  Dan Ogorchock  Revised/Simplified for Hubitat Composite Driver Model
 *    2018-07-15  D.J.O          OmniThing support
 *    2019-01-29  Dan Ogorchock  Added support for Humidity Offset and rounding of data
 * 
 */
metadata {
	definition (name: "child_RelativeHumidityMeasurement", namespace: "OmniThing", author: "Daniel Ogorchock") {
		capability "Relative Humidity Measurement"
		capability "Sensor"

		attribute "lastUpdated", "String"
	}

	preferences {
		section("Prefs") {
			input "humidityOffset", "number", title: "Humidity Offset in Percent", description: "Adjust humidity by this percentage", range: "*..*", displayDuringSetup: false
		}
	}
	
	tiles(scale: 2) {
		multiAttributeTile(name: "humidity", type: "generic", width: 6, height: 4, canChangeIcon: true) {
			tileAttribute("device.humidity", key: "PRIMARY_CONTROL") {
                attributeState("humidity", label:'${currentValue}%', unit:"%", defaultState: true,
                    backgroundColors:[
                        [value: 0, color: "#635C0C"],
                        [value: 16, color: "#EBEB21"],
                        [value: 22, color: "#C7DE6A"],
                        [value: 42, color: "#9AD290"],
                        [value: 64, color: "#44B621"],
                        [value: 80, color: "#3D79D9"],
                        [value: 96, color: "#0A50C2"]
                    ])
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
            float tmpValue = Float.parseFloat(e.value)
            if (humidityOffset) {
                tmpValue = tmpValue + humidityOffset
            }
            // Update device
			tmpValue = tmpValue.round(2)
            log.debug "sending event: name=${e.key} value=${tmpValue}"
            sendEvent(name: e.key, value: tmpValue)
        }
    }
    def nowDay = new Date().format("MMM dd", location.timeZone)
    def nowTime = new Date().format("h:mm a", location.timeZone)
    sendEvent(name: "lastUpdated", value: nowDay + " at " + nowTime, displayed: false)
}

def installed() {
}
