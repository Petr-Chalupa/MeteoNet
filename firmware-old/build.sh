#!/bin/bash

# ==========================
# ESP-IDF build / flash / monitor script
# ==========================

ESP_IDF_PATH=~/esp/esp-idf
PORT="/dev/ttyACM0"
FLASH=false
MONITOR=false
EXTRA_ARGS=""

# --- Options ---
while getopts "fmp:a:" opt; do
    case "$opt" in
        f) FLASH=true ;;
        m) MONITOR=true ;;
        p) PORT="$OPTARG" ;;
        a) EXTRA_ARGS="$OPTARG" ;;
        *)
            echo "Usage: $0 [-f flash] [-m monitor] [-p port] [-a 'CMAKE_ARGS']"
            exit 1
            ;;
    esac
done

# --- Activation ---
if [ ! -f "$ESP_IDF_PATH/export.sh" ]; then
    echo "ERROR: ESP-IDF not found at $ESP_IDF_PATH"
    exit 1
fi

. "$ESP_IDF_PATH/export.sh"

# --- Build ---
idf.py $EXTRA_ARGS build
if [ $? -ne 0 ]; then
    echo "ERROR: Build failed"
    exit 1
fi

# --- Flash ---
if [ "$FLASH" = true ]; then
    idf.py $EXTRA_ARGS -p "$PORT" flash
    if [ $? -ne 0 ]; then
        echo "ERROR: Flash failed"
        exit 1
    fi
fi

# --- Monitor ---
if [ "$MONITOR" = true ]; then
    idf.py -p "$PORT" monitor
fi
