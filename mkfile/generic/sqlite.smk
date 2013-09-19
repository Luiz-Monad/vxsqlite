#------------------------------------------------------------ 
#
#       SQLITE for VERIX
#
#-------------------------------------------------------------


#
# Paths
#

VRXSDK   =  C:\VerixAps\VFSDK\p2.1.5
ACT2000  =  C:\VerixAps\ACT2000\p2.0
VTCPIP   =  C:\VerixAps\TCPIP\P2.4.0M
VUCL     =  C:\VerixAps\UCL\P2.5.0M

ACTIncludes = $(ACT2000)\include
SDSIncludes = $(VRXSDK)\include

#  App Source Paths
SrcDir = ..\..\sqlite\$(VXSQLITE_VERSION)
VerixSrcDir = ..\..\verix

ACTIncludes = $(VACT2000)\include

#  Compiler/Linker/Outhdr Output Paths
ObjDir = ..\..\build\obj\lib
OutDir = ..\..\build\out\lib


ACTLibraries = $(ACT2000)\output\RV\files\static\Debug


#
# Options for Tools
#

# Compiler Options
Includes = -I$(SDSIncludes) -I$(ACTIncludes) -I$(VerixSrcDir)

# for release version change the COptions to 
#COptionsDBG =  -DLOGSYS_FLAG
COSOptions = -DSQLITE_OS_OTHER -DSQLITE_OS_VERIX
CSqliteOptions = -DSQLITE_DISABLE_LFS -DSQLITE_DISABLE_DIRSYNC
#CSqliteDebug = -DSQLITE_DEBUG
COptions =  $(COSOptions) $(COptionsDBG) $(CSqliteOptions) $(CSqliteDebug) -DNDEBUG=1 -O2

#
# Dependencies
#

ACTLibs =						\
	$(ACTLibraries)\act2000.a
	
AppObjects = \
        $(ObjDir)\sqlite3.o

#Using ACT static library.
	
#
#  sample Target Definition
#

$(OutDir)\VxSqlite.a : $(AppObjects)
  armar -r -create VxSqlite.a $(ACTLibs) $(AppObjects) 
	move  VxSqlite.a $(OutDir)

$(OSfile)\free.scf:    $(SDSFiles)\free.sc
    $(VRXSDK)\bin\mkscf -fgid2 $(SDSFiles)\free $(OSfile)\free.scf _notused
	

######  Compile #######
$(ObjDir)\sqlite3.o : $(SrcDir)\sqlite3.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\sqlite3.c
	move sqlite3.o	$(ObjDir)
