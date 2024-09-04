# auto-brightness
 Simple service that adjusts display brightness using data from an iio ambient light sensor and brightnessctl to control the display.
 
 The script uses simple arithmetic and does not "learn" over time, however you can (should) adjust the variables to fit your needs.
 
 Currently serves my needs on an almalinux system running gnome-kiosk (which doesn't have the auto-brightness that standard GNOME has).
 
## Usage
- You need brightnessctl installed and working for this service to work
- Run the script as ```sudo``` and uncomment the line with ```echo``` to experiment with the variables

Here is a convenience script (untested, [please check before running](./install.sh)) that should install and enable the systemd service on your computer:
```
sudo bash -c "$(curl -fsSL https://raw.githubusercontent.com/Autotren/auto-brightness/main/install.sh)"
```
## Tips
- I had to update the kernel on my almalinux machine using [ELRepo](https://wiki.almalinux.org/repos/Extras.html#the-elrepo-project) for my ambient sensor to work
