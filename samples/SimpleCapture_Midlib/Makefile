#/**************************************************************************************
#;* Copyright 2008 Micron Technology, Inc. All rights reserved.
#;*
#;*
#;* No permission to use, copy, modify, or distribute this software and/or
#;* its documentation for any purpose has been granted by Micron Technology, Inc.
#;* If any such permission has been granted ( by separate agreement ), it
#;* is required that the above copyright notice appear in all copies and
#;* that both that copyright notice and this permission notice appear in
#;* supporting documentation, and that the name of Micron Technology, Inc. or any
#;* of its trademarks may not be used in advertising or publicity pertaining
#;* to distribution of the software without specific, written prior permission.
#;*
#;*
#;*  This software and any associated documentation are provided AS IS and 
#;*  without warranty of any kind.   MICRON TECHNOLOGY, INC. EXPRESSLY DISCLAIMS 
#;*  ALL WARRANTIES EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO, NONINFRINGEMENT 
#;*  OF THIRD PARTY RIGHTS, AND ANY IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS 
#;*  FOR A PARTICULAR PURPOSE.  MICRON DOES NOT WARRANT THAT THE FUNCTIONS CONTAINED 
#;*  IN THIS SOFTWARE WILL MEET YOUR REQUIREMENTS, OR THAT THE OPERATION OF THIS SOFTWARE 
#;*  WILL BE UNINTERRUPTED OR ERROR-FREE.  FURTHERMORE, MICRON DOES NOT WARRANT OR 
#;*  MAKE ANY REPRESENTATIONS REGARDING THE USE OR THE RESULTS OF THE USE OF ANY 
#;*  ACCOMPANYING DOCUMENTATION IN TERMS OF ITS CORRECTNESS, ACCURACY, RELIABILITY, 
#;*  OR OTHERWISE.  
#;**************************************************************************************/


#
#  To compile:
#		$ make
#
#
#  To remove all compiled objects and libraries
#		$ make clean
#
#
#   This Makefile will produce a executable code SimpleCapture.
#


CC=g++
EMBED_LINUX_FLAGS =
CCOMPILER= $(CC)  -Wno-unused -g -D_REENTRANT -DMI_STATICLIB  -I .  -I../libraries/midlib2/



# system libraries.
SYSLIBRARIES=-pthread


# all libs together

LIBS = $(SYSLIBRARIES) -Wl,-Bdynamic,-ldl


DYNAMICLIB=midlib2.so
STATICLIB=libmidlib2.a

EXECUTIBLE=SimpleCapture

# want maybe to debug
SYSLDFLAGS=-g
DYNAMICLINK=$(CC) -shared
STATICLINK=ar -r



default: $(EXECUTIBLE)

# builds the executable itself
# don't forget to export LD_LIBRARY_PATH=/artefaktur/acdk/bin before execute the binary


$(EXECUTIBLE): SimpleCapture.o
	$(CC) -o SimpleCapture SimpleCapture.o -lmidlib2 -lparsifal -lusb-1.0 -L../libraries/midlib2 -L../libraries/parsifal -pthread -ldl -lm


%.o: %.c
	$(CCOMPILER) -c $< -o $@

%.o: %.cpp
	$(CCOMPILER) -c $< -o $@

clean:
	rm $(EXECUTIBLE) *.o


# type: sudo ./SimpleCapture
run:
	
