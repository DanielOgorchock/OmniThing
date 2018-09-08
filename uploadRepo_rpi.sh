#!/bin/bash

remote_host=$1

scp deb/Packages.gz deb/omnithing.deb $remote_host:~/omni_repositories/rpi
