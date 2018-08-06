/**
 *  omni_parent
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
 *  A large portion of this code is taken from or based on that of Dan Ogorchock and Allan (vseven) in the 
 *  ST_Anything library.
 */
metadata {
	definition (name: "omni_parent", namespace: "OmniThing", author: "Daniel Ogorchock") {
		capability "Configuration"
		capability "Refresh"
	}


	simulator {
		// TODO: define status and reply messages here
	}

	tiles (scale: 2){
		standardTile("refresh", "device.refresh", inactiveLabel: false, decoration: "flat", width: 2, height: 2) {
			state "default", label:'Refresh', action: "refresh.refresh", icon: "st.secondary.refresh-icon"
		}
        
		standardTile("configure", "device.configure", inactiveLabel: false, decoration: "flat", width: 2, height: 2) {
			state "configure", label:'Configure', action:"configuration.configure", icon:"st.secondary.tools"
		}
        
        childDeviceTiles("all")
	}
    
    preferences {
        input "ip", "text", title: "IP Address", description: "IP Address in form 192.168.1.226", required: true, displayDuringSetup: true
		input "port", "text", title: "Port", description: "port in form of 8090", required: true, displayDuringSetup: true
		input "mac", "text", title: "MAC Address",  description: "MAC Address in form of 02A1B2C3D4E5", required: true, displayDuringSetup: true
    }
}

// parse events into attributes
def parse(String description) {
	//log.debug "Parsing '${description}'"
    
    def msg = parseLanMessage(description)
    def headerString = msg.header
    def bodyString   = msg.body
    
    log.debug "received body:\n${bodyString}"
    
    if(bodyString.trim() == "ok")
    	return
    
    def bodyObj = null;
    try{
    	bodyObj = new groovy.json.JsonSlurper().parseText(bodyString)
        log.debug "${bodyObj}"
        
        if(bodyObj == null){
        	log.debug "body object not parsed"
            return
        }
    }
    catch(e){
    	log.error("Failed to parse json e = ${e}")
        return
    }
    
    if(bodyObj.containsKey('updates')) //parse updates
    {
    	log.debug "parsing updates array"
    	bodyObj.updates.each{ //iterate over all the updates in array
        	//make sure we were given name and type
            if(!it.containsKey('name'))
            {
            	log.error("name was not supplied")
                return
            }
            if(!it.containsKey('type'))
            {
            	log.error("type was not supplied")
                return
            }
        
        	def child = getChild(it.name)
            if(child == null)
            {
            	log.debug "child with name=${it.name} does not exist."
                createChildDevice(it.name, it.type)
                child = getChild(it.name)
            }
            else
            {
            	log.debug "child with name=${it.name} exists already."
            }
            
            if(child != null) // update the child
            {
            	try{
            		child.parse(it)
                }
                catch(e){
                	log.error("Child parse call failed: ${e}")
                }
            }
        }
    }
    else{
    	log.debug("message has no updates array")
    }

}

// handle commands
def configure() {
	log.debug "Executing 'configure'"
	updateDeviceNetworkID()
}

def refresh() {
	log.debug "Executing 'refresh'"
	// TODO: handle 'refresh' command
}

def installed() {
 	log.debug "Executing 'intalled'"
}

def updated() {
	log.debug "Executing updated()"
    updateDeviceNetworkID()
}

def initialize(){
	log.debug "Executing initialize()"
}

def updateDeviceNetworkID() {
	def formattedMac = mac.toUpperCase()
    formattedMac = formattedMac.replaceAll(":", "")
	log.debug "Executing 'updateDeviceNetworkID' formattedMac == " + formattedMac
    if(device.deviceNetworkId!=formattedMac) {
    	log.debug "setting deviceNetworkID = ${formattedMac}"
        device.setDeviceNetworkId("${formattedMac}")
	}
    //Need deviceNetworkID updated BEFORE we can create Child Devices
	//Have the OmniThing send an updated value for every device attached.  This will auto-created child devices!
	refresh()
}

private getHostAddress() {
    def ip = settings.ip
    def port = settings.port
    
    log.debug "Using ip: ${ip} and port: ${port} for device: ${device.id}"
    return ip + ":" + port
}

def sendData(String data){
	sendEthernet(data)
}

def sendEthernet(message){
	log.debug "Executing 'sendEthernet' ${message}"
	if (settings.ip != null && settings.port != null) {
        sendHubCommand(new physicalgraph.device.HubAction(
            method: "POST",
            path: "/",
            body: message,
            headers: [ HOST: "${getHostAddress()}" ]
        ))
    }
    else {
        state.alertMessage = "OmniThing Parent Device has not yet been fully configured. Click the 'Gear' icon, enter data for all fields, and click 'Done'"
        runIn(2, "sendAlert")   
    }
}

private void createChildDevice(String name, String type) {
	log.debug "Attempting to create child with name=" + name + " type=" + type;
    
    try{
    	addChildDevice("child_${type}", "${device.deviceNetworkId}_${name}", null,
        	[completedSetup: true, label: "${device.displayName} (${name})",
            isComponent: false, componentName: "${name}", componentLabel: "${name}"]);
        log.debug "Created child device with network id: ${device.deviceNetworkId}_${name}"
    }
    catch(e)
    {
    	log.error "Failed to create child device with error = ${e}";
    }
}

private boolean containsChild(String name)
{
	try{
    	def result = false
        childDevices.each{
			if(it.deviceNetworkId == "${device.deviceNetworkId}_${name}")
            {
            	result = true;
            }
        }
        return result;
    }
    catch(e)
    {
    	log.error "Failed to check childDevices: ${e}";
        return true; //prevents trying to create the child, since it'll likely just error
    }
}

private def getChild(String name)
{
	log.debug "Searching for child device with network id: ${device.deviceNetworkId}_${name}"
    def result = null
	try{
        childDevices.each{
        	//log.debug "child: ${it.deviceNetworkId}"
			if(it.deviceNetworkId == "${device.deviceNetworkId}_${name}")
            {
            	result = it;
            }
        }
        return result;
    }
    catch(e)
    {
    	log.error "Failed to find child without exception: ${e}";
        log.debug "NOTE: This might be fixed if you go to the configuration page in the SmartThings app for this OmniThing device and hit save"
        return null;
    }
}