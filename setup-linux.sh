#!bin/bash

set -e

cd dependencies/glew
sudo make
cd ../../

./vendor/bin/scripts/premake/premake5 gmake

