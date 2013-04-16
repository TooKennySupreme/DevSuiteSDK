This is a Aptina Qt sample program that opens the camera, initializes it and display images.
Note: User needs super-user to access the USB devices. (sudo ./SimpleQt)

Instruction for SimpleQt in Linux:

- Build/Test environments
  Mac OS X Lion 10.7.5 and 64-bit GNU C compiler version 4.8.0. XCode 4.5.2 and MacPorts 2.1.2 installed.
  Qt 4.7.4, Qt 4.8.4
  Qt Creator 2.6.1

- Required run-time libraries (from public domain):
	1. libusb-1.0
	2. libpython3.2
	3. libtbb
	
- Required shared object files (from Aptina):
	1. libapbase.so
	2. libmidlib2.so
	
- To build SimpleQt onMacOS: Use Qt Creator and open the SimpleQt.pro file then "Build All".
  The two .so files required are:
        /DevSuite/libraries/midlib2/gccRelease/libmidlib2.so and
        /DevSuite/libraries/apbase/gccRelease/libapbase.so
  Note: export LD_LIBRARY_PATH=SimpleQt_directory before execute the binary

- sdat/xsdat file - are located in /DevSuite/sensor_data directories.
  The sensor_data file contains the target sdat (or xsdat) files.

- /DevSuite/apps_data - contains INI files for the different supported sensors.

- /Devsuite/board_data - contains Aptina Demo board data.