var blankConfiguration = {};
var configuration = {};

var rawJson = "";

var compositePeriphs_All = {};
var inputFloats_All = {};
var networkSenders_All = {};
var outputStrings_All = {};
var devices_All = {};
var inputUInts_All = {};
var outputBools_All = {};
var outputVoids_All = {};
var inputBools_All = {};
var networkReceivers_All = {};
var outputFloats_All = {};

var configAltered = false;

$(window).on('load', function(){
    $.getJSON("blank_config.json", function(data){
        blankConfiguration = data;
        resetConfig();
        configAltered = false;
    });

    objectFromFile("compositePeriphs_All",  "config/composite_peripherals.json");
    objectFromFile("inputFloats_All",       "config/input_floats.json");
    objectFromFile("networkSenders_All",    "config/network_senders.json");
    objectFromFile("outputStrings_All",     "config/output_strings.json");
    objectFromFile("devices_All",           "config/devices.json");
    objectFromFile("inputUInts_All",        "config/input_uints.json");
    objectFromFile("outputBools_All",       "config/output_bools.json");
    objectFromFile("outputVoids_All",       "config/output_voids.json");
    objectFromFile("inputBools_All",        "config/input_bools.json");
    objectFromFile("networkReceivers_All",  "config/network_receivers.json");
    objectFromFile("outputFloats_All",      "config/output_floats.json");

    $("#buttonResetJson").click(resetConfig);

    $("#buttonImportJson").click(loadConfig);

    $(window).bind('beforeunload', function(){
        if(configAltered)
        {
            return true;
        }
        return undefined;
    });
});

var objectFromFile = function(objectName, filename){
    $.getJSON(filename, function(data){
        window[objectName] = data;
    });
}

var resetConfig = function(){
    configuration = Object.assign(blankConfiguration); 
    updateRawConfig();
}

var updateRawConfig = function(){
    configAltered = true;
    rawJson = JSON.stringify(configuration, null, 4);
    $("#raw_json").text(rawJson);

    renderAll();
}

var renderAll = function(){
    renderDevices();
    renderComposites();
    renderNetworkReceiver();
    renderNetworkSender();
}

var renderDevices = function(){
    var devices = configuration.Devices;
    var length = devices.length;

    var listDiv = $("#list-devices");
    var contentDiv = $("#content-list-devices");

    listDiv.empty();
    contentDiv.empty();

    for(var i = 0; i < length; i++)
    {
        var device = devices[i];

        var selectorId = "selectorDevice-" + device.name;
        var selectorText = device.name;

        var contentId = "contentDevice-" + device.name;
        var contentText = device.type; //TODO: change this

        console.log("Adding device named " + device.name);

        var listItem = $("<a/>", {
            "id": selectorId,
            "class": "list-group-item list-group-item-action",
            "data-toggle": "list",
            "href": "#" + contentId, 
            "role": "tab",
            "aria-control": contentId
        });

        listItem.append(selectorText);
        listDiv.append(listItem);


        var contentItem = $("<div/>", {
            "id": contentId,
            "class": "tab-pane show",
            "role": "tabpanel",
            "aria-labelledby": selectorId
        });

        contentItem.append(contentText);
        contentDiv.append(contentItem);
    }
}

var renderComposites = function(){

}

var renderNetworkReceiver = function(){

}

var renderNetworkSender = function(){

}

var getType = function(objectName, type){
    var collection = window[objectName];
    var length = collection.length;

    for(var i = 0; i < length; i++)
    {
        var tmp = collection[i];
        if(type == tmp.type)
        {
            return tmp;
        }
    }

    return null;
}

var loadConfig = function(){
    var input, file, fr;

    if (typeof window.FileReader !== 'function') {
        alert("The file API isn't supported on this browser yet.");
        return;
    }

    input = document.getElementById('fileImportJson');
    if (!input) {
        alert("Um, couldn't find the fileinput element.");
    }
    else if (!input.files) {
        alert("This browser doesn't seem to support the `files` property of file inputs.");
    }
    else if (!input.files[0]) {
        alert("No file has been selected");
    }
    else {
        file = input.files[0];
        fr = new FileReader();
        fr.onload = receivedText;
        fr.readAsText(file);
    }

    function receivedText(e) {
        try{
            let lines = e.target.result;
            var tmp = JSON.parse(lines); 
            configuration = tmp;
        }
        catch(err){
            alert("Failed to parse json configuration. Are you sure you selected a json file?");
        }
        updateRawConfig();
    }   
}
