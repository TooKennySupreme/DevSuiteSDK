//**************************************************************************************
// Copyright 2012 Aptina Imaging Corporation. All rights reserved.
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
// This software and any associated documentation are provided "AS IS" and
// without warranty of any kind. APTINA IMAGING CORPORATION EXPRESSLY DISCLAIMS
// ALL WARRANTIES EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO, NONINFRINGEMENT
// OF THIRD PARTY RIGHTS, AND ANY IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS
// FOR A PARTICULAR PURPOSE. APTINA DOES NOT WARRANT THAT THE FUNCTIONS CONTAINED
// IN THIS SOFTWARE WILL MEET YOUR REQUIREMENTS, OR THAT THE OPERATION OF THIS SOFTWARE
// WILL BE UNINTERRUPTED OR ERROR-FREE. FURTHERMORE, APTINA DOES NOT WARRANT OR
// MAKE ANY REPRESENTATIONS REGARDING THE USE OR THE RESULTS OF THE USE OF ANY
// ACCOMPANYING DOCUMENTATION IN TERMS OF ITS CORRECTNESS, ACCURACY, RELIABILITY,
// OR OTHERWISE.
//*************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <conio.h>
#else
#define _getch getchar
#define sscanf_s sscanf
#endif
#include "apbase.h"

/*
 * default amount of frames to capture...can also specify on command line
 */
#define DEFAULT_NUM_FRAMES      1

/*
 * the amount of tries before giving up trying to grab a valid frame. 
 */
#define MAX_BADFRAME_TRIES      10

//
//  Callback for INI PROMPT= command. Some initialization files
//  use this.
//
int APBASE_DECL MultipleChoice(void *pContext,
                               const char *szMessage,
                               const char *szChoices)
{
    int         nChoices = 0;

    if (szMessage)
        printf("\n%s\n", szMessage);

    if (szChoices == NULL || szChoices[0] == 0)
        return 0;   //   no choices, just an informational message

    printf("%4d. Cancel\n", -1);
    printf("%4d. Skip this\n", 0);
    for (const char *p = szChoices; nChoices < 25; ++p)
    {
        if (strlen(p) == 0)
            break;
        ++nChoices;
        printf("%4d. %s\n", nChoices, p);
        while (*p)
            ++p;
    }

    printf("Enter selection [1]: ");
    char s[256];
    fgets(s, sizeof(s), stdin);
    int n = 1;
    sscanf_s(s, "%i", &n);
    return n;
}

//
//  ApBase callbacks for log messages and error messages.
//  Not essential, but here for demonstration
//
//  Callback for INI file LOG= command
void APBASE_DECL MyLogComment(void *pContext, const char *szComment)
{
    printf("%s\n", szComment);
}

//  Callback for user query (based on MessageBox)
int APBASE_DECL MyErrorMessage(void *pContext, const char *szMessage, unsigned int mbType)
{
    printf("%s\n", szMessage);
    switch (mbType & 0xF)
    {
    case AP_MSGTYPE_OK:                 return AP_MSG_OK;
    case AP_MSGTYPE_OKCANCEL:           return AP_MSG_OK;
    case AP_MSGTYPE_ABORTRETRYIGNORE:   return AP_MSG_ABORT;
    case AP_MSGTYPE_YESNOCANCEL:        return AP_MSG_YES;
    case AP_MSGTYPE_YESNO:              return AP_MSG_YES;
    case AP_MSGTYPE_RETRYCANCEL:        return AP_MSG_CANCEL;
    case AP_MSGTYPE_CANCELTRYCONTINUE:  return AP_MSG_CANCEL;
    }
    return AP_MSG_OK;
}

//  Callback for Python print() calls, etc.
void APBASE_DECL MyScriptOutput(void *pContext, int nUnit, const char *szString)
{
    switch (nUnit)
    {
    case 0:
    default:
        printf("%s", szString);
        break;
    case 1:
        fprintf(stderr, "%s", szString);
        break;
    }
}


int main(int argc, char* argv[])
{
    AP_HANDLE       apbase;
    unsigned long   frameLength;                        //size of the frames from the camera
    unsigned char   *pBuffer;                           //grabFrame buffer
    size_t          nBufferSize;                        //size of the buffer
    unsigned int    num_frames = DEFAULT_NUM_FRAMES;    //number of frames to capture
    ap_u32          nWidth  = 0;                        //width of image (taken from command line or set to default below)
    ap_u32          nHeight = 0;                        //height of image (taken from command line or set to default below)
    FILE            *imfile;                            //capture file
    unsigned int    frame;
    char            imagetypestr[64];

    printf("usage:\n");
    printf("    SimpleCapture\n");
    printf("    SimpleCapture <num_frames>\n");
    printf("    SimpleCapture <num_frames> <width> <height>\n");

    //parse command line for the number of frames to capture and the width and height (if not default)
    switch (argc)
    {
    case 2:
        num_frames = atoi(argv[1]);
        break;
    case 4:
        num_frames = atoi(argv[1]);
        nWidth     = atoi(argv[2]);
        nHeight    = atoi(argv[3]);
        break;
    }

    //  Just take the first device
    apbase = ap_Create(0);

    //we need at least one camera protocol to use the sensor
    if (apbase == NULL)
    {
        printf("Could not initialize camera. \n");
        ap_Finalize();
#ifdef _DEBUG
        printf("<press a key>");
        _getch();
        printf("\n");
#endif
        return 1;
    }

    //  Application functions that scripts may use
    ap_SetCallback_MultipleChoice(apbase, MultipleChoice, NULL);
    ap_SetCallback_LogComment(apbase, MyLogComment, NULL);
    ap_SetCallback_ErrorMessage(apbase, MyErrorMessage, NULL);
    ap_SetCallback_ScriptOutput(apbase, MyScriptOutput, NULL);

    //  For informational purposes, enable log for SHIP commands during ap_LoadIniPreset
    ap_OpenIoLog(AP_LOG_SHIP, "ship_log_.txt");
    printf("Log file: %s \n", ap_GetIoLogFilename());

    //  Load default initialization preset [Demo Initialization] in default ini file
    ap_LoadIniPreset(apbase, NULL, NULL);

    //  Command line dimensions override
    if (nWidth && nHeight)
        ap_SetImageFormat(apbase, nWidth, nHeight, NULL);

    //display some stats
    ap_GetImageFormat(apbase, &nWidth, &nHeight, imagetypestr, sizeof(imagetypestr));
    printf("partNumber = %s \n", ap_GetPartNumber(apbase));
    printf("width = %d \n", nWidth);
    printf("height = %d \n", nHeight);
    printf("imageType = %s \n", imagetypestr);
    printf("bufferSize = %d \n", ap_GrabFrame(apbase, NULL, 0));
    printf("shipAddr = 0x%2X \n", ap_GetShipAddr(apbase));
    
    //Allocate a buffer to store the images
    nBufferSize = ap_GrabFrame(apbase, NULL, 0);
    pBuffer  = (unsigned char *)malloc(nBufferSize);
    if (pBuffer == NULL)
    {
        printf("Error trying to create a buffer of size %lu to grab the frames.\n", 
               (unsigned long)nBufferSize);
        ap_Finalize();
#ifdef _DEBUG
        printf("<press a key>");
        _getch();
        printf("\n");
#endif
        return 1;
    }

    printf("Saving frames to img.raw...");
    imfile = fopen("img.raw","wb");

    //grabFrame 
    printf("Grabbing frame \n");
    for (frame = 0; frame < num_frames; frame++)
    {
        /*
         * skip frames until a good frame is found.
         */
        int count = 0;
        do
        {
            frameLength = ap_GrabFrame(apbase, pBuffer, nBufferSize);
            if (ap_GetLastError() == AP_CAMERA_SUCCESS)
                break;
            printf("b");
        } while (count++ < MAX_BADFRAME_TRIES);

        if (ap_GetLastError() != AP_CAMERA_SUCCESS)
            printf("B (error code: %d)", ap_GetLastError());
        else
        {
            fwrite(pBuffer, frameLength, 1, imfile);
            printf(".");
        }
    }
    printf("[done]\n");

    fclose(imfile);
    
    //stop the camera and clean up
    ap_Finalize();
    free(pBuffer);
    ap_CloseIoLog();

#ifdef _DEBUG
    printf("<press a key>");
    _getch();
    printf("\n");
#endif

    return 0;
}
