This is a Aptina Qt sample program that opens the camera, initializes it and display images.
Note: User needs super-user to access the USB devices. (sudo ./SimpleQt)

Instruction for SimpleQt in Linux:

- Build/Test environments
  Ubuntu versions 11.10, 12.04 (32-bit GNU C compiler)
  Qt 4.4.7, Qt 4.5
  Qt Creator 2.2.1

- Required run-time libraries (from public domain):
	1. libusb-1.0
	2. libpython3.2
	3. libtbb
	
- Required shared object files (from Aptina):
	1. libapbase.so
	2. libmidlib2.so
	
- To build SimpleQt in Linux: Use Qt Creator and open the SimpleQt.pro file then build all.
  The two .so files required are:
        /DevSuite/libraries/midlib2/gccRelease/libmidlib2.so and
        /DevSuite/libraries/apbase/gccRelease/libapbase.so
  Note: export LD_LIBRARY_PATH=SimpleQt_directory before execute the binary

- run.sh is a sample bash script to run the SimpleQt. (sudo ./run.sh)
  Note: User may need to do (chmod 777 run.sh) first.

- sdat/xsdat file - are located in /DevSuite/sensor_data directories.
  The sensor_data file contains the target sdat (or xsdat) files.

- /DevSuite/apps_data - contains INI files for the different supported sensors.
