#!/bin/bash

cd webpage || exit

node_cmd="nodejs"

command "nodejs"
if test $? -ne 0; then
    echo "no nodejs command; using node"
    node_cmd="node"
fi

cd ..
mkdir -p web_logs

# Messy log rotation technique
# TODO: Do this better
mv web_logs/webpage.log.4 web_logs/webpage.log.5
mv web_logs/webpage.log.3 web_logs/webpage.log.4
mv web_logs/webpage.log.2 web_logs/webpage.log.3
mv web_logs/webpage.log.1 web_logs/webpage.log.2
mv web_logs/webpage.log web_logs/webpage.log.1

cd webpage
$node_cmd OmniThing.js ../webpage_config.json >> ../web_logs/webpage.log 2>&1
