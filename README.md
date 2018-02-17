An Adobe Acrobat DC plug-in to generate PDF bookmarks (catalog) based on given catalog information.

Author: liuzikai
Date: 02/18/2018

Revised based on Adobe Arcobat DC SDK project BasicPlugin

Develop and Test Environment:
* macOS 10.13.3
* Adobe Acrobat Pro DC Version 2018.009.20050
* Xcode Version 9.2

There are some JS scripts on the Internet that have similar function. However, these scripts can only make go-to-page actions with JS code, which is not supported by some PDF readers such as macOS Preview. But using plug-in, native jumping actions can be added to bookmarks.
![IMAGE](resources/C89E716F393116A6C7B229400693848A.jpg =505x566)

<br>

Install
---
**For macOS, Adobe Acrobat DC**
Copy **BookmarkGenerator.acroplugin** to `/Users/(username)/Library/Application Support/Adobe/Acrobat/DC/Plug-ins`.

<br>

[中文版说明]()

Usage
---
###1. Prepare a txt file containing catalog infomation.  
**Format:**
`Index Title PageNumber`
&ensp;Use `.` to show levels in index.
&ensp;Title can contain spaces.
&ensp;`Index Title` will be used as the name of bookmark.

For example:
![2-01.png](resources/DD6CE512A2C20CC41BC820BA62FC5F96.png =408x147)

The plug-in will use the first index level as "basic level", for example:
![Untitled-1-01.png](resources/1EFF63D43FDF3E8A273654DB6A6C5F92.png =408x147)

Please make sure there is no excess new lines at the end of the txt file.

If you have catalog pages in the PDF file, you can copy it out (maybe export to other form first) and process the text with some technology such as regular expressions.

<br>

###2. Open PDF file in Acrobat and switch to the first page of main body
Since the given page numbers may be different from the actual page number for PDF file, the plug-in need to confirm the first page of main body. For example,
![IMAGE](resources/442839AD4595D327A8B0C77B624345E5.jpg =220x42)
Here is the the first page of main body. However, for PDF file, it is actually page 23.
<br>
###3. Start the plug-in on menubar Plug-In Tool - Generate Bookmarks...
###4. Confirm the first page of main body.
###5. Select the catalog txt file.
###6. Save the PDF file.

<br>

For Developers
---
This plug-in is developed on macOS and for Acrobat DC. It seems that it can't be directly applied to Win or other version of Acrobat. If you are interested in it, it would be great if you can help developing other versions. :)

Some tips:
1. Adobe Acrobat DC SDK for Mac is developed based on old Xcode and macOS, so some revisions are needed.
2. For Acrobat DC, it seems that plug-ins won't be valid if directly put in the application package.
3. The file **MacCopyToPlugins.sh** is a script provided by SDK that runs after building. It can perform work such as copying the plug-in to the corresponding folder. In current project, please change the `ACROBAT_PLUGINS_FOLDER` in **Environment.xcconfig** to your own path. 

<br>

Reference
---
[Adobe Acrobat DC SDK for Mac](https://www.adobe.com/devnet/acrobat.html)
[Acrobat DC SDK Documentation](https://help.adobe.com/en_US/acrobat/acrobat_dc_sdk/2015/HTMLHelp/#t=Acro12_MasterBook%2FIntroduction_Help_TitlePage%2FAbout_This_Help.htm)
[Acrobat and PDF Library API Reference](https://help.adobe.com/en_US/acrobat/acrobat_dc_sdk/2015/HTMLHelp/Acro12_MasterBook/API_References_SectionPage/API_References/Acrobat_API_Reference/index.html)
[在Acrobat中利用Javascript根据目录一键生成带页码的书签](http://blog.51cto.com/2134255/641839)
