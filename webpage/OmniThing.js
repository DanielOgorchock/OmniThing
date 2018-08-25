const express = require('express')
const app = express()
const cp = require('child_process')

app.use(express.static('files'))
app.use(express.json())

app.get('/', function(req, res){
    res.sendFile("index.html", {root: __dirname+'/files'})
})

app.post('/arduino/header', function(req, res){
    var input = JSON.stringify(req.body);
    var process = cp.exec('../jsonToHeader.py', (error, stdout, stderr) => {
        res.send(stdout);
    });
    process.stdin.write(input);
    process.stdin.end();
});












app.listen(process.argv[2], function(){
    console.log('OmniThing web configuration tool listening on port ' + process.argv[2])
})
