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

 SafeResources.h

*********************************************************************/

#ifndef _H_SafeResources
#define _H_SafeResources

#include <Carbon/Carbon.h>

#ifdef __cplusplus

class StAcroResourceContext
{
public:
				StAcroResourceContext();
				StAcroResourceContext(short resFile);
	virtual		~StAcroResourceContext();

protected :
	short		mOldResFile;
	short		mCurResFile;
};

#endif /* __cplusplus */

#endif /* _H_SafeResources */
