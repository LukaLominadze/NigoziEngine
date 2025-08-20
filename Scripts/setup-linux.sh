#!bin/bash

set -e

$ pushd ..
./vendor/bin/scripts/premake/premake5 gmake
$ popd

