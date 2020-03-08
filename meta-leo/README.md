# meta-smartroad
yocto layer for the china smart road app components

## How to build the image and repo for China SmartRoad on the ccimx6sbc

These instructions are targeted towards new team members setting up the project
for the first time. A short summary for more experienced yocto developers is given at the bottom.

### Install Digi Yocto and create a build project

The current China SmartRoad is developed using Release 2.4 of Digi Yocto Linux,
which was the most current version at the time the project was started. Newer releases
are now available. However, unless you know what you are doing, it is recommended that
you use Release 2.4.

#### Install Digi Yocto

Follow the Digi Yocto documentation to
[set up a workstation](https://www.digi.com/resources/documentation/digidocs/embedded/dey/2.4/cc6/yocto_t_set-up-workstation)
and
[Install Digi Embedded Yocto](https://www.digi.com/resources/documentation/digidocs/embedded/dey/2.4/cc6/yocto_t_install-dey).
You can skip the steps on setting up TFTP and NFS servers for now.

Hint: The folder `/usr/local/dey-2.4` given as an example in the Digi
documentation needs local admin (sudo) permissions on your developement
system. If you don't have local admin rights, you can safely choose a folder in your home directory
instead. In this case, you need to replace the correct folder in the example
commands given in the documentation.

#### Create a stock Digi Yocto project folder

Create a project folder by following the first part
*Create Projects* in the Digi Yocto Documentation on
[Creating and building projects](https://www.digi.com/resources/documentation/digidocs/embedded/dey/2.4/cc6/yocto_t_create-build-projects).

#### Build a plain Digi yocto core image

Before you start adding project specific layers to the image, it is a good
idea to first build a stock Digi yocto image to make sure your build system is sane.
The `digi-yocto-qt` image proposed by the Digi Yocto Documentation on
[building images](https://www.digi.com/resources/documentation/digidocs/embedded/dey/2.4/cc6/yocto_t_create-build-projects) includes
many unnecessary features and takes very long to download and build.
It is sufficient to only build a minimal image at this time, by replacing the command `bitbake dey-image-qt`
shown in the documentation by `bitbake dey-image-tiny`

#### Flash the image to the board

At this stage, you may optionally choose to flash the image which you just built
to your Digi board and connect to it. For the beginning, it is suggested you use
the [flashing from microSD card process](https://www.digi.com/resources/documentation/digidocs/embedded/dey/2.4/cc6/yocto_t_program-firmware-from-uboot#update-firmware-from-microsd)


### Customize the build project

Once you successfully built the stock Digi Yocto (tiny) image, it is time
to customioze your project folder to contain Continental SmartRoad specific
software and configuration

#### checkout additional yocto layers from Conti github

The Digi Yocto installation you made above contains a dozen yocto layers
containing recipes (automated build instructions) for stock linux and for
Digi board specific software.

For the China Smartroad Project, an additional 3 yocto layers need to be downloaded
to your development system and included in the build folder. these layers contain

1. [Recipes for the Continental eCAL messaging middleware](http://github.conti.de/SmartCity/meta-ecal)
2. [Recipes for tools and applications reused from the older US Smart Cities Project](http://github.conti.de/SmartCity/meta-smart-city)
3. [China Smartroad project specific recipes](http://github.conti.de/Corp-SnT-SmartRoad/meta-smartroad), including image recipes for building the project specific image

You should checkout the git repositories that contain the 3 layers in any convenient
folder *outside* your yocto project folder created above.

For example, if you want to check the three repositories out in the SmartRoadLayers folder
of your Home Directory, you might execute the following commands from the shell:

```
mkdir ~/SmartRoadLayers
cd ~/SmartRoadLayers
git clone http://github.conti.de/SmartCity/meta-ecal
git clone http://github.conti.de/SmartCity/meta-smart-city
git clone http://github.conti.de/Corp-SnT-SmartRoad/meta-smartroad

```

The examples below assume that you checked out the three layers into this location.
You can chose any other location you prefer, however, you will need to
adapt the commands and configuration examples below.

#### Add the new layers to the bitbake configuration in your project

Now you should navigate back to the project directory you created earlier, for example:
```
cd ~/workspace/ccimx6sbc
```

The project directory will contain a `conf` subdirectory with two important
configuration files, called `bblayers.conf` and `local.conf`.

Using your prefered editor, open the file conf/bblayers.conf. You will find a list
of paths to the stock Digi Yocto layers assigned to a variable BBLAYERS.
Near the bottom (above the final line containing just the closing quotation marks)
add three new lines, which point to the three layers you checked out in the last
section. Pay attention to the syntax (don't forget the backslash at the end of
each line you add. There must not be any trailing spaces after the backslash).

The last 6 lines of the `bblayers.conf` file should now look similar to this example:

```
  /usr/local/dey-2.4/sources/meta-digi/meta-digi-arm \
  /usr/local/dey-2.4/sources/meta-digi/meta-digi-dey \
  /home/uidxxxxx/SmartRoadLayers/meta-ecal \
  /home/uidxxxxx/SmartRoadLayers/meta-smart-city \
  /home/uidxxxxx/SmartRoadLayers/meta-smartroad \
  "
```

#### Customize image configuration

SmartRoad China uses debian package management (APT) to manage
application software updates without having to flash a complete new image.
The default package management system of digi yocto needs to be switched from
rpm to deb.

Open the file conf/local.conf in you project folder an search for the line that
starts with `PACKAGE_CLASSES ?= `.
Make sure that this line reads
`PACKAGE_CLASSES ?= "package_deb"`.
If your project is setup freshly, most probably you will need to correct it, since
It will read "package_rpm" instead.

#### Example configuration files

You can find examples of the configuration files in the doc/conf folder of this repository

#### Build project specific image

Once you have added the project specific layers and edited the local configuration file,
you can build a new image for the china smartroad project:

`bitbake dey-image-smartroad`

#### Device specific configuration

Some network configuration (default IP Address etc.) might be device specific.

The network configuration can be found in the meta-smartroad layer which
you checked out earlier. Navigate to the root folder of the layer
`cd /home/uidxxxxx/SmartRoadLayers/meta-smartroad`.

In there, you will find all the project specific recipes. The recipes for network
interface configuration can be found in:
`cd recipes-core/init-ifupdown/init-ifupdown-1.0/`.

You can edit the ip addresses and route setup in the file `interfaces.eth0` both in
the recipes folder and in the machine specific (for example ccimx6sbc or ccimx6ul)
subfolders. Please pay attention that if there is a interface.eth0 file found in
a machine specific subfolder which matches your actual hardware, this will override
the hardware-independend file in the recipe folder itself.

Similarily, ecal configuration (ini) files can be found in the same layer in the
`recipes-ecal/ecal/files` subfolder.

### Include Local Sources for development

Normally, yocto will always check out sources itself from the upstream that is configured
in the package recipe (mostly git repositories). For local development, it is often more
convenient for yocto to use your local copy of the sources, where you are making
your changes, for a certain package which you are currently developing.

This is possible in yocto by adding a couple of lines at the bottom of conf/local.conf
file in the project folder.

For yocto to support local sources (external to the normal yocto fetch mechanism), it needs
to inherit from the externalsrc class. In addition, for each package with local sources,
yocto needs to know where the local copy is located. As an example, if you are currently
actively developing SCRadarEM, then the last two lines of you conf/local.conf might
look like this:

```

INHERIT += "externalsrc"
EXTERNALSRC_pn-sc-radar-em = "/home/uidxxxx/Project/SCRadarEM"
```

You can add additional `EXTERNALSRC_pn-<packagename>` lines if you are actively developing
more than one package.

Once you are finished working on a packages sources, please commit your changes to upstream
and remove (or comment out) the EXTERNALSRC line in your conf/local.conf. Do not have yocto
build from local sources permanently. This mechanism is only intended to simplify rebuilding
the same code again and again during active development or during debugging.

#### Copy the app binary to target board to test
On build machine, the app binary is at a place like
```
/tmp/work/aarch64-dey-linux/hwmoniter/git-r0/hwmoniter-git/src
```
or
```
/tmp/work/aarch64-dey-linux/cannet2ecal/git-r0/build/src
```
You can use `scp` to copy it to the target digi board to test, like
```
scp ./Cannet2Ecal root@192.168.1.30:/usr/bin/Cannet2Ecal
```

## Software Upgrade

### General
Watch dog is enabled during u-boot phase, run `wdt stop` to stop it.
```
wdt stop
```

`/data` partition is required, run `mkfs.ext4 -L data /dev/mmcblk0p7` for each newboard.
```
mkfs.ext4 -L data /dev/mmcblk0p7
```

### Software update using RAUC

Rauc is a popular framework for safe and secure updating of linux based embedded systems.
Currently, smartroad bundle is currently setup to update rootfs only. Update of the
linux kernel or the bootloader si currently not setup.


#### Prerequisites

Before RAUC can be used to update the board, it needs to be flashed once with all 4 smartroad
specific partitions: bootloader (u-boot), linux kernel, rootfs and recovery.

You can check the current status of the rauc update system by typing "rauc status" on the console. The output
contains info about both "slots". Example output:

```
root@ccimx8x-sbc-pro:~# rauc status
Compatible:  Conti cn SmartRoad RSU
Variant:
Booted from: rootfs.0 (/dev/mmcblk0p3)
Activated:   rootfs.0 (rootfs)
slot states:
  rootfs.0: class=rootfs, device=/dev/mmcblk0p3, type=ext4, bootname=rootfs
      state=booted, description=, parent=(none), mountpoint=/
      boot status=good
  rootfs.1: class=rootfs, device=/dev/mmcblk0p4, type=ext4, bootname=update
      state=inactive, description=, parent=(none), mountpoint=(none)
      boot status=good

root@ccimx8x-sbc-pro:~#
```

If the board is completely new, and no rauc update was ever made yet, the status of slot two, as well as the
booted from and activated information might be unknown/unset. This is not an error, but is
normal. The information will be set on first update.

Also, RAUC bundles are signed with a certificate for security reasons. Validation of this
signature will fail if the current time on the board is not set. It is therefore a
prerequisite to set up a valid time source (for example NTP). For testing purposes, the time
can be set manually using the date -s <YYYY.MM.DD-HH:MM> command from the console.

The current developer certificate/key pair used to sign the bundles is valid for one year.
To create a new certificate/key pair, you can use the following command:

```
openssl req -x509 -newkey rsa:4096 -nodes -keyout smartroad_dev.key.pem -out smartroad_dev.cert.pem -days 365 -subj "/O=Continental Inc./CN=smartroad-dev"
```

Copy *both* files into your local developer key folder. Copy *only* the .cert.pem file into meta-smartroad/recipe-core/rauc/files folder.
The .key.pem file is the *private* key, that is used to sign the update bundles with the certificate. The .cert.pem file is the *public*
certificate file and is later used on the device to verify if the bundle was signed with the correct certificate.
used on the device




#### Instructions

Build the upgrade bundle using "bitbake dey-bundle-smartroad". After this, the image folder in
tmp/deploy will contain

The update bundle can be copied to any location on the board. It is recommended to copy it either
to the sd card or to the /data partition via ssh/sftp.

Optional, check if the bundle is correctly copied and the signature can be validated (correct time
set on the board etc...) by using "rauc info dey-bundle-smartroad-ccimx8x-sbc-pro.raucb".

Start the update by using "rauc install dey-bundle-smartroad-ccimx8x-sbc-pro.raucb". Update will take about 1 minute
at the current image size, and progress status will be printed

If the update was successful, the other "slot" should now be activated (see "rauc status"):

```
Booted from: rootfs.0 (/dev/mmcblk0p3)
Activated:   rootfs.1 (rootfs)
```

You can then "reboot" the board, and it will boot into the new slot. A final "rauc status" will show:

```
Booted from: rootfs.1 (/dev/mmcblk0p4)
Activated:   rootfs.1 (update)
```


## Summary for experienced Developers

After creating a stock Digi Yocto project folder, add the following three layers to the bblayers.conf:

git clone http://github.conti.de/SmartCity/meta-ecal
git clone http://github.conti.de/SmartCity/meta-smart-city
git clone http://github.conti.de/Corp-SnT-SmartRoad/meta-smartroad

Also, set `PACKAGE_CLASSES` to `package_deb` in your local configuration

The image to build is called dey-image-smartroad, and its recipe is in meta-smartroad
layer.

IP address and networking configuration can be found in


python -m SimpleHTTPServer 5678
