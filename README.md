# auto-brightness
 Simple service written in C that adjusts display brightness using data from an iio ambient light sensor. The program accepts any file with numeric value as data source, and should work standalone.
 
 The algorithm uses simple arithmetic and does not "learn" over time, however you can adjust the parameters to fit your needs.
 
 Currently serves my needs on an almalinux system running gnome-kiosk (which doesn't have the auto-brightness that standard GNOME has).
 
## Installing
- Download the executable from releases
- Run the program manually as ```sudo``` to test the command-line arguments for your setup
- When you have finished, add your command-line arguments to the ```ExecStart``` directive in the [sample service file](./auto-brightness.service) found on the repo root
- To enable and start the service, run ```systemctl daemon-reload``` and ```systemctl enable --now auto-brightness.service```
- Check service status by running ```systemctl status auto-brightness.service```

Here is a convenience script (untested, [please check before running](./install.sh)) that downloads the program and systemd service file:
```
sudo bash -c "$(curl -fsSL https://raw.githubusercontent.com/Autotren/auto-brightness/main/install.sh)"
```
## Help page
```
Usage: auto-brightness --help --verbose --illuminance-path "/sys/bus/iio/devices/iio:device0/in_illuminance_input" --set-integration --illuminance-integration-time-path "/sys/bus/iio/devices/iio:device0/in_illuminance_integration_time" --illuminance-integration-time 0.8 --illuminance-min 0 --illuminance-max 30 --poll-interval 800 --step-const 10 --step-min 100 --step-max 1000 --brightness-path "/sys/class/backlight/intel_backlight/brightness" --max-brightness-path "/sys/class/backlight/intel_backlight/max_brightness" --brightness-interval 50

Arguments explanation:
--help: Show this help information and exit
--verbose: Show debug information
--illuminance-path: Path to light sensor file
--set-integration: Enables writing integration time value to the sensor
--illuminance-integration-time-path: Path to sensor integration time value file
--illuminance-integration-time: Value to be set as integration time
--illuminance-min: Below this value the brightness will be 0
--illuminance-max: Above this value the brightness will be maxed
--poll-interval: Illuminance sensor poll interval in ms
--step-const: The value to divide the brightness difference by
--step-min: Minimum absolute step value
--step-max: Maximum absolute step value
--brightness-path: Path to display brightness file
--max-brightness-path: Path to max display brightness file
--brightness-interval: Time between brightness steps
```

## Tips
- I had to update the kernel on my almalinux machine using [ELRepo](https://wiki.almalinux.org/repos/Extras.html#the-elrepo-project) for my ambient light sensor to work
