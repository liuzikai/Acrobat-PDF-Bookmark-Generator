#!/bin/sh

# MacCopyToPlugins.sh
#
# *********************************************************************
#
# ADOBE SYSTEMS INCORPORATED
# Copyright (C) 2006-2008 Adobe Systems Incorporated
# All rights reserved.
#
# NOTICE: Adobe permits you to use, modify, and distribute this file
# in accordance with the terms of the Adobe license agreement
# accompanying it. If you have received this file from a source other
# than Adobe, then your use, modification, or distribution of it
# requires the prior written permission of Adobe.
#
# *********************************************************************
#
# This script runs in the last build phase of each plug-in target.
# It copies a symbolic link to the plug-in package into the 
# Acrobat plug-ins directory.
# The Acrobat plug-ins directory is set to the default install 
# location for Acrobat in the Environment.xcconfig file.
#
# If you update this file you must clean your target in order to
# read the script updates
#
# Commented out below is the replacement script to make a copy of
# the plug-in itself rather than a symlink
#
# IMPORTANT:
# If you try to launch Acrobat with an orphan plug-in symlink in its 
# plug-in directory, it will crash.
# This can occur if you clean your target, which will delete your plug-in 
# package from your BuildResults tree, but will *NOT* delete the symlink 
# copied by this script to the actual Acrobat plug-ins folder.
# It can also occur if you move or delete the original.
#
# *********************************************************************

# The script can be used to copy your plugin in the acrobat plugins folder
# Please specify the correct path for the Acrobat plugins folder in the ../MacSDKConfigurations/Environment.xcconfig
# Please change the permissions of the acrobat plugins folder.
# Uncomment the commands to use the script.


# SYMLINK=${ACROBAT_PLUGINS_FOLDER}/${PRODUCT_NAME}.acroplugin

# if symlink exists delete it
# if  [ -L "$SYMLINK" ]; then 
# 	rm "$SYMLINK"
# fi

# create a new symlink for this build
#use echo and xargs to handle path names with spaces properly

# SOURCE=${DSTROOT}/${PRODUCT_NAME}.acroplugin
# TARGET=${ACROBAT_PLUGINS_FOLDER}

# 	SOURCEANDTARGET=`(echo \'${SOURCE}\' ;echo \'${TARGET}\')`
# 	echo ${SOURCEANDTARGET} | xargs ln -s -v

# *******************************************************************

##### Create a new copy of plug-in instead of symlink #####

COPYFROM=${DSTROOT}/${PRODUCT_NAME}.acroplugin

COPYTO=${ACROBAT_PLUGINS_FOLDER}/${PRODUCT_NAME}.acroplugin

FROMANDTO=`(echo \'${COPYFROM}\' ;echo \'${COPYTO}\')`

echo $FROMANDTO | xargs ditto -rsrc -V






