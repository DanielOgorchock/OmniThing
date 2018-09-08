#!/bin/bash

mkdir -p logs

# Messy log rotation technique
# TODO: Do this better
mv logs/omnithing.log.4 logs/omnithing.log.5
mv logs/omnithing.log.3 logs/omnithing.log.4
mv logs/omnithing.log.2 logs/omnithing.log.3
mv logs/omnithing.log.1 logs/omnithing.log.2
mv logs/omnithing.log logs/omnithing.log.1

./OmniThing config.json > logs/omnithing.log 2>&1
