#!/bin/bash

cd /opt/omnithing || exit

mkdir -p backups

# Messy backup rotation technique
# TODO: Do this better
mv backups/backup.json.4 backups/backup.json.5
mv backups/backup.json.3 backups/backup.json.4
mv backups/backup.json.2 backups/backup.json.3
mv backups/backup.json.1 backups/backup.json.2
mv backups/backup.json backups/backup.json.1

cp config.json backups/backup.json

IFS= read -t 0.8 -r -d '' indata

echo "$indata" > config.json
