const express = require('express')
const app = express()
const cp = require('child_process')
const stream = require('stream')
const fs = require('fs');
const path = require('path');

app.use(function(req, res, next) {
    if(req.url.includes("Packages")){
        console.log("Repository request made");
    }
    next();
});
app.use("/", express.static('files'))
app.use(express.json())


cp.exec("rm -rf work/zips work/sketches");

if(process.argv.length != 3)
{
    console.log("usage: node[js] Omnithing.js [configfile.json]")
    process.exit();
}

var config = null;
try{
    config = JSON.parse(fs.readFileSync(process.argv[2], 'utf8'));
} catch(e) {
    console.log("Failed to parse json: " + e);
    process.exit();
}

app.get('/', function(req, res){
    console.log("Visitor to homepage");
    res.sendFile("index.html", {root: __dirname+'/files'})
})

app.get('/configuration/web', function(req, res){
    res.send(config);
})

app.post('/arduino/header', function(req, res){
    var input = JSON.stringify(req.body, null, 4);
    var process = cp.exec('../jsonToHeader.py', (error, stdout, stderr) => {
        res.send(stdout);
    });
    process.stdin.write(input);
    process.stdin.end();
    console.log("header file download");
});

var sketchIndex = 0;
app.post('/arduino/sketch', function(req, res){
    var index = sketchIndex;
    sketchIndex++;
    var sketchLocation = 'work/sketches/'+index+'/';
    var zipLocation = 'work/zips/'+index+'/';
    var configFile = sketchLocation+'config.json';


    var createDir = cp.exec('mkdir -p '+sketchLocation+' ; mkdir -p ' + zipLocation, (error, stdout, stderr) => { 
        fs.writeFile(configFile, JSON.stringify(req.body, null, 4), (err) => {
            var cmake_cmd = 'cd ..; cmake . -DBUILD_TARGET=arduino -DARDUINO_BUILD_LOCATION=\'webpage/'+sketchLocation+'OmniThing\' -DARDUINO_CONFIG=\'webpage/'+configFile+'\'';
            //console.log(cmake_cmd);
            var cmake = cp.exec(cmake_cmd, (error, stdout, stderr) => {
                //console.log(stderr);
                //console.log(stdout);

                var zip_cmd = "cd " + sketchLocation + "; zip -r ../../../"+zipLocation+'/OmniThing.zip *';
                var zip = cp.exec(zip_cmd, (error, stdout, stderr) => {
                    //console.log(stderr);
                    //console.log(stdout);
                    res.send({url: '/arduino/zip', "sketchIndex": index});
                });
            });
        });
    });
});


app.get('/arduino/zip', function(req, res){
    var index = parseInt(req.query.sketchIndex);
    if(index == NaN){
        console.log("Invalid index: " + req.query.sketchIndex);
        res.send("Invalid sketchIndex!");
        return;
    }

    console.log("sketch zip download: sketchIndex=" + index);

    var sketchLocation = 'work/sketches/'+index+'/';
    var zipLocation = 'work/zips/'+index+'/';

    res.set({
        'Content-Type': 'application/zip'
    });
    res.sendFile("OmniThing.zip", {root: __dirname+'/'+zipLocation}, function(err){
        cp.exec('rm -rf ' + sketchLocation + ' ; rm -rf ' + zipLocation, (error, stdout, stderr) => {
            if(error)
            {
                console.log(error);
            }
            console.log(stderr);
            //console.log(stdout);
            
            console.log("Cleaned up zip file and sketch folder");
        });
    });
    
});

if(config.selfhost)
{
    var installDir = "/opt/omnithing/"
    app.post('/saveconfig', function(req, res){
        var newJson = JSON.stringify(req.body, null, 4);
        console.log("saving new config.json");

        var updateProc = cp.exec(installDir+"updateConfig.sh", (error, stdout, stderr) => {
            console.log(stdout);
            console.log(stderr);

            console.log("Restarting omnithing service...");
            cp.exec("systemctl restart omnithing", (error, stdout, stderr) => {
                console.log(stdout);
                console.log(stderr);

                res.send("ok");
            });
        });
        var inputStream = new stream.Readable();
        inputStream.push(newJson);
        inputStream.push(null);
        inputStream.pipe(updateProc.stdin);
    });

    app.get('/configuration/omni', function(req, res){
        var omniConfig = JSON.parse(fs.readFileSync("/opt/omnithing/config.json", 'utf8'));
        res.send(omniConfig);
    });
}

if(config.repositories != undefined && config.repositories != null)
{
    var repos = config.repositories;
    var numRepos = repos.length;
    for(var i = 0; i < numRepos; ++i)
    {
        var rep = repos[i];
        app.use("/repository/"+rep.name, express.static(rep.path));
    }
}




app.listen(config.port, function(){
    console.log('OmniThing web configuration tool listening on port ' + config.port)
})
