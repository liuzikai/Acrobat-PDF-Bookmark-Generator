/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1996-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 SafeResources.cpp

 - Glue implementation of "safe" version of Macintosh toolbox routines
   that put the plug-in's resource file on top of the resource chain
   before calling the "real" implementation of the trap. Note that all
   routines also save and restore the current resource file.

*********************************************************************/

#include <string.h>

#include "PICommon.h"
#include "SafeResources.h"
#include "PIMain.h"

static short gLocaleResFile = 0;

StAcroResourceContext::StAcroResourceContext()
	: mCurResFile(GetAcroPluginResourceMap())
{
	mOldResFile = CurResFile();
	
	if( mCurResFile )
		UseResFile(mCurResFile);
	
	if( gLocaleResFile )
		UseResFile(gLocaleResFile);
}

StAcroResourceContext::StAcroResourceContext(short resFile)
	: mCurResFile(resFile)
{
	mOldResFile = CurResFile();

	if( mCurResFile )
		UseResFile(mCurResFile);
}

StAcroResourceContext::~StAcroResourceContext( )
{
	::UseResFile( mOldResFile );
} 
