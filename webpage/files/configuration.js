var blankConfiguration = {};
var configuration = {};

var webpageConfig = {};

var platform = null;
var platforms = null;

var rawJson = "";

var saveChangesFuncs = [];


var focusedDeviceId;
var focusedCompositeId;

var allSupported = true;


configObjects = {};

var configObjectsReceived = 0;

var configAltered = false;

$(window).on('load', function(){
    $.getJSON("/configuration/web", function(data){
        webpageConfig = data;
        if(webpageConfig.selfhost)
            console.log("Selfhost mode");
        else
            console.log("Website mode");
    });
    objectFromFile("CompositePeriph",  "config/composite_peripherals.json", configInitialization);
    objectFromFile("InputFloat",       "config/input_floats.json", configInitialization);
    objectFromFile("NetworkSender",    "config/network_senders.json", configInitialization);
    objectFromFile("OutputString",     "config/output_strings.json", configInitialization);
    objectFromFile("Device",           "config/devices.json", configInitialization);
    objectFromFile("InputUInt",        "config/input_uints.json", configInitialization);
    objectFromFile("OutputBool",       "config/output_bools.json", configInitialization);
    objectFromFile("OutputVoid",       "config/output_voids.json", configInitialization);
    objectFromFile("InputBool",        "config/input_bools.json", configInitialization);
    objectFromFile("NetworkReceiver",  "config/network_receivers.json", configInitialization);
    objectFromFile("OutputFloat",      "config/output_floats.json", configInitialization);
    objectFromFile("platforms",        "config/platforms.json", configInitialization);

    $("#buttonResetJson").click(resetConfig);

    $("#buttonImportJson").click(loadConfig);

    $("#buttonDiscardChanges").click(discardChanges);

    $("#buttonSaveChanges").click(function(){
        saveChanges();
        $("#alertSaveChanges").show();
        setTimeout(function(){$("#alertSaveChanges").hide();}, 2000);
    });

    $("#buttonAddDevice").click(function(){
        var devices = configuration.Devices;
        var device = {};
        devices.push(device);
        createNewThingModal("Device", device);
    });

    $("#buttonAddComposite").click(function(){
        var composites = configuration.CompositePeriphs;
        var composite = {};
        composites.push(composite);
        createNewThingModal("CompositePeriph", composite);
    });

    $("#buttonDownloadJson").click(function(){
        downloadJsonConfig("config.json", $("#raw_json").text());
    });

    $("#buttonDownloadHeader").click(function(){
        downloadArduinoHeader();
    });

    $("#buttonDownloadSketch").click(function(){
        downloadArduinoSketch();
    });

    $(window).bind('beforeunload', function(){
        if(configAltered)
        {
            return true;
        }
        return undefined;
    });
});

function remove(array, index) {
    if (index !== -1) {
        array.splice(index, 1);
    }
}

var configInitialization = function(){
    if(configObjectsReceived >= 12)
    {
        $.getJSON("blank_config.json", function(data){
            blankConfiguration = data;

            platforms = configObjects["platforms"];
            console.log("Defaulting to platform at index=" + webpageConfig.platformIndex);
            platform = platforms[webpageConfig.platformIndex];

            var numPlatforms = platforms.length;
            for(var i = 0; i < numPlatforms; ++i)
            {
                var plat = platforms[i];
                var option = $("<option/>");
                option.text(plat.name);
                $("#selectPlatform").append(option);
            }
            $("#selectPlatform").val(platform.name);
            $("#selectPlatform").change(function(){
                var name = $("#selectPlatform").val();
                for(var i = 0; i < numPlatforms; ++i)
                {
                    var plat = platforms[i];
                    if(plat.name == name)
                    {
                        console.log("Switching platform to " + plat.name);
                        platform = plat;
                        break;
                    }
                }

                saveChanges();
            });

            resetConfig();
            configAltered = false;

            if(webpageConfig.selfhost)
            {
                $.getJSON("/configuration/omni", function(data){
                    configuration = data;
                    console.log("Loaded local json");
                    updateRawConfig();
                    configAltered = false;
                });

                $("#divArduinoButtons").hide();
                $("#divArduinoInstructions").hide();
                $("#divSelfhostButtons").show();
                $("#buttonUpdateLocalHeader").show();
                $("#divSelfhostInstructions").show();

                $("#buttonUpdateLocal, #buttonUpdateLocalHeader").click(function(){
                    saveChanges();
                    console.log("Updating local config");
                    $.ajax({
                        method: "POST",
                        url: "/saveconfig",
                        data: JSON.stringify(configuration),
                        contentType: "application/json",
                        success: function(data){
                            console.log("Response: " + data);
                            $("#alertUpdateLocal").show();
                            setTimeout(function(){$("#alertUpdateLocal").hide();}, 2000);
                        }
                    });
                });
            }
        });
    }
}

var typeSupported = function(type){
    var supports = platform.supports;
    var numSupports = supports.length;
    for(var i = 0; i < numSupports; ++i)
    {
        var support = supports[i];
        var supportedPlats = type.supported_platforms;
        var numPlats = supportedPlats.length;
        for(var j = 0; j < numPlats; j++)
        {
            var plat = supportedPlats[j];
            if(plat == "all" || plat == support)
            {
                return true;
            }
        }
    }
    return false;
}

var downloadArduinoHeader = function(){
    $.ajax({
        method: "POST",
        url: "/arduino/header",
        data: JSON.stringify(configuration),
        contentType: "application/json",
        success: function(data){
            downloadJsonConfig("ArduinoJsonConfig.h", data);
        }
    });
}

var downloadArduinoSketch = function(){
    $.ajax({
        method: "POST",
        url: "/arduino/sketch",
        data: JSON.stringify(configuration),
        contentType: "application/json",
        success: function(data){
            console.log("url: " + data.url);
            console.log("sketchIndex: " + data.sketchIndex);
            $("body").append("<iframe src='" + data.url+"?sketchIndex="+data.sketchIndex+ "' style='display: none;' ></iframe>");
        }
    });
}

var objectFromFile = function(objectName, filename, callback){
    $.getJSON(filename, function(data){
        configObjects[objectName] = data;
        ++configObjectsReceived;
        if(callback != null && callback != undefined)
        {
            callback();
        }
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

    saveChangesFuncs = [];
    renderAll();
}

var discardChanges = function(){
    updateRawConfig();
}

var saveChanges = function(){
    console.log("saveChanges()");

    var len = saveChangesFuncs.length;
    for(var i = 0; i < len; ++i)
    {
        saveChangesFuncs[i]();
    }

    var devices = configuration.Devices;
    var numDevices = devices.length;
    for(var i = 0; i < numDevices; ++i)
    {
        var dev = devices[i];
        if(jQuery.isEmptyObject(dev))
        {
            remove(devices, i);
            break;
        }
    }

    var composites = configuration.CompositePeriphs;
    var numComposites = composites.length;
    for(var i = 0; i < numComposites; ++i)
    {
        var com = composites[i];
        if(jQuery.isEmptyObject(com))
        {
            remove(composites, i);
            break;
        }
    }

    updateRawConfig();
}

var escapeName = function(name){
    return name.replace(/ /g, "_");
}

function downloadJsonConfig(filename, text) {
    var element = document.createElement('a');
    element.setAttribute('href', 'data:text/plain;charset=utf-8,' + encodeURIComponent(text));
    element.setAttribute('download', filename);

    element.style.display = 'none';
    document.body.appendChild(element);

    element.click();

    document.body.removeChild(element);
}

var getThingByName = function(name){
    var things = configuration.Devices;
    var numThings = things.length;
    for(var i = 0; i < numThings; ++i)
    {
        var tmp = things[i];
        if(tmp.name == name)
        {
            return tmp;
        }
    }

    things = configuration.CompositePeriphs;
    numThings = things.length;

    for(var i = 0; i < numThings; ++i)
    {
        var tmp = things[i];
        if(tmp.name == name)
        {
            return tmp;
        }
    }
    return null;
}

var createNewThing = function(paramType, thing, thingType, name){
    console.log("createNewThing paramType=" + paramType + "  thingType=" + thingType);
    var configs = configObjects[paramType];
    var config = getType(configs, thingType);

    for(var member in thing) delete thing[member];

    if(paramType == "Device")
    {
        thing.triggers = [];
        thing.subscriptions = [];
    }

    var len = config.parameters.length;
    thing.type = config.type;

    if(name != null && name != undefined)
    {
        if(getThingByName(name) != null || getThingByName(name.replace(/ /g, "_")) != null || getThingByName(name.replace(/_/g, " ")) != null)
        {
            alert("This name is already in use. Choose a unique name.");
            for(var member in thing) delete thing[member];
            return false;
        }
        if(name == "")
        {
            alert("Blank names are not valid. Enter a valid name.");
            for(var member in thing) delete thing[member];
            return false;
        }
        if(name.includes("\\"))
        {
            alert("No backslashes permitted in name");
            for(var member in thing) delete thing[member];
            return false;
        }
        if(name.includes("/"))
        {
            alert("No forwardslashes permitted in name");
            for(var member in thing) delete thing[member];
            return false;
        }

        thing.name = name;
    }

    for(var i = 0; i < len; i++)
    {
        var param = config.parameters[i];
        var pType = param.type;
        if(param.required == true)
        {
            if(   pType == "InputBool" || pType == "InputFloat" || pType == "InputUInt"
               || pType == "OutputVoid" || pType == "OutputBool" || pType == "OutputFloat"
               || pType == "OutputString" || pType == "Device")
            {
                thing[param.name] = {};
            }
            else
            {
                thing[param.name] = null;
            }
        }
    }

    saveChanges();
    return true;
}

var createNewThingModal = function(paramType, thing){
    var modal = $("#modalCreateNewThing");
    var modalTitle = $("#modalCreateNewThingTitle");
    var modalBody = $("#modalCreateNewThingBody");
    var modalFooter = $("#modalCreateNewThingFooter");

    modalTitle.text("Create new " + paramType);

    var selectLabel = $("<label/>", {
        "for": "createNewThingSelect"
    });
    selectLabel.text("Type");

    var select = $("<select/>", {
        "class": "form-control",
        "id": "createNewThingSelect"
    });

    var configs = configObjects[paramType];
    var len = configs.length;
    for(var i = 0; i < len; ++i)
    {
        if(typeSupported(configs[i]))
        {
            var option = $("<option/>");
            option.text(configs[i].type);
            select.append(option);
        }
    }


    modalBody.empty();
    modalBody.append(selectLabel);
    modalBody.append(select);

    var nameInput = null;
    if(paramType == "Device" || paramType == "CompositePeriph")
    {
        var nameFormGroup = $("<div/>", {
            "class": "form-group"
        });

        var nameLabel = $("<label/>", {
            "for": "newThingName"
        });
        nameLabel.text("Name");

        nameInput = $("<input/>", {
            "class": "form-control",
            "type": "text",
            "id": "newThingName"
        });


        nameFormGroup.append(nameLabel);
        nameFormGroup.append(nameInput);
        modalBody.append(nameFormGroup);
    }

    var buttonCreate = $("#buttonCreateNewThing");
    buttonCreate.off("click");
    buttonCreate.click(function(){
        var name = null;
        if(paramType == "Device" || paramType == "CompositePeriph")
        {
            name = nameInput.val();
        }
        if(createNewThing(paramType, thing, select.val(), name))
        {
            $("#buttonCreateNewThingClose").click();
        }
    });

    modal.modal();
}

var renderParam = function(mainContainer, param, thing, uid, renderDepth){
    var pType = param.type;

    var paramId = escapeName(uid)+"-param-"+escapeName(param.name);

    var formGroup = $("<div/>", {
        "class": "form-group",
        "id": paramId
    });

    var labelString = "<strong>" + param.name + "</strong> (type=" + param.type + "&ensp; required=" + param.required;
    if(param.default != undefined && param.default != null)
    {
        labelString += "&ensp; default=" + param.default;
    }
    labelString += "):";
    
    var descriptionString = "<i>" + param.description + "</i>";

    if(   pType == "InputBool" || pType == "InputFloat" || pType == "InputUInt"
       || pType == "OutputVoid" || pType == "OutputBool" || pType == "OutputFloat"
       || pType == "OutputString" || pType == "Device")
    {
        var tmpThing = thing[param.name];
        var thingCard = null;
        var addThingButton = null;
        if(tmpThing == null || tmpThing == undefined)
        {
            thing[param.name] = {};
            tmpThing = thing[param.name];
        }
        if(tmpThing.type == undefined)
        {
            addThingButton = $("<button/>", {
                "class": "btn btn-primary",
                "type": "button"
            });
            addThingButton.append("Create");

            addThingButton.click(function(){
                createNewThingModal(param.type, tmpThing);
            });
        }
        else
        {
            thingCard = renderOmni(mainContainer, tmpThing, configObjects[pType], escapeName(uid)+"_"+escapeName(param.name), renderDepth+1, param.type+": ");
        }


        formGroup.append(labelString);
        formGroup.append("<br/>");
        formGroup.append(descriptionString);
        formGroup.append("<br/>");
        if(thingCard != null)
        {
            formGroup.append(thingCard);
        }
        else
        {
            formGroup.append(addThingButton);
        }
        return formGroup;
    }

    var inputId = escapeName(uid)+"-param-input-"+escapeName(param.name);

    var label = $("<label/>", {
        "for": inputId
    });

    label.append(labelString);
    label.append("<br/>");
    label.append(descriptionString);

    if(pType == "bool")
    {
        var modalElementTrue = $("<div/>", {
            "class": "form-check form-check-inline"
        });

        var trueId = escapeName(uid)+"-param-"+escapeName(param.name)+"-bool-true";
        var inputElementTrue = $("<input/>", {
            "class": "form-check-input",
            "type": "radio",
            "name": escapeName(uid)+"-param-"+escapeName(param.name)+"-bool-name",
            "id": trueId,
            "value": "true" 
        });

        var labelElementTrue = $("<label/>", {
            "class": "form-check-label",
            "for": trueId
        });
        labelElementTrue.append("true");

        if(thing[param.name] == true)
        {
            inputElementTrue.attr("checked", "true");
        }

        modalElementTrue.append(inputElementTrue);
        modalElementTrue.append(labelElementTrue);



        var modalElementFalse = $("<div/>", {
            "class": "form-check form-check-inline"
        });

        var falseId = escapeName(uid)+"-param-"+escapeName(param.name)+"-bool-false";
        var inputElementFalse = $("<input/>", {
            "class": "form-check-input",
            "type": "radio",
            "name": escapeName(uid)+"-param-"+escapeName(param.name)+"-bool-name",
            "id": falseId,
            "value": "false" 
        });

        var labelElementFalse = $("<label/>", {
            "class": "form-check-label",
            "for": falseId
        });
        labelElementFalse.append("false");

        if(thing[param.name] == false)
        {
            inputElementFalse.attr("checked", "true");
        }

        modalElementFalse.append(inputElementFalse);
        modalElementFalse.append(labelElementFalse);

        formGroup.append(labelString);
        formGroup.append("<br/>");
        formGroup.append(descriptionString);
        formGroup.append("<br/>");

        formGroup.append(modalElementTrue);
        formGroup.append(modalElementFalse);

        saveChangesFuncs.push(function(){
            var val = $("input:radio[name=" + escapeName(uid)+"-param-"+escapeName(param.name)+"-bool-name]:checked").val();
            var bVal = null;

            if(val == "true")
            {
                bVal = true;
            }
            else if(val == "false")
            {
                bVal = false;
            }

            thing[param.name] = bVal;
        });

        return formGroup;
    }

    if(pType == "uint")
    {
        var input = $("<input/>", {
            "id": inputId,
            "class": "form-control",
            "type": "number"
        });

        input.val(thing[param.name]);

        formGroup.append(label);
        formGroup.append(input);

        saveChangesFuncs.push(function(){
            try{
                thing[param.name] = parseInt(input.val());
            }
            catch(e){
                alert("Failed to parse uint: " + e);
            }
        });
        return formGroup;
    }

    if(pType == "float")
    {
        var input = $("<input/>", {
            "id": inputId,
            "class": "form-control",
            "type": "number"
        });

        input.val(thing[param.name]);

        formGroup.append(label);
        formGroup.append(input);

        saveChangesFuncs.push(function(){
            try{
                thing[param.name] = parseFloat(input.val());
            }
            catch(e){
                alert("Failed to parse float: " + e);
            }
        });
        return formGroup;
    }

    if(pType == "string")
    {
        var input = $("<input/>", {
            "id": inputId,
            "class": "form-control",
            "type": "text"
        });

        input.val(thing[param.name]);

        formGroup.append(label);
        formGroup.append(input);

        saveChangesFuncs.push(function(){
            var val = input.val();
            if(val != "")
            {
                thing[param.name] = val;
            }
            else
            {
                thing[param.name] = null;
            }
        });
        return formGroup;
    }

    if(pType == "enum")
    {
        var enumVals = param["enum"];
        var enumCount = enumVals.length;

        formGroup.append(labelString);
        formGroup.append("<br/>");
        formGroup.append(descriptionString);
        formGroup.append("<br/>");

        for(var i = 0; i < enumCount; i++)
        {
            var enumVal = enumVals[i];

            var modalElement = $("<div/>", {
                "class": "form-check form-check-inline"
            });

            var modalId = escapeName(uid)+"-param-"+escapeName(param.name)+"-enum-"+enumVal;
            var inputElement = $("<input/>", {
                "class": "form-check-input",
                "type": "radio",
                "name": escapeName(uid)+"-param-"+escapeName(param.name)+"-enum-name",
                "id": modalId,
                "value": enumVal
            });

            var labelElement = $("<label/>", {
                "class": "form-check-label",
                "for": modalId
            });
            labelElement.append(enumVal);

            if(thing[param.name] == enumVal)
            {
                inputElement.attr("checked", "true");
            }

            modalElement.append(inputElement);
            modalElement.append(labelElement);

            formGroup.append(modalElement);
        }


        saveChangesFuncs.push(function(){
            var val = $("input:radio[name=" + escapeName(uid)+"-param-"+escapeName(param.name)+"-enum-name]:checked").val();

            thing[param.name] = val;
        });
        return formGroup;
    }
}

var renderOmni = function(mainContainer, thing, configObject, uid, renderDepth, prepend){
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

    var supportedThing = typeSupported(type);
    var headerClass = "card-header"
    if(supportedThing)
    {
        headerClass += " bg-primary";
    }
    else
    {
        headerClass += " bg-warning";
        allSupported = false;
    }
    var cardHeaderElement = $("<div/>", {
        "class": headerClass
    });

    var headerText = "";
    if(prepend != null && prepend != undefined)
    {
        headerText += prepend;
    }
    headerText += thing.type;

    if(!supportedThing)
    {
        headerText += "&ensp;&ensp; WARNING: This type is not supported on " + platform.name;
    }
    cardHeaderElement.append(headerText);


    var deleteButtonElement = $("<button/>", {
        "class": "btn btn-danger",
        "type": "button",
        "style": "float: right;"
    });
    deleteButtonElement.append("Delete");

    deleteButtonElement.click(function(){
        if(mainContainer != null && renderDepth == 0)
        {
            console.log("Removing " + thing.name);
            saveChanges();
            var len = mainContainer.length;
            for(var i = 0; i < len; ++i)
            {
                if(mainContainer[i].name == thing.name)
                {
                    console.log("Found " + thing.name + " for deletion");
                    remove(mainContainer, i);
                    break;
                }
            }
            updateRawConfig();
        }
        else
        {
            thing.type = undefined; 
            saveChanges();
            for(var member in thing) delete thing[member];
            updateRawConfig();
        }
    });

    cardHeaderElement.append(deleteButtonElement);

    cardElement.append(cardHeaderElement);

    var cardBodyElement = $("<div/>", {
        "class": "card-body"
    });

    var formElement = $("<form/>");

    if(thing.name != null && thing.name != undefined)
    {
        var nameId = escapeName(uid)+"-name";
        var formGroupName = $("<div/>", {
            "class": "input-group",
            "id": nameId
        });

        var prependElement = $("<div/>", {
            "class": "input-group-prepend"
        });
        var buttonElement = $("<button/>", {
            "class": "btn btn-primary",
            "type": "button"
        });

        buttonElement.append("Change Name");

        prependElement.append(buttonElement);
        formGroupName.append(prependElement);

        var nameInputId = escapeName(uid)+"-name-input";
        var inputNameElement = $("<input/>", {
            "id": nameInputId,
            "class": "form-control",
            "type": "text"
        });

        buttonElement.click(function(){
            var name = thing.name;
            var newName = inputNameElement.val();
            if(newName == name)
            {
                console.log("No name change");
                return;
            }
            if(newName.includes("\\"))
            {
                alert("No backslashes permitted in name");
                return;
            }
            if(newName.includes("/"))
            {
                alert("No forwardslashes permitted in name");
                return false;
            }
            
            var testThing = getThingByName(newName);
            if(testThing != null)
            {
                alert("This new name is already in use. Please choose a unique name.");
                return;
            }

            testThing = getThingByName(newName.replace(/ /g, "_"))
            if(testThing != null)
            {
                alert("This new name is already in use. Please choose a unique name.");
                return;
            }

            testThing = getThingByName(newName.replace(/_/g, " "))
            if(testThing != null)
            {
                alert("This new name is already in use. Please choose a unique name.");
                return;
            }

            testThing = getThingByName(name);
            if(testThing == null)
            {
                alert("Couldn't find the name to change. This shouldn't be possible.");
                return;
            }

            testThing.name = newName;

            var selectorIdDev = "selectorDevice-" + escapeName(name);
            if(focusedDeviceId == selectorIdDev)
            {
                console.log("Changing focused device to new name");
                focusedDeviceId = "selectorDevice-" + escapeName(newName);
            }
            var selectorIdCom = "selectorComposite-" + name;
            if(focusedCompositeId == selectorIdCom)
            {
                console.log("Changing focused composite to new name");
                focusedCompositeId = "selectorComposite-" + escapeName(newName);
            }

            saveChanges();

            var devNum = configuration.Devices.length;
            for(var i = 0; i < devNum; ++i)
            {
                var dev = configuration.Devices[i];
                var numSubs = dev.subscriptions.length;
                for(var j = 0; j < numSubs; ++j)
                {
                    var s = dev.subscriptions[j];
                    if(s.source == name)
                    {
                        console.log("Change subscription source for name change");
                        s.source = newName;
                    }
                }
            }

            updateRawConfig();
        });

        inputNameElement.val(thing.name);

        formGroupName.append(inputNameElement);

        formElement.append(formGroupName);
        formElement.append("<br/>");
    }

    // Add triggers card for Devices
    if(thing.triggers != null && thing.triggers != undefined)
    {
        var triggers = thing.triggers;
        var triggersCardElement = $("<div/>", {
            "class": "card border-secondary"
        });

        var triggersCardHeaderElement = $("<div/>", {
            "class": "card-header bg-primary"
        });
        triggersCardHeaderElement.text("Triggers  [Interval, Offset, Command]");

        var triggersAddButton = $("<button/>", {
            "class": "btn btn-success",
            "type": "button",
            "style": "float: right;"
        });
        triggersAddButton.text("Add Trigger");
        triggersAddButton.click(function(){
            triggers.push({interval: null, offset: null, command: null});
            saveChanges();
        });
        triggersCardHeaderElement.append(triggersAddButton);

        triggersCardElement.append(triggersCardHeaderElement);

        var triggersCardBodyElement = $("<div/>", {
            "class": "card-body"
        });
        triggersCardElement.append(triggersCardBodyElement);

        var triggerCount = triggers.length;
        for(var i = 0; i < triggerCount; ++i)
        {
            var trigger = triggers[i];
            var commands = type.commands;
            var triggerFormElement = $("<form/>");
            var triggerRowElement = $("<div/>", {
                "class": "row"
            });
            var triggerIntervalColElement = $("<div/>", {
                "class": "col"
            });
            var triggerOffsetColElement = $("<div/>", {
                "class": "col"
            });
            var triggerCommandColElement = $("<div/>", {
                "class": "col"
            });
            var triggerDeleteColElement = $("<div/>", {
                "class": "col-sm-2"
            });

            var triggerDeleteButton = $("<button/>", {
                "class": "btn btn-danger",
                "type": "button",
                "style": "float: right;"
            });
            triggerDeleteButton.text("Delete");
            triggerDeleteColElement.append(triggerDeleteButton);

            var deleteButtonClick = function(index){
                triggerDeleteButton.click(function(){
                    remove(triggers, index); 
                    saveChanges();
                });
            };

            deleteButtonClick(i);

            triggerFormElement.append(triggerRowElement);
            triggerRowElement.append(triggerIntervalColElement);
            triggerRowElement.append(triggerOffsetColElement);
            triggerRowElement.append(triggerCommandColElement);
            triggerRowElement.append(triggerDeleteColElement);
            var triggerBaseId = escapeName(uid) + "-triggers-" + i;

            var triggerIntervalInputElement = $("<input/>", {
                "class": "form-control",
                "type": "number",
                "placeholder": "Interval (milliseconds)",
                "id": triggerBaseId + "interval"
            });
            triggerIntervalInputElement.val(trigger.interval);
            triggerIntervalColElement.append(triggerIntervalInputElement);

            var triggerOffsetInputElement = $("<input/>", {
                "class": "form-control",
                "type": "number",
                "placeholder": "Offset (milliseconds)",
                "id": triggerBaseId + "offset"
            });
            triggerOffsetInputElement.val(trigger.offset);
            triggerOffsetColElement.append(triggerOffsetInputElement);

            var triggerCommandInputElement = $("<select/>", {
                "class": "form-control",
                "id": triggerBaseId + "command"
            });

            var numCommands = type.commands.length;
            for(var j = 0; j < numCommands; ++j)
            {
                var command = type.commands[j];
                var optionElement = $("<option/>");
                optionElement.text(command);
                triggerCommandInputElement.append(optionElement);
            }

            triggerCommandInputElement.val(trigger.command);
            triggerCommandColElement.append(triggerCommandInputElement);


            triggersCardBodyElement.append(triggerFormElement);
            triggersCardBodyElement.append("<br/>");

            var saveTriggerFunc = function(t, intervalE, offsetE, commandE){
                saveChangesFuncs.push(function(){
                    t.command = commandE.val();
                    try{
                        t.interval = parseInt(intervalE.val())    
                    } catch(e){
                        t.interval = null;
                    }
                    try{
                        t.offset = parseInt(offsetE.val())    
                    } catch(e){
                        t.offset = null;
                    }
                });
            };
            saveTriggerFunc(trigger, triggerIntervalInputElement, triggerOffsetInputElement, triggerCommandInputElement);
        }

        formElement.append(triggersCardElement);
        formElement.append("<br/>");
    }


    // Add subscriptions card for Devices
    if(thing.subscriptions != null && thing.subscriptions != undefined)
    {
        var subscriptions = thing.subscriptions;
        var subscriptionsCardElement = $("<div/>", {
            "class": "card border-secondary"
        });

        var subscriptionsCardHeaderElement = $("<div/>", {
            "class": "card-header bg-primary"
        });
        subscriptionsCardHeaderElement.text("Subscriptions  [Source Device, Source Event, Reaction Command]");

        var subscriptionsAddButton = $("<button/>", {
            "class": "btn btn-success",
            "type": "button",
            "style": "float: right;"
        });
        subscriptionsAddButton.text("Add Subscription");
        subscriptionsAddButton.click(function(){
            subscriptions.push({source: null, "event": null, command: null});
            saveChanges();
        });
        subscriptionsCardHeaderElement.append(subscriptionsAddButton);

        subscriptionsCardElement.append(subscriptionsCardHeaderElement);

        var subscriptionsCardBodyElement = $("<div/>", {
            "class": "card-body"
        });
        subscriptionsCardElement.append(subscriptionsCardBodyElement);

        var subscriptionCount = subscriptions.length;
        for(var i = 0; i < subscriptionCount; ++i)
        {
            var subscription = subscriptions[i];
            var commands = type.commands;

            var sources = []
            var devices = configuration.Devices;
            var numDevices = devices.length;
            for(var j = 0; j < numDevices; ++j)
            {
                var dev = devices[j];
                sources.push(dev.name);
            }

            var subscriptionFormElement = $("<form/>");
            var subscriptionRowElement = $("<div/>", {
                "class": "row"
            });
            var subscriptionSourceColElement = $("<div/>", {
                "class": "col"
            });
            var subscriptionEventColElement = $("<div/>", {
                "class": "col"
            });
            var subscriptionCommandColElement = $("<div/>", {
                "class": "col"
            });
            var subscriptionDeleteColElement = $("<div/>", {
                "class": "col-sm-2"
            });

            var subscriptionDeleteButton = $("<button/>", {
                "class": "btn btn-danger",
                "type": "button",
                "style": "float: right;"
            });
            subscriptionDeleteButton.text("Delete");
            subscriptionDeleteColElement.append(subscriptionDeleteButton);

            var deleteButtonClick = function(index){
                subscriptionDeleteButton.click(function(){
                    remove(subscriptions, index); 
                    saveChanges();
                });
            };

            deleteButtonClick(i);

            subscriptionFormElement.append(subscriptionRowElement);
            subscriptionRowElement.append(subscriptionSourceColElement);
            subscriptionRowElement.append(subscriptionEventColElement);
            subscriptionRowElement.append(subscriptionCommandColElement);
            subscriptionRowElement.append(subscriptionDeleteColElement);
            var subscriptionBaseId = escapeName(uid) + "-subscriptions-" + i;

            var subscriptionSourceInputElement = $("<select/>", {
                "class": "form-control",
                "id": subscriptionBaseId + "source"
            });

            var numSources = sources.length;
            for(var j = 0; j < numSources; ++j)
            {
                var s = sources[j];
                var optionElement = $("<option/>");
                optionElement.text(s);
                subscriptionSourceInputElement.append(optionElement);
            }

            subscriptionSourceInputElement.val(subscription.source);
            subscriptionSourceColElement.append(subscriptionSourceInputElement);

            var subscriptionEventInputElement = $("<select/>", {
                "class": "form-control",
                "id": subscriptionBaseId + "event"
            });

            var populateEvents = function(sourceName, eventE){
                eventE.empty();

                var dev = getDeviceByName(sourceName);
                if(dev == null)
                {
                    console.log("Source device: " + sourceName + " does not exist");
                    return;
                }

                var t = getType(configObjects["Device"], dev.type);
                var events = t.events;
                var numEvents = events.length;
                for(var j = 0; j < numEvents; ++j)
                {
                    var e = events[j];
                    var optionElement = $("<option/>");
                    optionElement.text(e);
                    eventE.append(optionElement);
                }
            };

            var registerChangeCallback = function(subE, eventE)
            {
                subE.change(function(){
                    populateEvents(subE.val(), eventE);
                });
            }

            registerChangeCallback(subscriptionSourceInputElement, subscriptionEventInputElement);


            populateEvents(subscription.source, subscriptionEventInputElement);

            subscriptionEventInputElement.val(subscription["event"]);
            subscriptionEventColElement.append(subscriptionEventInputElement);

            var subscriptionCommandInputElement = $("<select/>", {
                "class": "form-control",
                "id": subscriptionBaseId + "command"
            });

            var numCommands = type.commands.length;
            for(var j = 0; j < numCommands; ++j)
            {
                var command = type.commands[j];
                var optionElement = $("<option/>");
                optionElement.text(command);
                subscriptionCommandInputElement.append(optionElement);
            }

            subscriptionCommandInputElement.val(subscription.command);
            subscriptionCommandColElement.append(subscriptionCommandInputElement);


            subscriptionsCardBodyElement.append(subscriptionFormElement);
            subscriptionsCardBodyElement.append("<br/>");

            var saveSubscriptionFunc = function(t, sourceE, eventE, commandE){
                saveChangesFuncs.push(function(){
                    t.source = sourceE.val();
                    t["event"] = eventE.val();
                    t.command = commandE.val();
                });
            };
            saveSubscriptionFunc(subscription, subscriptionSourceInputElement, subscriptionEventInputElement, subscriptionCommandInputElement);
        }

        formElement.append(subscriptionsCardElement);
        formElement.append("<br/>");
    }

    if(type.type == "InputBoolRef" || type.type == "InputFloatRef" || type.type == "InputUIntRef")
    {
        var compositeNameId = escapeName(uid) + "-compositeNameId";
        var compositeValId = escapeName(uid) + "-compositeValId";
        var compositeNameLabel = $("<label/>", {
            "for": compositeNameId
        });
        var compositeValLabel = $("<label/>", {
            "for": compositeValId
        });

        compositeNameLabel.text("Composite Peripheral");
        compositeValLabel.text("Referenced Value");

        var valArrName = null;
        if(type.type == "InputBoolRef")
        {
            valArrName = "bools";
        }
        if(type.type == "InputFloatRef")
        {
            valArrName = "floats";
        }
        if(type.type == "InputUIntRef")
        {
            valArrName = "uints";
        }

        var compositeNameSelect = $("<select/>", {
            "class": "form-control",
            "id": compositeNameId
        });
        var compositeValSelect = $("<select/>", {
            "class": "form-control",
            "id": compositeValId
        });

        var numCom = configuration.CompositePeriphs.length;
        for(var j = 0; j < numCom; ++j)
        {
            var option = $("<option/>");
            option.text(configuration.CompositePeriphs[j].name);
            compositeNameSelect.append(option);
        }
        compositeNameSelect.val(thing.compositeName);

        var updateValChoices = function()
        {
            compositeValSelect.empty();
            var periph = getCompositeByName(compositeNameSelect.val());
            if(periph == null)
            {
                return;
            }

            var cType = getType(configObjects["CompositePeriph"], periph.type);
            var vals = cType[valArrName];
            var numVals = vals.length;
            for(var j = 0; j < numVals; ++j)
            {
                var option = $("<option/>");
                option.text(vals[j]);
                compositeValSelect.append(option);
            }
        }
        updateValChoices();
        compositeValSelect.val(thing.paramName);

        compositeNameSelect.change(function(){
            updateValChoices();
        });

        saveChangesFuncs.push(function(){
            thing.compositeName = compositeNameSelect.val();
            thing.paramName = compositeValSelect.val();
        });

        formElement.append(compositeNameLabel);
        formElement.append(compositeNameSelect);
        formElement.append(compositeValLabel);
        formElement.append(compositeValSelect);
    }
    else
    {
        for(var i = 0; i < paramsLength; i++)
        {
            var param = parameters[i];
            
            var paramElement = renderParam(mainContainer, param, thing, uid, renderDepth);

            formElement.append(paramElement);
        }
    }


    cardBodyElement.append(formElement);
    cardElement.append(cardBodyElement);

    return cardElement;

}

var renderAll = function(){
    var tempScrollTop = $(window).scrollTop();

    allSupported = true;

    renderDevices();
    renderComposites();
    renderNetworkReceiver();
    renderNetworkSender();


    try{
        $("#"+focusedDeviceId).click();
    }
    catch(e){
        console.log("Failed to focus: " + focusedDeviceId);
    }

    try{
        $("#"+focusedCompositeId).click();
    }
    catch(e){
        console.log("Failed to focus: " + focusedCompositeId);
    }


    $(window).scrollTop(tempScrollTop);


    $("#alertNotSupported").hide();
    if(!allSupported)
    {
        $("#alertNotSupported").show();
    }
    
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

        var selectorId = "selectorDevice-" + escapeName(device.name);
        var selectorText = device.name;

        var contentId = "contentDevice-" + escapeName(device.name);

        console.log("Adding device named " + device.name);

        var listItem = $("<a/>", {
            "id": selectorId,
            "class": "list-group-item list-group-item-action",
            "data-toggle": "list",
            "href": "#" + contentId, 
            "role": "tab",
            "aria-control": contentId
        });

        listItem.click(function(){
            var tmpId = $(this).attr('id');
            focusedDeviceId = tmpId;
        });

        listItem.append(selectorText);
        listDiv.append(listItem);


        var contentItem = $("<div/>", {
            "id": contentId,
            "class": "tab-pane show",
            "role": "tabpanel",
            "aria-labelledby": selectorId
        });

        contentItem.append(renderOmni(configuration.Devices, device, configObjects["Device"], device.name, 0, device.name+": "));
        contentDiv.append(contentItem);
    }
}

var renderComposites = function(){
    var composites = configuration.CompositePeriphs;
    var length = composites.length;

    var listDiv = $("#list-composites");
    var contentDiv = $("#content-list-composites");

    listDiv.empty();
    contentDiv.empty();

    for(var i = 0; i < length; i++)
    {
        var composite = composites[i];

        var selectorId = "selectorComposite-" + escapeName(composite.name);
        var selectorText = composite.name;

        var contentId = "contentComposite-" + escapeName(composite.name);

        console.log("Adding composite peripheral named " + composite.name);

        var listItem = $("<a/>", {
            "id": selectorId,
            "class": "list-group-item list-group-item-action",
            "data-toggle": "list",
            "href": "#" + contentId, 
            "role": "tab",
            "aria-control": contentId
        });

        listItem.click(function(){
            var tmpId = $(this).attr('id');
            focusedCompositeId = tmpId;
        });

        listItem.append(selectorText);
        listDiv.append(listItem);


        var contentItem = $("<div/>", {
            "id": contentId,
            "class": "tab-pane show",
            "role": "tabpanel",
            "aria-labelledby": selectorId
        });

        contentItem.append(renderOmni(configuration.CompositePeriphs, composite, configObjects["CompositePeriph"], composite.name, 0, composite.name+": "));
        contentDiv.append(contentItem);
    }

}

var renderNetworkReceiver = function(){
    cardBody = $("#card-body-nreceiver");
    cardBody.empty();

    if(configuration.NetworkReceiver.type != undefined && configuration.NetworkReceiver.type != null)
    {
        cardBody.append(renderOmni(null, configuration.NetworkReceiver, configObjects["NetworkReceiver"], "nreceiver", 0));
    }
    else
    {
        var addButton = $("<button/>", {
            "id": "button-add-nreceiver",
            "class": "btn btn-primary"
        });
        addButton.append("Add Network Receiver");
        addButton.click(function(){
            createNewThingModal("NetworkReceiver", configuration.NetworkReceiver);        
        });

        cardBody.append(addButton);
    }
}

var renderNetworkSender = function(){
    cardBody = $("#card-body-nsender");
    cardBody.empty();
    if(configuration.NetworkSender.type != undefined && configuration.NetworkSender.type != null)
    {
        cardBody.append(renderOmni(null, configuration.NetworkSender, configObjects["NetworkSender"], "nsender", 0));
    }
    else
    {
        var addButton = $("<button/>", {
            "id": "button-add-nsender",
            "class": "btn btn-primary"
        });
        addButton.append("Add Network Sender");
        addButton.click(function(){
            createNewThingModal("NetworkSender", configuration.NetworkSender);        
        });

        cardBody.append(addButton);
    }
}

var getDeviceByName = function(name){
    var numDev = configuration.Devices.length;
    for(var i = 0; i < numDev; ++i)
    {
        var dev = configuration.Devices[i];
        if(dev.name == name)
        {
            return dev;
        }
    }
    return null;
}

var getCompositeByName = function(name){
    var numCom = configuration.CompositePeriphs.length;
    for(var i = 0; i < numCom; ++i)
    {
        var com = configuration.CompositePeriphs[i];
        if(com.name == name)
        {
            return com;
        }
    }
    return null;
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

            var numDevs = configuration.Devices.length;
            for(var i = 0; i < numDevs; ++i)
            {
                var dev = configuration.Devices[i];
                if(dev.triggers == undefined)
                {
                    dev.triggers = [];
                }
                if(dev.subscriptions == undefined)
                {
                    dev.subscriptions = [];
                }
            }
        }
        catch(err){
            alert("Failed to parse json configuration. Are you sure you selected a json file?");
        }
        updateRawConfig();
    }   
}
