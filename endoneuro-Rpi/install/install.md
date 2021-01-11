
Lancer Raspberry Pi Imager v1.4
Choisir Raspberry Pi OS (32-bit)

## Configurer la Raspberry pour SSH
- connecter la Raspberry à un écran, une souris et un clavier
- Insérez la carte SD
- Alimentez la Raspberry

A la fin duy boot on a un welcome dialog box
 - click on Next
In Set Country dialog box
 - Choose Belgium, French, Brussels
 - check use english langage (your keyboard will be in Qwerty)
 - Click on Next
Change password
 - New password: kiwi

Set up screen
we don't care, we will go headless soon

Select Wireless Network (1)
 - select your Wireless network. We will need an internet connection to install the needed softwares

Update software
 - Click Next
 - Click OK when the system is up-to-date
 - Click Restart in Setup Complete dialog box

 Ater restart 
  - Click on the Raspberry menu (in the top left of the screen)
  - Select Préférences/Raspberry Pi Configuration
# In 'System' tab
# - boot: To CLI

In Interfaces tab
 - Enable Camera
 - Enable SSH
 - Enable SPI

In Localisation tab
 - set keyboard/layout : Belgian

Click Ok and and Yes to restart (ignore the warning for the password)

In the file manager (third icon in the top toolbar)
 - navigate to /etc
 - double-click on dhcpcd.conf

 Around the end, find the block begining by 
   # exapmple static IP configuration 

Below it, add these 3 lines:
interface eth0
static ip_address=10.0.0.1
static routers=10.0.0.254

Save the file

In the Raspberry menu, select logout/ shutdown

Remove the Power
Disconnect screen, mouse and keyboard
Connect the Ethernet cable, the camera and the Endoneuro HAT

Power the RPi

Launch MobaXterm (or another SSH client)
Create a new PuTTy session

Double-click on it
Say yes for the MoTTY security Warning

log in (pi, kiwi)

upload config.sh
chmod +x config.sh
upload bcm2835-1.68.tar.gz
upload openCV.zip
upload openCV_contrib.zip

sh config.sh all


## NOTES

(1): At this point of the project, we suppose that the ethernet connection between the RPi and the host is point-to-point. If that changes, this configuratuion step may be affected.