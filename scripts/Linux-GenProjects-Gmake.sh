#!/bin/bash

cd "${0%/*}/../"
chmod +x vendor/bin/premake/linux/premake5
./vendor/bin/premake/linux/premake5 gmake