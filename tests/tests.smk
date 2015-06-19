#------------------------------------------------------------ 
#
#       tests.SMK 
#
#-------------------------------------------------------------


#
# Paths
#

#VRXSDK   =  C:\VerixAps\VFSDK\p2.1.5
#ACT2000  =  C:\VerixAps\ACT2000\p2.0
#VTCPIP   =  C:\VerixAps\TCPIP\P2.4.0M
#VUCL     =  C:\VerixAps\UCL\P2.5.0M
ACT2000   =  $(EVOACT)
VRXSDK    =  $(EVOSDK)



# SDS Tools Related Paths

# Lib Include Paths
SDSIncludes        =  $(VRXSDK)\include
VTCPIncludes       =  $(VTCPIP)\include
VTCPVerixIncludes  =  $(VTCPIP)\Verixinclude
VUCLIncludes       =  $(VUCL)\Include
ACTIncludes        =  $(ACT2000)\include
EOSIncludes        =  $(EOSSDK)\include

#
# Project Path
#
SrcDir = .\

#  Compiler/Linker/Outhdr Output Paths
ObjDir = ..\build\obj\tests
OutDir = ..\build\out\tests


ACTLibraries	= $(ACT2000)\Output\RV\Files\Static\Release
VRXLibraries	= $(VRXSDK)\lib
EOSLibraries    = $(EOSSDK)\lib

# TCPIP Library Paths
VTCPLibraries = $(VTCPIP)\Output\RV\Files\Static\Debug

# UCL Library Paths
VUCLLibraries = $(VUCL)\Output\RV\Files\Static\Debug

#
# Options for Tools
#

# Compiler Options
Includes =  -I$(SDSIncludes) -I$(ACTIncludes) -I$(EOSIncludes) -I..\sqlite\$(VXSQLITE_VERSION) -I..\verix

# for release version change the COptions to 
COptDBG =  -DLOGSYS_FLAG
#COptDBG =  -DNDEBUG=1 -O2
COSOptions = -DSQLITE_OS_OTHER -DSQLITE_OS_VERIX
CSqliteOptions = -DSQLITE_DISABLE_LFS -DSQLITE_DISABLE_DIRSYNC \
	-DSQLITE_THREADSAFE -DSQLITE_OMIT_AUTHORIZATION \
	-DSQLITE_OMIT_UTF16 -DSQLITE_OMIT_TCL_VARIABLE
#CSqliteDebug = -DSQLITE_DEBUG
GeraMap =  -g -map
COptions = -b $(COSOptions) $(CSqliteOptions) $(COptDBG) -armcc,"--diag_suppress 177\,550\,1135\,962" $(GeraMap)

#
# Dependencies
#

AppObjects = 				\
	$(ObjDir)\tests.o		\
	$(ObjDir)\sqlite3.o

#Using static library.

Libs =							\
	$(ACTLibraries)\act2000.a	\
	$(EOSLibraries)\svc_net.o	\
	$(EOSLibraries)\ssl.o		\
	$(EOSLibraries)\ceif.o		\
	$(EOSLibraries)\elog.o		\
	$(VRXLibraries)\ctls11.lib	\
	$(VRXLibraries)\voyn.lib	\
	$(VRXLibraries)\VxSTLn.a

#Using dinamic library.

SharedLibs =					\
	-lnet.lib=N:/net.lib		\
	-lceif.lib=N:/ceif.lib		\
	-lelog.lib=N:/elog.lib		\
	-lssl.lib=N:/ssl.lib

#
#  sample Target Definition
#

pseudoOut : $(OutDir)\tests.out
    $(VRXSDK)\bin\vrxhdr -s 131072 -h 1048576 $(OutDir)\tests.out $(SharedLibs)

#For 3xxx, use the following lines to sign the out and scf file. 
	
#	"$(VSFSTOOL)\filesignature" .\tests.fst -nogui
#	"$(VSFSTOOL)\filesignature" -C Certif.crt -F .\tests.fst -L -CM

	rename $(OutDir)\tests.out.p7s tests.p7s
	
$(OutDir)\tests.out : $(AppObjects)
  $(VRXSDK)\bin\vrxcc $(COptions) $(AppObjects) $(Libs) -o tests.out
	move  tests.out $(OutDir)

$(OSfile)\free.scf:    $(SDSFiles)\free.sc
    $(VRXSDK)\bin\mkscf -fgid2 $(SDSFiles)\free $(OSfile)\free.scf _notused
	

######  Compile #######

$(ObjDir)\tests.o : $(SrcDir)\tests.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(FuelPayIncludes) $(SrcDir)\tests.c 
	move tests.o	$(ObjDir)

$(ObjDir)\sqlite3.o : $(SrcDir)\..\sqlite\$(VXSQLITE_VERSION)\sqlite3.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\..\sqlite\$(VXSQLITE_VERSION)\sqlite3.c 
	move sqlite3.o	$(ObjDir)

