/*********************************************************************
 
 File: BookmarkGenerator.cpp
 Description: Functions of the plug-in.
 Author: liuzikai
 Revise based on Adobe Acrobat DC SDK BasicPlugin.cpp
 Data: 02/16/2018
 
 *********************************************************************/



// Acrobat Headers.
#ifndef MAC_PLATFORM
#include "PIHeaders.h"
#endif

#include <iostream>
#include <fstream>


const char* MyPluginExtensionName = "ADBE:BookmarkGenerator";

//A convenient function to add a menu item for your plugin.
ACCB1 ASBool ACCB2 PluginMenuItem(char* MyMenuItemTitle, char* MyMenuItemName);

ACCB1 ASBool ACCB2 MyPluginSetmenu()
{
	return PluginMenuItem("Generate Bookmarks...", "ADBE:BookmarkGeneratorMenu");
}

// Show a dialog allowing users to select the catalog file.
ASPathName OpenCatalogFile(ASFileSys * ASF)
{
    AVOpenSaveDialogParamsRec dialogParams;
    
    //Local variables
    AVFileFilterRec filterRec,*filterRecP ;
    AVFileDescRec descRec;
    ASPathName * pathName = NULL;
    ASFileSys fileSys = NULL;
    ASBool bSelected = false;
    char errorBuf[256];
    
    //Set up the PDF file filter description
    strcpy (descRec.extension, "txt");
    descRec.macFileType = 0;
    descRec.macFileCreator = 0;
    
    //Set attributes that belong to the AVFileFilterRec object
    memset (&filterRec, 0, sizeof(AVFileFilterRec));
    filterRec.fileDescs = &descRec;
    filterRec.numFileDescs = 1;
    filterRecP = &filterRec;
    
    //Set attributes that belong to the AVOpenSaveDialogParamsRec object
    memset (&dialogParams, 0, sizeof (AVOpenSaveDialogParamsRec));
    dialogParams.size = sizeof(AVOpenSaveDialogParamsRec);
    dialogParams.fileFilters = &filterRecP;
    dialogParams.numFileFilters = 1;
    
    DURING
    //Set the AVFileFilterRec object’s filterDescription attribute
    filterRec.filterDescription = ASTextNew();
    ASTextSetEncoded (filterRec.filterDescription, "TXT Files",
                      ASScriptToHostEncoding(kASRomanScript));
    
    //Set the AVOpenSaveDialogParamsRec object's windowTitle attribute
    dialogParams.windowTitle = ASTextNew();
    ASTextSetEncoded (dialogParams.windowTitle, "Select the Catalog File", ASScriptToHostEncoding(kASRomanScript));
                      
    //Display the Open dialog box - pass the address of the ASFileSys object
    bSelected = AVAppOpenDialog(&dialogParams, &fileSys, (ASPathName**)&pathName, NULL, NULL);

    HANDLER
    //Display an error message to the user
    ASGetErrorString (ASGetExceptionErrorCode(), errorBuf, 256);
    AVAlertNote (errorBuf);
    END_HANDLER

    //Destroy the ASText object then return
    ASTextDestroy (filterRec.filterDescription);
    ASTextDestroy (dialogParams.windowTitle);

    //Point the ASFileSys argument to the address of the ASFileSys object
    *ASF = fileSys;

    return bSelected ? *pathName : NULL;
}

// Allow users to select the catalog file and return its content.
bool SelectAndReadCatalogFile(std::string* result) {
    ASPathName pathName;
    ASInt32 retVal;
    ASFileSys myFileSys;
    ASFile myFile;
    ASTFilePos myFileSize;
    
    //Allow user to select the catalog txt file.
    pathName = OpenCatalogFile(&myFileSys);
    
    //If PathName is valid
    if (pathName) {
        
        //Open the file specified in the file system
        retVal = ASFileSysOpenFile (myFileSys, pathName, ASFILE_READ, &myFile);
        
        if (retVal == 0) {
            
            myFileSize = ASFileGetEOF(myFile);
            (*result).resize(myFileSize);
            ASFileRead(myFile, &(*result)[0], myFileSize);
            
            return true;
        }
    }
    return false;
}

//Generate bookmarks
std::string catalogContent;
ACCB1 void ACCB2 MyPluginCommand(void *clientData)
{
    
    char str[256];
    
	// Try to get front PDF document
	AVDoc avDoc = AVAppGetActiveDoc();
    
	if(avDoc == NULL) {
		// If no doc is loaded, make a message.
		sprintf(str, "%s", "There is no PDF document loaded in Acrobat.");
	}
	else {
        
        ASInt32 basicPageNumber = AVPageViewGetPageNum(AVDocGetPageView(avDoc)) + 1;
        
        sprintf(str, "Is current page (%d) the the first page of main body?", basicPageNumber);
        if (AVAlertConfirm(str)) {
        
            PDDoc pdDoc = AVDocGetPDDoc(avDoc);
            
            PDBookmark rootBookmark;
            
            //Store levels of bookmarks
            PDBookmark currentBookmark[20];
            
            //Get the root bookmark
            rootBookmark = PDDocGetBookmarkRoot(pdDoc);
            
            if (PDBookmarkIsValid(rootBookmark)){
                
                //Add a child bookmark to the root bookmark
                currentBookmark[0] = PDBookmarkAddNewChild(rootBookmark, "Contents");
                
                if (PDBookmarkIsValid(currentBookmark[0])){
                    
                    if (SelectAndReadCatalogFile(&catalogContent)) {
                    
                        if (catalogContent[catalogContent.length() - 1] != '\n') catalogContent.append("\n");
                        
                        int basicDotCount = -2;
                        
                        unsigned long i = 0, j, s = 0, e;
                        while ((e = catalogContent.find_first_of("\n", s + 1)) != std::string::npos)  {
                            
                            int dotCount = 0;
                            while (catalogContent[i] != ' ') {
                                if (catalogContent[i] == '.') dotCount++;
                                i++;
                            }
                            //Use the dotCount for the first bookmark as basicDotCount
                            if (basicDotCount == -2) basicDotCount = dotCount - 1;
                            
                            std::string pageNumber;
                            for(j = e - 1; j > i; j--){
                                if (catalogContent[j] >= '0' && catalogContent[j] <= '9') {
                                    pageNumber = catalogContent[j] + pageNumber;
                                } else {
                                    break;
                                }
                            }
                            
                            std::string bookmarkName = catalogContent.substr(s, j - s);
                            
                            i = s = e + 1;
                            
                            currentBookmark[dotCount - basicDotCount] = PDBookmarkAddNewChild(currentBookmark[dotCount - basicDotCount - 1], bookmarkName.c_str());
                            
                            //Create a AVPageView object that represents the page view of a document
                            AVPageView myPageView = AVDocGetPageView(AVDocFromPDDoc(pdDoc));
                            
                            //Set the page view to the page
                            //The page number for PDF file starts from 0
                            ASInt32 pNum = atoi(pageNumber.c_str()) - 1 + basicPageNumber - 1;
                            AVPageViewGoTo(myPageView,pNum);
                            
                            //Create an PDViewDestination object that is used to create a PDAction object
                            PDViewDestination pdvDes = AVPageViewToViewDest(myPageView, ASAtomFromString("Fit"), pdDoc);
                            
                            //Create a PDAction object
                            PDAction myAction = PDActionNewFromDest(pdDoc,pdvDes,pdDoc);
                            
                            //Attach an action to the bookmark
                            PDBookmarkSetAction(currentBookmark[dotCount - basicDotCount], myAction);
                        }
                        sprintf(str, "%s", "Complete generating bookmarks!");
                    
                    } else {
                        sprintf(str, "%s", "Failed to open catalog file.");
                    }
                } else {
                    sprintf(str, "%s", "Failed to the create root bookmark \"Contents\".");
                }
            } else {
                sprintf(str, "%s", "Failed to get the bookmark root.");
            }
        } else {
            sprintf(str, "%s", "Cancelled!");
        }
	}

	// display message
	AVAlertNote(str);
    
	return;
}

ACCB1 ASBool ACCB2 MyPluginIsEnabled(void *clientData)
{
	return (AVAppGetActiveDoc() != NULL);
}

