# build_app.sh
# 
# This script is a wrapper for Vitis V++ baremetal project build.
# Make sure to run setup.sh first.
# 
# Usage:
# ./build_app.sh <project_name> <app_name> [clean]
# 

# Check arguments and handle clean option
if [ "$#" -lt 2 ]; then
	echo "Usage: $0 <project_name> <app_name> [clean]"
	exit 1
fi

PROJECT_NAME="$1"
APP_NAME="$2"
ACTION="${3:-build}"

# Define source and build directories
SRC_DIR="/home/george/repos/aimbot/lib/${PROJECT_NAME}_vitis/${APP_NAME}/src"
BUILD_DIR="/home/george/repos/aimbot/lib/${PROJECT_NAME}_vitis/${APP_NAME}/build"

if [ "$ACTION" = "clean" ]; then
	echo "Cleaning build for Vitis app: $APP_NAME in project: $PROJECT_NAME"
	cd "$BUILD_DIR"
	bash -c "unset CC CXX ; cmake --build ./ --target clean --verbose"
else
	echo "Building Vitis app: $APP_NAME for project: $PROJECT_NAME"
	bash -c "unset CC CXX ; empyro build_app -s $SRC_DIR -b $BUILD_DIR"
fi
