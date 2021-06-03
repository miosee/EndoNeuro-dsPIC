# Configuration de la Raspberry

## Configurer l'adresse IP

https://howchoo.com/pi/configure-static-ip-address-raspberry-pi
sudo nano /etc/dhcpcd.conf
interface eth0
static ip_address=10.0.0.1
static routers=10.0.0.254

## Installer les mises à jour

sudo apt-get update && sudo apt-get upgrade

## Installer cmake

<https://nickcullen.net/blog/raspberry-pi-tutorials/raspberry-pi-c-using-cmake/>

sudo apt install cmake

## Installer openCV

<https://www.pyimagesearch.com/2018/09/26/install-opencv-4-on-your-raspberry-pi/>

sudo apt-get install libjpeg-dev libpng-dev libtiff-dev
sudo apt-get install libavcodec-dev libavformat-dev libswscale-dev libv4l-dev
sudo apt-get install libxvidcore-dev libx264-dev
sudo apt-get install libgtk-3-dev
sudo apt-get install libcanberra-gtk*
sudo apt-get install libatlas-base-dev gfortran

Installation pour Python => optionnel ?
sudo apt-get install python3-dev

wget -O opencv.zip https://github.com/opencv/opencv/archive/4.0.0.zip
wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/4.0.0.zip
unzip opencv.zip
unzip opencv_contrib.zip
mv opencv-4.0.0 opencv
mv opencv_contrib-4.0.0 opencv_contrib

cd ~/opencv
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE \
    -D CMAKE_INSTALL_PREFIX=/usr/local \
    -D OPENCV_EXTRA_MODULES_PATH=~/opencv_contrib/modules \
    -D ENABLE_NEON=ON \
    -D ENABLE_VFPV3=ON \
    -D BUILD_TESTS=OFF \
    -D OPENCV_ENABLE_NONFREE=ON \
    -D INSTALL_PYTHON_EXAMPLES=OFF \
    -D BUILD_EXAMPLES=OFF ..

### Augmenter la taille du swap

sudo nano /etc/dphys-swapfile

- modifiez la taille du swap en commentant et remplaçant la définition de CONF_SWAPSIZE
    # CONF_SWAPSIZE=100
    CONF_SWAPSIZE=2048

- Sauvez le fichier
- Redémarrez le service de swap :
    $ sudo /etc/init.d/dphys-swapfile stop
    $ sudo /etc/init.d/dphys-swapfile start

### Compiler et installer la librairie

- Compilez openCV, en utilisant tous les coeurs
    $ make -j$(nproc)

- Installez la librairie
    $ sudo make install
    $ sudo ldconfig

### Nettoyage

- Remettez la taille par défaut du swap (cela évite de tuer trop vite la carte SD).
    $ sudo nano /etc/dphys-swapfile

- Décommentez la définition par défaut de CONF_SWAPSIZE et supprimer celle que vous aviez ajoutée
    CONF_SWAPSIZE=100

- Sauvez le fichier
- Redémarrez le service de swap :
    $ sudo /etc/init.d/dphys-swapfile stop
    $ sudo /etc/init.d/dphys-swapfile 


### test



## Installer la librairie BCM2835

wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.68.tar.gz 
tar xvfz bcm2835-1.68.tar.gz
cd bcm2835-1.68
./configure
make
sudo make install

## Libérer de l'espace mémoire

sudo apt-get purge wolfram-engine
$ sudo apt-get purge libreoffice*
$ sudo apt-get clean
$ sudo apt-get autoremove


## rendre la Raspberry "headless"