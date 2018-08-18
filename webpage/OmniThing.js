const express = require('express')
const app = express()


app.get('/', function(req, res){
    res.send("OmniThing")
})













app.listen(process.argv[2], function(){
    console.log('OmniThing web configuration tool listening on port ' + process.argv[2])
})
