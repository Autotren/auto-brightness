#!/bin/bash

# Variables
REPO_URL="https://raw.githubusercontent.com/Autotren/auto-brightness/main"
SCRIPT_NAME="auto-brightness.sh"
SERVICE_NAME="auto-brightness.service"

# Directories
INSTALL_DIR="/usr/local/bin"
SERVICE_DIR="/etc/systemd/system"

echo "Downloading the script..."
curl -o ${INSTALL_DIR}/${SCRIPT_NAME} ${REPO_URL}/${SCRIPT_NAME}
chmod +x ${INSTALL_DIR}/${SCRIPT_NAME}
echo "Script downloaded to ${INSTALL_DIR}/${SCRIPT_NAME}"

echo "Downloading the systemd service file..."
curl -o ${SERVICE_DIR}/${SERVICE_NAME} ${REPO_URL}/${SERVICE_NAME}
echo "Service file downloaded to ${SERVICE_DIR}/${SERVICE_NAME}"

echo "Reloading systemd daemon..."
systemctl daemon-reload

echo "Enabling the service..."
systemctl enable ${SERVICE_NAME}

# echo "Starting the service..."
# systemctl start ${SERVICE_NAME}

echo "Installation complete!"
echo "Please check the script in ${INSTALL_DIR}/${SCRIPT_NAME} and adjust the variables for your setup."
echo "When you have finished, reboot your system or run systemctl start ${SERVICE_NAME}"