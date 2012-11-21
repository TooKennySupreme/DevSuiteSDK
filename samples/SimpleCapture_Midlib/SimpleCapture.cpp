//**************************************************************************************       
// Copyright 2009 Aptina Imaging Corporation. All rights reserved.                             
//                                                                                             
//                                                                                             
// No permission to use, copy, modify, or distribute this software and/or                      
// its documentation for any purpose has been granted by Aptina Imaging Corporation.           
// If any such permission has been granted ( by separate agreement ), it                       
// is required that the above copyright notice appear in all copies and                        
// that both that copyright notice and this permission notice appear in                        
// supporting documentation, and that the name of Aptina Imaging Corporation or any            
// of its trademarks may not be used in advertising or publicity pertaining                    
// to distribution of the software without specific, written prior permission.                 
//                                                                                             
//                                                                                             
//      This software and any associated documentation are provided "AS IS" and                
//      without warranty of any kind.   APTINA IMAGING CORPORATION EXPRESSLY DISCLAIMS         
//      ALL WARRANTIES EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO, NONINFRINGEMENT       
//      OF THIRD PARTY RIGHTS, AND ANY IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS        
//      FOR A PARTICULAR PURPOSE.  APTINA DOES NOT WARRANT THAT THE FUNCTIONS CONTAINED        
//      IN THIS SOFTWARE WILL MEET YOUR REQUIREMENTS, OR THAT THE OPERATION OF THIS SOFTWARE   
//      WILL BE UNINTERRUPTED OR ERROR-FREE.  FURTHERMORE, APTINA DOES NOT WARRANT OR          
//      MAKE ANY REPRESENTATIONS REGARDING THE USE OR THE RESULTS OF THE USE OF ANY            
//      ACCOMPANYING DOCUMENTATION IN TERMS OF ITS CORRECTNESS, ACCURACY, RELIABILITY,         
//      OR OTHERWISE.                                                                          
//*************************************************************************************/       

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "midlib2.h"


    // 1) Set the desired width/height (ONLY sets it for the application, does not set it in the sensor
    // 2) Choose the specific sensor routine
#define OUTWIDTH   1280     //3264;  // 640;  //1600; //2592;  //1280;
#define OUTHEIGHT   720      // 2448;  // 480; //1200; //1944;  //720;

/*
 * default amount of frames to capture...can also specify on command line
 */
#define DEFAULT_NUM_FRAMES      1

/*
 * PIXELBITS: number of bits per pixel to capture.
 * For bayer imagers this can be set to:
 * 8    = discard the low order 2 bits, pack into a single unsigned byte per pixel
 * 10   = Unswizzled such that the data is available as a single unsigned short per pixel
 * 16   = Swizzled, 2 bytes per pixel with the following format: 
 *        Byte 1   Byte 0
 *        xxxxxx10 98765432
 */
#define PIXELBITS               10

/*
 * the amount of tries before giving up trying to grab a valid frame. 
 */
#define MAX_BADFRAME_TRIES      10

//;jr;$*
//;jr;$* const char senpath[] = "./sensor_data_sdat/A-1040SOC-REV2-DEV.sdat";
//;jr;$* const char senpath[] = "./sensor_data/.";
// const char senpath[] = "./sensor_data/A-3140SOC-REV1-DEV.xsdat";
const char senpath[] = "./sensor_data/A-1040SOC-REV2-DEV.xsdat";


//**********************************************************************************************
void grab_frame(mi_camera_t *pCamera, int num_frames, 
                unsigned char *pCameraBuff, unsigned char *pGrabframeBuff, 
                mi_u32 nSwizzleNeeded, unsigned long frameSize,
                FILE *imfile)
{
    int frame;

    printf("Grabbing frame \n");
    for (frame = 0; frame < num_frames; frame++)
    {
        int    nRet;
        mi_u8  tempByte;
        int    i;

        /*
         * skip frames untill a good frame is found.
         */
        int count=0;
        do
        {
    //;jr;$*            printf("^^^^^^^ grab_frame in loop: count: %d\n", count);

            nRet = pCamera->grabFrame(pCamera, pGrabframeBuff, pCamera->sensor->bufferSize);
            if (nRet != MI_CAMERA_SUCCESS)
            {
                printf("b");
            }
//        } while (nRet != MI_CAMERA_SUCCESS && count++ < MAX_BADFRAME_TRIES);
        }
        while(0);
        nRet = MI_CAMERA_SUCCESS;

        //The data for 10 bpp needs to be swizzled on the Demo1 & Demo1A boards
        if (pCamera->sensor->imageType == MI_BAYER_10 && nSwizzleNeeded)
        {

            //We go through data 2 Bytes at a time
            for (i = 0; i < (int)frameSize; i += 2)
            {
                //data comes in as         and is changed to
                // Byte 1   Byte 0         Byte 1   Byte0
                //xxxxxx10 98765432       xxxxxx98 76543210 
                tempByte = pGrabframeBuff[i+0];
                pCameraBuff[i+0]  = (pGrabframeBuff[i+0] << 2) | (pGrabframeBuff[i+1]&0x03); 
                pCameraBuff[i+1]  = (tempByte >> 6); 
            }
        }
        else {
            memcpy(pCameraBuff, pGrabframeBuff, frameSize);
        }

        if (nRet != MI_CAMERA_SUCCESS)
            printf("B");
        else
        {
            fwrite(pCameraBuff, frameSize, 1, imfile);
            printf(".");
        }
    }

    return;
}







void str_replace(char* strOut, char* strIn, char* oldStr, char* newStr)
{
    int total_len = strlen(strIn);
    char* pChar;
    int lenBeg;

    pChar= strstr(strIn,oldStr);
    lenBeg = total_len-strlen(pChar);
    strncpy(strOut, strIn,lenBeg);
    strOut[lenBeg] = 0;
    strcat(strOut,newStr);
    strcat(strOut,&strIn[lenBeg+strlen(oldStr)]);
}

int main(int argc, char* argv[])
{
    mi_s32          num_cameras;                        //number of cameras found
    mi_camera_t     *cameras[MI_MAX_CAMERAS];           //cameras found
    mi_camera_t     *pCamera = NULL;                    //current camera
    unsigned long   frameSize;                          //size of the frames we want to save
    unsigned char   *pCameraBuff;                       //memory buffer for all the sensor images
    unsigned char   *pGrabframeBuff;                    //grabFrame buffer
    unsigned int    num_frames = DEFAULT_NUM_FRAMES;    //number of frames to capture
    unsigned long   nWidth  = 0;                        //width of image (taken from command line or set to default below)
    unsigned long   nHeight = 0;                        //height of image (taken from command line or set to default below)
    FILE            *imfile;                            //capture file
    unsigned int    frame;
    mi_u32          nSwizzleNeeded;
    mi_string       errorFileName;
    char            imagetypestr[32];

    //;jr;$* 
    mi_u32          ret, retVal;
    mi_u32          retValarray[5];
    mi_u32          checkret;
    int             i;
    mi_u32          regret;


    printf("usage:\n");
    printf("    SimpleCapture\n");
    printf("    SimpleCapture <num_frames>\n");
    printf("    SimpleCapture <num_frames> <width> <height>\n");

    //parse command line for the number of frames to capture and the width and height (if not default)
    switch (argc-1)
    {
        case 1: 
        {
            num_frames = atoi(argv[1]);
            break;
        }
        case 3: 
        {
            num_frames = atoi(argv[1]);
            nWidth     = atoi(argv[2]);
            nHeight    = atoi(argv[3]);
            break;
        }
    }


    //mi_OpenCameras() is the first midlib routine which must be called
    // ;jr;$* mi_SensorData() fails - MIDLIB .*sdat vs. .xsdat compare problem
    mi_OpenCameras(cameras, &num_cameras, mi_SensorData());
    // mi_OpenCameras(cameras, &num_cameras, senpath);

//;jr;$*
    printf("After mi_OpenCameras, num_cameras: %d  mi_SensorData(): %s\n", num_cameras, mi_SensorData());
	
    //we need at least one camera protocol to use the sensor
    if (num_cameras==0)
    {
        printf("Could not initialize camera\n");
        mi_CloseCameras();
#ifdef _DEBUG
        printf("<press a key>");
        _getch();
        printf("\n");
#endif
        return 1;
    }

    //we will use the first available camera
    pCamera = cameras[0];

    //We must start the camera before we can grab an image
    if( pCamera->startTransport(pCamera) != MI_CAMERA_SUCCESS )
    {
        printf("Start Transport Unsuccessful.\n");
        mi_CloseCameras();
#ifdef _DEBUG
        printf("<press a key>");
        _getch();
        printf("\n");
#endif
        return 1;
    }    

    //Log all SHIP commands
    mi_OpenErrorLog(MI_LOG_SHIP | MI_ALL_ERRORS, "ship_log.txt");
    mi_GetErrorLogFileName(errorFileName);
    printf("Log file: %s \n", errorFileName);

    //  Load default initialization preset [Demo Initialization] in default ini file
    mi_LoadINIPreset(pCamera, NULL, NULL);

    //  If there is a serial receiver board (Dual-MIPI, HSSAB, or HDMI board)
    //  then it may need to be configured.
    //  Uncomment and edit these lines as needed.
    //mi_setMode(m_pCamera, MI_RX_LANES        , 4);  //  MIPI and HiSPI only
    //mi_setMode(m_pCamera, MI_RX_BIT_DEPTH    , 10); //  CCP and HiSPi only; pixel bit depth at the sensor serial interface
    //mi_setMode(m_pCamera, MI_RX_MODE         , MI_RX_HISPI_S);  //  HiSPi only; use one of mi_rx_modes
    //mi_setMode(m_pCamera, MI_RX_CLASS        , 2);  //  CCP only
    //mi_setMode(m_pCamera, MI_RX_SYNC_CODE    , 0xF555);  //  HiSPi only; usually not needed
    //mi_setMode(m_pCamera, MI_RX_EMBEDDED_DATA, (m_pCamera->sensor->height & 3) == 2);  //  CCP/MIPI/HiSPi; set to 1 if SMIA embedded data is enabled on the sensor
    //mi_setMode(m_pCamera, MI_RX_VIRTUAL_CHANNEL, 0);  //  MIPI only
    //mi_setMode(m_pCamera, MI_RX_MSB_FIRST    , 0);  //  HiSPi only
    //mi_setMode(m_pCamera, MI_RX_TYPE         , MI_RX_CCP);  //  for CCP
    //mi_setMode(m_pCamera, MI_RX_TYPE         , MI_RX_MIPI);  //  for MIPI
    //mi_setMode(m_pCamera, MI_RX_TYPE         , MI_RX_HISPI);  //  for HISPI

    //Turn off logging
    pCamera->setMode(pCamera,MI_ERROR_LOG_TYPE,MI_NO_ERROR_LOG);

    if (nWidth < 1 || nHeight < 1)
    {
        //nWidth     = pCamera->sensor->width;
        //nHeight    = pCamera->sensor->height;
        nWidth =  OUTWIDTH;     //3264;  // 640;  //1600; //2592;  //1280;
        nHeight = OUTHEIGHT;    // 2448;  // 480; //1200; //1944;  //720;
    }

    //For 8bit Bayer sensors, update frame size to capture PIXELBITS data (10bit)
    if (mi_IsBayer(pCamera)) {
        pCamera->updateFrameSize(pCamera, nWidth, nHeight, PIXELBITS,0);
    }
    else {
        pCamera->updateFrameSize(pCamera, nWidth, nHeight, 0,0);    
    }
    //display some stats
    mi_GetImageTypeStr(pCamera->sensor->imageType, imagetypestr);
    printf("sensorName = %s \n", pCamera->sensor->sensorName);
    printf("sensorType = %d \n", pCamera->sensor->sensorType);
    printf("width = %d \n", pCamera->sensor->width);
    printf("height = %d \n", pCamera->sensor->height);
    printf("pixelBytes = %d \n", pCamera->sensor->pixelBytes);
    printf("pixelBits = %d \n", pCamera->sensor->pixelBits);
    printf("bufferSize = %d \n", pCamera->sensor->bufferSize);
    printf("imageType = %s \n", imagetypestr);
    printf("shipAddr = %d \n", pCamera->sensor->shipAddr);
    printf("num_regs = %d \n", pCamera->sensor->num_regs);
    
    frameSize = pCamera->sensor->width * pCamera->sensor->height
              * pCamera->sensor->pixelBytes  * 8;  //jr;$* added * 4

    //Allocate a buffer to store the images
//;jr;$*     printf("  frameSize (pCameraBuff):  %ld -----------------------------\n", frameSize);

    pCameraBuff  = (unsigned char *)malloc(frameSize);
    if (pCameraBuff==NULL)
    {
        printf("Error trying to create a buffer of size %ld to capture %d frames.\n", 
               frameSize, num_frames);
        mi_CloseCameras();
#ifdef _DEBUG
        printf("<press a key>");
        _getch();
        printf("\n");
#endif
        return 1;
    }

    //Allocate a buffer to store the images
    pGrabframeBuff  = (unsigned char *)malloc(pCamera->sensor->bufferSize);
    if (pGrabframeBuff==NULL)
    {
        printf("Error trying to create a buffer of size %d to grab the frames.\n", 
            pCamera->sensor->bufferSize);
        mi_CloseCameras();
#ifdef _DEBUG
        printf("<press a key>");
        _getch();
        printf("\n");
#endif
        return 1;
    }

#if 0
    //;jr;$* Check register reads/writes
        pCamera->setMode(pCamera, MI_REG_ADDR_SIZE, 16);
        pCamera->setMode(pCamera, MI_REG_DATA_SIZE, 8);

            i = 0x204;
            ret = pCamera->readRegister(pCamera, pCamera->sensor->shipAddr, i, &regret);
            printf("   Register Read:  addr: 0x%x  ret: 0x%x   Value returned: 0x%x\n", i, ret, regret);
            ret = pCamera->writeRegister(pCamera, pCamera->sensor->shipAddr, i, 0x0000FF);
            printf("   Register Write :  addr: 0x%x  ret: 0x%x\n", i, ret);
            ret = pCamera->readRegister(pCamera, pCamera->sensor->shipAddr, i, &regret);
            printf("   Register Read:  addr: 0x%x  ret: 0x%x   Value returned: 0x%x\n", i, ret, regret);


//        for (i = 0; i < 10; i++) {
//            ret = pCamera->readRegister(pCamera, pCamera->sensor->shipAddr, 0x3000 + i*2, &regret);
//            printf("   Register Read:  addr: 0x%x  ret: 0x%x   Value returned: 0x%x\n", i*2, ret, regret);
//        }

    goto testExit;
#endif

#if 0
    // BAYER Sensors /A5100/A8140
    // Set the Demo2 for Bayer 10 mode (MUST be done otherwise funny results).


    pCamera->setMode(pCamera, MI_REG_ADDR_SIZE, 8);
        pCamera->setMode(pCamera, MI_REG_DATA_SIZE, 16);

    // A5100 set color red for testing
//            ret = pCamera->writeRegister(pCamera, pCamera->sensor->shipAddr, 0xA0, 0x0021);
//            printf("   Register Write :  addr: 0x%x  ret: 0x%x\n", 0xA0, ret);
//            ret = pCamera->writeRegister(pCamera, pCamera->sensor->shipAddr, 0xA1, 0x0000FFF);
//            printf("   Register Write :  addr: 0x%x  ret: 0x%x\n", 0xA1, ret);

            ret = pCamera->readRegister(pCamera, pCamera->sensor->shipAddr, 0xA0, &regret);
            printf("   Register Read:  addr: 0x%x  ret: 0x%x   Value returned: 0x%x\n", 0xA0, ret, regret);
            ret = pCamera->readRegister(pCamera, pCamera->sensor->shipAddr, 0xA1, &regret);
            printf("   Register Read:  addr: 0x%x  ret: 0x%x   Value returned: 0x%x\n", 0xA1, ret, regret);


            ret = pCamera->readRegister(pCamera, 0x64, 0x10, &regret);
            printf("   Register Read:  addr: 0x%x  ret: 0x%x   Value returned: 0x%x\n", 0x10, ret, regret);

            // DEMO2 - set the DEMO2 for 10 bit BAYER output format!!!!!
            ret = pCamera->writeRegister(pCamera, 0x64, 0x10, 0x012C);
            printf("   Register Write :  addr: 0x%x  ret: 0x%x\n", 0xA1, ret);

//            ret = pCamera->readRegister(pCamera, 0x64, 0x10, &regret);
//            printf("   Register Read:  addr: 0x%x  ret: 0x%x   Value returned: 0x%x\n", 0x10, ret, regret);

#endif

            ret = pCamera->readRegister(pCamera, 0x64, 0x10, &regret);
            printf("   Register Read:  addr: 0x%x  ret: 0x%x   Value returned: 0x%x\n", 0x10, ret, regret);




    pCamera->getMode(pCamera, MI_SW_UNSWIZZLE_MODE, &nSwizzleNeeded);

    //For better performance on Demo1 and Demo1A do the swizzle in application
    if (nSwizzleNeeded) {
        pCamera->setMode(pCamera, MI_SW_UNSWIZZLE_MODE, 0);
        printf(" nSwizzleNeeded == 0x%x\n", nSwizzleNeeded);
    }



#if 1


#if 0

    //  AS0260
    checkret = mi_LoadINIPreset(pCamera, "./apps_data/AS0260-REV2.ini", "Toolbar: VGA_60");
//    checkret = mi_LoadINIPreset(pCamera, "./apps_data/AS0260-REV2.ini", "Toolbar: 1080p30");
    printf("   mi_LoadINIPreset returns: 0x%x\n", checkret);
    pCamera->updateFrameSize(pCamera, nWidth, nHeight, 0,0);
    frameSize = nWidth * nHeight * pCamera->sensor->pixelBytes;

    for (i = 0; i < 10; i++) {
        imfile = fopen("img_as0260.raw","wb");
         grab_frame(pCamera, num_frames, pCameraBuff, pGrabframeBuff,
               nSwizzleNeeded, frameSize, imfile);
        printf("[done - %d]\n", i);
        fclose(imfile); 
    }

    printf("Saving frames to img_as0260.raw... \n");

#endif


#if 0
    //  MT9P111-REV2/MISOC5140/MT9V111 (no INI settings needed)
    checkret = mi_LoadINIPreset(pCamera, "./apps_data/MT9P111-REV2.ini", "Viewfinder OFF");
    printf("   mi_LoadINIPreset returns: 0x%x\n", checkret);
    pCamera->updateFrameSize(pCamera, nWidth, nHeight, 0,0);
    frameSize = nWidth * nHeight * pCamera->sensor->pixelBytes;

    for (i = 0; i < 2; i++) {
        imfile = fopen("img_mt9p111.raw","wb");
        grab_frame(pCamera, num_frames, pCameraBuff, pGrabframeBuff,
               nSwizzleNeeded, frameSize, imfile);
        printf("[done - %d]\n", i);
        fclose(imfile); 
    }


#endif



#if 0
    //  MT9E013/MI-8140
    checkret = mi_LoadINIPreset(pCamera, "./apps_data/MT9E013-REV3.ini", "Parallel 640x480 60FPS 16.7ms RAW10 Ext=25MHz Vt_pix_clk=190MHz Op_pix_clk=95MHz FOV=3264x2448");
//    checkret = mi_LoadINIPreset(pCamera, "./apps_data/MT9E013-REV3.ini", "Parallel 3264x2448 11.1FPS 89.9ms RAW10 Ext=25MHz Vt_pix_clk=190MHz Op_pix_clk=95MHz FOV=3264x2448");
    printf("   mi_LoadINIPreset returns: 0x%x\n", checkret);
    pCamera->updateFrameSize(pCamera, nWidth, nHeight, 0,0);
    frameSize = nWidth * nHeight * pCamera->sensor->pixelBytes;

    for (i = 0; i < 4; i++) {
        imfile = fopen("img_mt9e013.raw","wb");
        grab_frame(pCamera, num_frames, pCameraBuff, pGrabframeBuff,
               nSwizzleNeeded, frameSize, imfile);
        printf("[done - %d]\n", i);
        fclose(imfile); 
    }

#endif

 
#if 0
    //  MT9E013/MI-8140
    checkret = mi_LoadINIPreset(pCamera, "./apps_data/MT9E013-REV3.ini", "Parallel 3264x2448 11.1FPS 89.9ms RAW10 Ext=25MHz Vt_pix_clk=190MHz Op_pix_clk=95MHz FOV=3264x2448");
    printf("   mi_LoadINIPreset returns: 0x%x\n", checkret);
    pCamera->updateFrameSize(pCamera, nWidth, nHeight, 0,0);
    frameSize = nWidth * nHeight * pCamera->sensor->pixelBytes;

    for (i = 0; i < 3; i++) {
        imfile = fopen("img_mt9e013.raw","wb");
        grab_frame(pCamera, num_frames, pCameraBuff, pGrabframeBuff,
               nSwizzleNeeded, frameSize, imfile);
        printf("[done - %d]\n", i);
        fclose(imfile); 
    }


#endif


#if 0
    //  MT9P001/MI-5100
    checkret = mi_LoadINIPreset(pCamera, "./apps_data/MT9P001-REV8.ini", "2592x1944");
    printf("   mi_LoadINIPreset returns: 0x%x\n", checkret);
    pCamera->updateFrameSize(pCamera, nWidth, nHeight, 0,0);
    frameSize = nWidth * nHeight * pCamera->sensor->pixelBytes;

    for (i = 0; i < 15; i++) {
        imfile = fopen("img_mt9p001.raw","wb");
        grab_frame(pCamera, num_frames, pCameraBuff, pGrabframeBuff,
               nSwizzleNeeded, frameSize, imfile);
        printf("[done - %d]\n", i);
        fclose(imfile); 
    }

#endif

#if 0
    // SOC1040 640x480
    // SOC360 640x480 (comment out the checkret=... line and get a VGA image
    checkret = mi_LoadINIPreset(pCamera, "./apps_data/MT9M114-REV2.ini", "size_640x480");
    printf("   mi_LoadINIPreset returns: 0x%x\n", checkret);
    pCamera->updateFrameSize(pCamera, nWidth, nHeight, 0,0);
    frameSize = nWidth * nHeight * pCamera->sensor->pixelBytes;

    for (i = 0; i < 4; i++) {

    // TRY 640x480
        imfile = fopen("img0.raw","wb");
        grab_frame(pCamera, num_frames, pCameraBuff, pGrabframeBuff,
               nSwizzleNeeded, frameSize, imfile);
        printf("[done - 0]\n");
        fclose(imfile); 
    }

#endif

#if 1
    // SOC1040  1280x720
    checkret = mi_LoadINIPreset(pCamera, "./apps_data/MT9M114-REV2.ini", "size_1280x720");
    printf("   mi_LoadINIPreset returns: 0x%x\n", checkret);

    pCamera->updateFrameSize(pCamera, nWidth, nHeight, 0,0);
    frameSize = nWidth * nHeight * pCamera->sensor->pixelBytes;

    for (i = 0; i < 5; i++) {
    // TRY 1280x720

        imfile = fopen("img1.raw","wb");
        grab_frame(pCamera, num_frames, pCameraBuff, pGrabframeBuff,
               nSwizzleNeeded, frameSize, imfile);
        printf("[done - 1]\n");
        fclose(imfile); 
    }
#endif

#if 0
    // SOC1040  320x240
    checkret = mi_LoadINIPreset(pCamera, "./apps_data/MT9M114-REV2.ini", "size_320x240");
    printf("   mi_LoadINIPreset returns: 0x%x\n", checkret);
    pCamera->updateFrameSize(pCamera, nWidth, nHeight, 0,0);
    frameSize = nWidth * nHeight * pCamera->sensor->pixelBytes;

    for (i = 0; i < 3; i++) {
        // TRY 320x240

        imfile = fopen("img2.raw","wb");
        grab_frame(pCamera, num_frames, pCameraBuff, pGrabframeBuff,
               nSwizzleNeeded, frameSize, imfile);
        printf("[done - 2]\n");
        fclose(imfile); 
    }
#endif


#if 0
    // SOC2010 Rev1 J
    checkret = mi_LoadINIPreset(pCamera, "./apps_data/MT9D111-REV1.ini", "[Preview - 800x600");
    printf("   mi_LoadINIPreset returns: 0x%x\n", checkret);

    pCamera->updateFrameSize(pCamera, nWidth, nHeight, 0,0);
    frameSize = nWidth * nHeight * pCamera->sensor->pixelBytes;

    for (i = 0; i < 5; i++) {

        imfile = fopen("img3.raw","wb");
        grab_frame(pCamera, num_frames, pCameraBuff, pGrabframeBuff,
               nSwizzleNeeded, frameSize, imfile);
        printf("[done - 1]\n");
        fclose(imfile);

    }

#endif

#else

    imfile = fopen("img.raw","wb");

    //grabFrame 
    printf("Grabbing frame \n");
    for (frame = 0; frame < num_frames; frame++)
    {
        int    nRet;
        mi_u8  tempByte;
        int    i;

        /*
         * skip frames untill a good frame is found.
         */
        int count=0;
        do
        {
            nRet = pCamera->grabFrame(pCamera, pGrabframeBuff, pCamera->sensor->bufferSize);
            if (nRet != MI_CAMERA_SUCCESS)
            {
                printf("b");
            }
        } while (nRet != MI_CAMERA_SUCCESS && count++ < MAX_BADFRAME_TRIES);

        //The data for 10 bpp needs to be swizzled on the Demo1 & Demo1A boards
        if (pCamera->sensor->imageType == MI_BAYER_10 && nSwizzleNeeded)
        {
            //We go through data 2 Bytes at a time
            for (i = 0; i < (int)frameSize; i += 2)
            {
                //data comes in as         and is changed to
                // Byte 1   Byte 0         Byte 1   Byte0
                //xxxxxx10 98765432       xxxxxx98 76543210 
                tempByte = pGrabframeBuff[i+0];
                pCameraBuff[i+0]  = (pGrabframeBuff[i+0] << 2) | (pGrabframeBuff[i+1]&0x03); 
                pCameraBuff[i+1]  = (tempByte >> 6); 
            }
        }
        else
             memcpy(pCameraBuff, pGrabframeBuff, frameSize);

        if (nRet != MI_CAMERA_SUCCESS)
            printf("B");
        else
        {
            fwrite(pCameraBuff, frameSize, 1, imfile);
            printf(".");
        }
    }

    printf("[done]\n");

    fclose(imfile); 
#endif

testExit:
    //stop the camera and clean up
    pCamera->stopTransport(pCamera);
    mi_CloseCameras();
    free(pGrabframeBuff);
    free(pCameraBuff);
    mi_CloseErrorLog();

#ifdef _DEBUG
    printf("<press a key>");
    _getch();
    printf("\n");
#endif

    return 0;
}
