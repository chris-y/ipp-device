#!/bin/bash
# prepare linux distribution
export DIST=../dist/linux
mkdir -p $DIST
cp -a ipr readme.txt license.txt install.sh ipp-template ipp-narrowjobs $DIST

