#!/bin/bash

# Define delimiter as comma for "brightnessctl -m"
IFS=","

# Increase the integration time to reduce sensor noise
# In my case the available values are on /sys/bus/iio/devices/iio:device0/in_illuminance_integration_time_available
echo 0.800000 | tee "/sys/bus/iio/devices/iio:device0/in_illuminance_integration_time" # Remove this if not supported on your sensor

illuminance_sensor_path="/sys/bus/iio/devices/iio:device0/in_illuminance_input" # Path of your ambient sensor
illuminance_min=0 # Below this value the brightness will be 0
illuminance_max=30 # Above this value the brightness will be maxed

step_const=10 # The value to divide the brightness difference by
min_step=100 # Minimum absolute step value
max_step=1000 # Maximum absolute step value

sleep_short=0.01 # Interval in seconds while changing brightness
sleep_long=1 # Interval in seconds while idle

while true; do
    illuminance=$(cat $illuminance_sensor_path)
    set -- $(brightnessctl -m)
    current=$3
    max=$5

    target=$(((illuminance - illuminance_min) * max / (illuminance_max - illuminance_min)))
    if (( target > max )); then
        target=$max
    elif (( target < 0 )); then
        target=0
    fi

    diff=$((target - current))
    step=$(((target - current) / step_const))
    if (( step > 0 )); then
        if (( step < min_step )); then
            step=$min_step
            if (( diff < min_step )); then
                step=$diff
            fi
        elif (( step > max_step )); then
            step=$max_step
        fi
    elif (( step < 0 )); then
        if (( step > -min_step )); then
            step=$((-min_step))
            if (( diff > -min_step )); then
                step=$diff
            fi
        elif (( step < -max_step )); then
            step=$((-max_step))
        fi
    fi
    
    brightnessctl -q set $((current + step))

    # echo Sensor: $(cat $illuminance_sensor_path), display: $(brightnessctl -m), target: $target, step: $step

    if (( step == 0 )); then
        sleep $sleep_long
    else
        sleep $sleep_short
    fi
done
