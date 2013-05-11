This is an Aptina Qt sample program that opens the camera, initializes it and display images.
Note: User needs Administrator password to access the USB devices. (sudo ./SimpleQt)

Instruction for SimpleQt in Linux:

- Build/Test environments
  Ubuntu versions 11.10, 12.04 (32/64-bit GNU C compiler)
  Qt 4.4.7, Qt 4.5
  Qt Creator 2.2.1

- Required run-time libraries (from public domain):
	1. libusb-1.0
	2. libtbb
	3. libpython3.3
	   Note: 1. User needs to download and build the python source code for the Ubuntu earlier than 13.04
	         2. User needs to activate the shared object option and set prefix to /usr (or your path settings) in the python configuration.
	            (e.g. ./configure --enable-shared --prefix=/usr)
	
- Required shared object files (from Aptina):
	1. libapbase.so
	2. libmidlib2.so
	
- To build SimpleQt in Linux: Use Qt Creator and open the SimpleQt.pro file then "Build All".
  The two .so files required for 64-bit Linux are:
        /DevSuite/libraries/midlib2/gccRelease/libmidlib2.so and
        /DevSuite/libraries/apbase/gccRelease/libapbase.so
		(In Github: /DevSuiteSDK/lib/linux/x64/libmidlib2.so and
		    	    /DevSuiteSDK/lib/linux/x64/libapbase.so)
  Note: export LD_LIBRARY_PATH=SimpleQt_directory before execute the binary

- run.sh is a sample bash script to run the SimpleQt. (sudo ./run.sh)
  Note: User may need to do (chmod 777 run.sh) first.

- sdat/xsdat sensor data files - are located in "sensor_data" directory.

- INI setting files for the different supported sensors - are located in "apps_data" directory.

- cdat files for Aptina Demo board kits - are located in "board_data' directory.

Note:
In Linux and MacOS, the default locations for above directories are in "../../data/".
In Windows, the default locations for above directories are in MI_HOME path.
