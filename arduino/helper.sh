#!/bin/bash

###
### global settings
###
ARDUINO_DIR="/usr/share/arduino"
HARDWARE_DIRS=("${ARDUINO_DIR}/hardware")
TOOLS_DIRS=("${ARDUINO_DIR}/tools-builder" "${ARDUINO_DIR}/hardware/tools/avr")
LIBRARIES_DIRS=()

###
### user settings
###
HARDWARE_DIRS+=("${HOME}/.arduino15/packages")
TOOLS_DIRS+=("${HOME}/.arduino15/packages")

###
### project vars
###
SKETCH="${PWD}/sketches/main/main.ino"
FQBN="arduino:avr:uno"
#FQBN="arduino:sam:arduino_due_x_dbg"
LIBRARIES_DIRS+=("${PWD}/libraries")
BUILDPATH="${PWD}/.build"


cmd_build ()
{
  mkdir -p "${BUILDPATH}"


  local args=()
  args+=("-logger=human")
  for idx in "${HARDWARE_DIRS[@]}"; do
    args+=("-hardware=${idx}")
  done
  for idx in "${TOOLS_DIRS[@]}"; do
    args+=("-tools=${idx}")
  done
  args+=("-built-in-libraries=${ARDUINO_DIR}/libraries")
  for idx in "${LIBRARIES_DIRS[@]}"; do
    args+=("-libraries=${idx}")
  done
  args+=("-fqbn=${FQBN}")
  args+=("-ide-version=10607")
  args+=("-build-path=${BUILDPATH}")
  args+=("-warnings=none")
  args+=("-prefs=build.warn_data_percentage=75")
  args+=("-verbose")

  #"${ARDUINO_DIR}/arduino-builder" -dump-prefs "${args[@]}" "${SKETCH}"
  "${ARDUINO_DIR}/arduino-builder" -compile "${args[@]}" "${SKETCH}"
}
cmd_clean ()
{
  rm -rf "${BUILDPATH}"
}
cmd_help ()
{
    echo "Usage:"
    echo "    helper.sh <command>"
    echo ""
    echo "  Commands:"
    echo "    build     Compile and verify sketch"
    echo "    clean     Clean local build artifacts"
    echo "    help      Displays this information"
    echo "    upload    Upload sketch to device"
    echo ""
}
cmd_upload ()
{
  echo
}

cmd="${1}"; shift
case "${cmd}" in
  build)    cmd_build;;
  clean)    cmd_clean;;
  ""|help)  cmd_help;;
  upload)   cmd_upload;;
  *)        echo "helper.sh: '${cmd}' is not a valid command. Please use 'helper.sh help'." 1>&2;;
esac
