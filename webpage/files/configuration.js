var blankConfiguration = {};
var configuration = {};

var rawJson = "";


configObjects = {};


var configAltered = false;

$(window).on('load', function(){
    $.getJSON("blank_config.json", function(data){
        blankConfiguration = data;
        resetConfig();
        configAltered = false;
    });

    objectFromFile("CompositePeriph",  "config/composite_peripherals.json");
    objectFromFile("InputFloat",       "config/input_floats.json");
    objectFromFile("NetworkSender",    "config/network_senders.json");
    objectFromFile("OutputString",     "config/output_strings.json");
    objectFromFile("Device",           "config/devices.json");
    objectFromFile("InputUInt",        "config/input_uints.json");
    objectFromFile("OutputBool",       "config/output_bools.json");
    objectFromFile("OutputVoid",       "config/output_voids.json");
    objectFromFile("InputBool",        "config/input_bools.json");
    objectFromFile("NetworkReceiver",  "config/network_receivers.json");
    objectFromFile("OutputFloat",      "config/output_floats.json");

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
        configObjects[objectName] = data;
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

var renderParam = function(param, thing, uid, renderDepth){
    var pType = param.type;

    if(   pType == "InputBool" || pType == "InputFloat" || pType == "InputUInt"
       || pType == "OutputVoid" || pType == "OutputBool" || pType == "OutputFloat"
       || pType == "OutputString" || pType == "Device")
    {
        return renderOmni(thing[param.name], configObjects[pType], uid, renderDepth+1, param.name+": ");
    }

    var formGroup = $("<div/>", {
        "class": "form-group"
    });

    var inputId = uid+"-param-"+param.name;

    var label = $("<label/>", {
        "for": inputId
    });
    label.append(param.name);

    var input = $("<input/>", {
        "id": inputId,
        "class": "form-control",
        "type": "text"
    });

    formGroup.append(label);
    formGroup.append(input);

    return formGroup;
}

var renderOmni = function(thing, configObject, uid, renderDepth, prepend){
    var type = getType(configObject, thing.type);
    var parameters = type.parameters;
    var paramsLength = parameters.length;

    var cardClass = "card border-secondary"
    if(renderDepth % 2 == 0)
    {
        cardClass += " bg-secondary";
    }

    var cardElement = $("<div/>", {
        "class": cardClass
    });

    var cardHeaderElement = $("<div/>", {
        "class": "card-header bg-primary"
    });

    var headerText = "";
    if(prepend != null && prepend != undefined)
    {
        headerText += prepend;
    }
    headerText += thing.type;

    cardHeaderElement.append(headerText);
    cardElement.append(cardHeaderElement);

    var cardBodyElement = $("<div/>", {
        "class": "card-body"
    });

    var formElement = $("<form/>");

    for(var i = 0; i < paramsLength; i++)
    {
        var param = parameters[i];
        
        var paramElement = renderParam(param, thing, uid, renderDepth);

        formElement.append(paramElement);
    }


    cardBodyElement.append(formElement);
    cardElement.append(cardBodyElement);

    return cardElement;

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

        contentItem.append(renderOmni(device, configObjects["Device"], device.name, 0));
        contentDiv.append(contentItem);
    }
}

var renderComposites = function(){

}

var renderNetworkReceiver = function(){

}

var renderNetworkSender = function(){

}

var getType = function(configObject, type){
    var collection = configObject;
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
