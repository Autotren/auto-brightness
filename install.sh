#!/bin/bash

# Variables
REPO_URL="https://raw.githubusercontent.com/Autotren/auto-brightness/main"
EXEC_NAME="auto-brightness"
SERVICE_NAME="auto-brightness.service"
RELEASE_URL="https://github.com/Autotren/auto-brightness/releases/download/dev"
ARCHIVE_NAME="auto-brightness.zip"

# Directories
INSTALL_DIR="/usr/local/bin"
SERVICE_DIR="/etc/systemd/system"

echo "Downloading the executable..."
wget  ${RELEASE_URL}/${ARCHIVE_NAME}
unzip -o ${ARCHIVE_NAME} -d ${INSTALL_DIR}
rm ${ARCHIVE_NAME}
chmod +x ${INSTALL_DIR}/${EXEC_NAME}
echo "Executable installed to ${INSTALL_DIR}/${EXEC_NAME}"

echo "Downloading the systemd service file..."
curl -o ${SERVICE_DIR}/${SERVICE_NAME} ${REPO_URL}/${SERVICE_NAME}
echo "Service file downloaded to ${SERVICE_DIR}/${SERVICE_NAME}"

echo "Download complete!"
echo "Please run manually ${INSTALL_DIR}/${EXEC_NAME} in order to adjust and test the command-line arguments for your setup."
echo "When you have finished, add your command-line arguments to the ExecStart directive in ${SERVICE_DIR}/${SERVICE_NAME}"
echo "Finally, run the following to enable the service: systemctl daemon-reload && systemctl enable --now ${SERVICE_NAME}"
echo "You can check the status of the service by running systemctl status ${SERVICE_NAME}"