const express = require('express')
const app = express()
const cp = require('child_process')
const fs = require('fs');

app.use(express.static('files'))
app.use(express.json())


cp.exec("rm -rf work/zips work/sketches");

app.get('/', function(req, res){
    res.sendFile("index.html", {root: __dirname+'/files'})
})

app.post('/arduino/header', function(req, res){
    var input = JSON.stringify(req.body, null, 4);
    var process = cp.exec('../jsonToHeader.py', (error, stdout, stderr) => {
        res.send(stdout);
    });
    process.stdin.write(input);
    process.stdin.end();
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
            console.log(cmake_cmd);
            var cmake = cp.exec(cmake_cmd, (error, stdout, stderr) => {
                console.log(stderr);
                console.log(stdout);

                var zip_cmd = "cd " + sketchLocation + "; zip -r ../../../"+zipLocation+'/OmniThing.zip *';
                var zip = cp.exec(zip_cmd, (error, stdout, stderr) => {
                    console.log(stderr);
                    console.log(stdout);
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

    console.log("sketchIndex=" + index);

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
            console.log(stdout);
            
            console.log("Cleaned up zip file and sketch folder");
        });
    });
    
});







app.listen(process.argv[2], function(){
    console.log('OmniThing web configuration tool listening on port ' + process.argv[2])
})
