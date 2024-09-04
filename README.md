# auto-brightness
Simple service that adjust display brightness using date from an iio ambient light sensor and brightnessctl
## Usage
- You need brightnessctl installed and working for this service to work
- Run the script as ```sudo``` and uncomment the line with ```echo``` to experiment with the variables

Here is a convenience script that installs and enables the systemd service to your computer. 
```
sudo bash -c "$(curl -fsSL https://raw.githubusercontent.com/Autotren/auto-brightness/main/install.sh)"
```