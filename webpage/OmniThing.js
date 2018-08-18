const express = require('express')
const app = express()


app.get('/', function(req, res){
    res.sendFile("index.html", {root: __dirname+'/files'})
})

app.use(express.static('files'))











app.listen(process.argv[2], function(){
    console.log('OmniThing web configuration tool listening on port ' + process.argv[2])
})
