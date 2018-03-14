#!/bin/env bash

git submodule update --recursive --remote
cd libzippp
make
