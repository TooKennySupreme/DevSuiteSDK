This is a Aptina Qt sample program that opens the camera, initializes it and display images.
Note: User needs Administrator account to access the USB devices. (sudo ./SimpleQt)

Instruction for SimpleQt in Mac:

- Build/Test environments
  Mac OS X Lion 10.7.5 and 64-bit GNU C compiler version 4.8.0. XCode 4.5.2 and MacPorts 2.1.2 installed.
  Qt 4.7.4, Qt 4.8.4
  Qt Creator 2.6.1

- Required run-time libraries (from public domain):
	1. libusb-1.0 (Use MacPorts: sudo port install libusb)
	2. libpython3.3m (download the Mac dmg file from http://www.python.org/download/)
	   (Note: Remember create a symbolic link for libpython3.3m.dylib)
	   (e.g. sudo ln –s /Library/Frameworks/Python.framework/Versions/3.3/Python /usr/lib/libpython3.3m.dylib)
	3. libtbb (sudo port install tbb)
	
- Required shared object files (from Aptina):
	1. libapbase.so
	2. libmidlib2.so
	
- To build SimpleQt on MacOS: Use Qt Creator and open the SimpleQt.pro file then "Build All".
  The two .so files required for MacOS are:
        /DevSuite/libraries/midlib2/gccRelease/libmidlib2.so and
        /DevSuite/libraries/apbase/gccRelease/libapbase.so
	(In Github: /DevSuiteSDK/lib/macos/libmidlib2.so and
		        /DevSuiteSDK/lib/macos/libapbase.so)
  Note: export LD_LIBRARY_PATH=SimpleQt_directory before execute the binary

- sdat/xsdat sensor data file - are located in "sensor_data" directory.

- INI setting files for the different supported sensors - are located in "apps_data" directory.

- cdat files for Aptina Demo board kits - are located in "board_data' directory.

Note:
In Linux and MacOS, the default locations for above directories are in "../../data/".
In Windows, the default locations for above directories are in MI_HOME path.
