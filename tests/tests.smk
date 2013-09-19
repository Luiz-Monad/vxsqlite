#------------------------------------------------------------ 
#
#       tests.SMK 
#
#-------------------------------------------------------------


#
# Paths
#

VRXSDK   =  C:\VerixAps\VFSDK\p2.1.5
ACT2000  =  C:\VerixAps\ACT2000\p2.0
VTCPIP   =  C:\VerixAps\TCPIP\P2.4.0M
VUCL     =  C:\VerixAps\UCL\P2.5.0M


# SDS Tools Related Paths

# Lib Include Paths
SDSIncludes = $(VRXSDK)\include
VTCPIncludes       =  $(VTCPIP)\include
VTCPVerixIncludes  =  $(VTCPIP)\Verixinclude
VUCLIncludes       =  $(VUCL)\Include
ACTIncludes = $(ACT2000)\include

#
# Project Path
#
SrcDir = .\

#  Compiler/Linker/Outhdr Output Paths
ObjDir = ..\build\obj\tests
OutDir = ..\build\out\tests


ACTLibraries = $(ACT2000)\output\RV\files\static\Debug

# TCPIP Library Paths
VTCPLibraries = $(VTCPIP)\Output\RV\Files\Static\Debug

# UCL Library Paths
VUCLLibraries = $(VUCL)\Output\RV\Files\Static\Debug

#
# Options for Tools
#

# Compiler Options
Includes =  -I$(SDSIncludes) -I$(ACTIncludes) -I..\sqlite\$(VXSQLITE_VERSION)

# for release version change the COptions to 
COptDBG =  -DLOGSYS_FLAG
#GeraMap =  -g -map
COptions = $(COptDBG) -armcc,"--diag_suppress 177\,550\,1135\,962" $(GeraMap)

#
# Dependencies
#

AppObjects = \
	$(ObjDir)\tests.o

#Using ACT static library.

ACTLibs =						\
	$(ACTLibraries)\act2000.a \
	..\build\out\lib\vxsqlite.a
	
#
#  sample Target Definition
#

pseudoOut : $(OutDir)\tests.out
    $(VRXSDK)\bin\vrxhdr -s 220000 -h 220000 $(OutDir)\tests.out

#For 3xxx, use the following lines to sign the out and scf file. 
	
	"$(VSFSTOOL)\filesignature" .\tests.fst -nogui

	rename $(OutDir)\tests.out.p7s tests.p7s
	
$(OutDir)\tests.out : $(AppObjects)
  $(VRXSDK)\bin\vrxcc $(COptions) $(AppObjects) $(ACTLibs) -o tests.out
	move  tests.out $(OutDir)

$(OSfile)\free.scf:    $(SDSFiles)\free.sc
    $(VRXSDK)\bin\mkscf -fgid2 $(SDSFiles)\free $(OSfile)\free.scf _notused
	

######  Compile #######

$(ObjDir)\tests.o : $(SrcDir)\tests.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(FuelPayIncludes) $(SrcDir)\tests.c 
	move tests.o	$(ObjDir)
