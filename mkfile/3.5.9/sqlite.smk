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
SrcDir = ..\..\sqlite\3.5.9

ACTIncludes = $(VACT2000)\include

#  Compiler/Linker/Outhdr Output Paths
ObjDir = ..\..\build\obj\lib
OutDir = ..\..\build\out\lib


ACTLibraries = $(ACT2000)\output\RV\files\static\Debug


#
# Options for Tools
#

# Compiler Options
Includes = -I$(SDSIncludes) -I$(ACTIncludes)

# for release version change the COptions to 
#COptionsDBG =  -DLOGSYS_FLAG
COptions =  $(COptionsDBG) -DNDEBUG=1 -O2

#
# Dependencies
#

ACTLibs =						\
	$(ACTLibraries)\act2000.a
	
AppObjects = \
        $(ObjDir)\alter.o\
        $(ObjDir)\analyze.o\
        $(ObjDir)\attach.o\
        $(ObjDir)\auth.o\
        $(ObjDir)\bitvec.o\
        $(ObjDir)\btmutex.o\
        $(ObjDir)\btree.o\
        $(ObjDir)\build.o\
        $(ObjDir)\callback.o\
        $(ObjDir)\complete.o\
        $(ObjDir)\date.o\
        $(ObjDir)\delete.o\
        $(ObjDir)\expr.o\
        $(ObjDir)\fault.o\
        $(ObjDir)\fts3.o\
        $(ObjDir)\fts3_hash.o\
        $(ObjDir)\fts3_icu.o\
        $(ObjDir)\fts3_porter.o\
        $(ObjDir)\fts3_tokenizer.o\
        $(ObjDir)\fts3_tokenizer1.o\
        $(ObjDir)\func.o\
        $(ObjDir)\hash.o\
        $(ObjDir)\insert.o\
        $(ObjDir)\journal.o\
        $(ObjDir)\legacy.o\
        $(ObjDir)\loadext.o\
        $(ObjDir)\main.o\
        $(ObjDir)\malloc.o\
        $(ObjDir)\mem1.o\
        $(ObjDir)\mem2.o\
        $(ObjDir)\mem3.o\
        $(ObjDir)\mem4.o\
        $(ObjDir)\mem5.o\
        $(ObjDir)\mutex.o\
        $(ObjDir)\opcodes.o\
        $(ObjDir)\os.o\
        $(ObjDir)\os_verix.o\
        $(ObjDir)\pager.o\
        $(ObjDir)\parse.o\
        $(ObjDir)\pragma.o\
        $(ObjDir)\prepare.o\
        $(ObjDir)\printf.o\
        $(ObjDir)\random.o\
        $(ObjDir)\select.o\
        $(ObjDir)\table.o\
        $(ObjDir)\tokenize.o\
        $(ObjDir)\trigger.o\
        $(ObjDir)\update.o\
        $(ObjDir)\utf.o\
        $(ObjDir)\util.o\
        $(ObjDir)\vacuum.o\
        $(ObjDir)\vdbe.o\
        $(ObjDir)\vdbeapi.o\
        $(ObjDir)\vdbeaux.o\
        $(ObjDir)\vdbeblob.o\
        $(ObjDir)\vdbefifo.o\
        $(ObjDir)\vdbemem.o\
        $(ObjDir)\vtab.o\
        $(ObjDir)\where.o

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
$(ObjDir)\alter.o : $(SrcDir)\alter.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\alter.c
	move alter.o	$(ObjDir)

$(ObjDir)\analyze.o : $(SrcDir)\analyze.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\analyze.c
	move analyze.o	$(ObjDir)
	
$(ObjDir)\attach.o : $(SrcDir)\attach.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\attach.c
	move attach.o	$(ObjDir)

$(ObjDir)\auth.o : $(SrcDir)\auth.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\auth.c
	move auth.o	$(ObjDir)

$(ObjDir)\bitvec.o : $(SrcDir)\bitvec.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\bitvec.c
	move bitvec.o	$(ObjDir)
	
$(ObjDir)\btmutex.o : $(SrcDir)\btmutex.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\btmutex.c
	move btmutex.o	$(ObjDir)
	
$(ObjDir)\btree.o : $(SrcDir)\btree.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\btree.c
	move btree.o	$(ObjDir)
	
$(ObjDir)\build.o : $(SrcDir)\build.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\build.c
	move build.o	$(ObjDir)
	
$(ObjDir)\callback.o : $(SrcDir)\callback.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\callback.c
	move callback.o	$(ObjDir)
	
$(ObjDir)\complete.o : $(SrcDir)\complete.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\complete.c
	move complete.o	$(ObjDir)					

$(ObjDir)\date.o : $(SrcDir)\date.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\date.c
	move date.o	$(ObjDir)
	
$(ObjDir)\delete.o : $(SrcDir)\delete.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\delete.c
	move delete.o	$(ObjDir)
	
$(ObjDir)\expr.o : $(SrcDir)\expr.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\expr.c
	move expr.o	$(ObjDir)
	
$(ObjDir)\fault.o : $(SrcDir)\fault.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\fault.c
	move fault.o	$(ObjDir)	
	
$(ObjDir)\fts3.o : $(SrcDir)\fts3.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\fts3.c
	move fts3.o	$(ObjDir)		
	
$(ObjDir)\fts3_hash.o : $(SrcDir)\fts3_hash.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\fts3_hash.c
	move fts3_hash.o	$(ObjDir)	
	
$(ObjDir)\fts3_icu.o : $(SrcDir)\fts3_icu.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\fts3_icu.c
	move fts3_icu.o	$(ObjDir)	
	
$(ObjDir)\fts3_porter.o : $(SrcDir)\fts3_porter.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\fts3_porter.c
	move fts3_porter.o	$(ObjDir)	
	
$(ObjDir)\fts3_tokenizer.o : $(SrcDir)\fts3_tokenizer.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\fts3_tokenizer.c
	move fts3_tokenizer.o	$(ObjDir)	
	
$(ObjDir)\fts3_tokenizer1.o : $(SrcDir)\fts3_tokenizer1.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\fts3_tokenizer1.c
	move fts3_tokenizer1.o	$(ObjDir)						

$(ObjDir)\func.o : $(SrcDir)\func.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\func.c
	move func.o	$(ObjDir)						
	
$(ObjDir)\hash.o : $(SrcDir)\hash.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\hash.c
	move hash.o	$(ObjDir)						
	
$(ObjDir)\insert.o : $(SrcDir)\insert.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\insert.c
	move insert.o	$(ObjDir)						
	
$(ObjDir)\journal.o : $(SrcDir)\journal.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\journal.c
	move journal.o	$(ObjDir)						
				
$(ObjDir)\legacy.o : $(SrcDir)\legacy.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\legacy.c
	move legacy.o	$(ObjDir)						
	
$(ObjDir)\loadext.o : $(SrcDir)\loadext.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\loadext.c
	move loadext.o	$(ObjDir)						
	
$(ObjDir)\main.o : $(SrcDir)\main.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\main.c
	move main.o	$(ObjDir)						
	
$(ObjDir)\malloc.o : $(SrcDir)\malloc.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\malloc.c
	move malloc.o	$(ObjDir)	
	
$(ObjDir)\mem1.o : $(SrcDir)\mem1.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\mem1.c
	move mem1.o	$(ObjDir)						
	
$(ObjDir)\mem2.o : $(SrcDir)\mem2.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\mem2.c
	move mem2.o	$(ObjDir)		
	
$(ObjDir)\mem3.o : $(SrcDir)\mem3.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\mem3.c
	move mem3.o	$(ObjDir)						
	
$(ObjDir)\mem4.o : $(SrcDir)\mem4.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\mem4.c
	move mem4.o	$(ObjDir)		
	
$(ObjDir)\mem5.o : $(SrcDir)\mem5.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\mem5.c
	move mem5.o	$(ObjDir)		
	
$(ObjDir)\mutex.o : $(SrcDir)\mutex.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\mutex.c
	move mutex.o	$(ObjDir)	
	
$(ObjDir)\opcodes.o : $(SrcDir)\opcodes.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\opcodes.c
	move opcodes.o	$(ObjDir)		
	
$(ObjDir)\os.o : $(SrcDir)\os.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\os.c
	move os.o	$(ObjDir)		
	
$(ObjDir)\os_verix.o : $(SrcDir)\os_verix.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\os_verix.c
	move os_verix.o	$(ObjDir)	
	
$(ObjDir)\pager.o : $(SrcDir)\pager.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\pager.c
	move pager.o	$(ObjDir)		
	
$(ObjDir)\parse.o : $(SrcDir)\parse.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\parse.c
	move parse.o	$(ObjDir)		
	
$(ObjDir)\pragma.o : $(SrcDir)\pragma.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\pragma.c
	move pragma.o	$(ObjDir)			
	
$(ObjDir)\prepare.o : $(SrcDir)\prepare.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\prepare.c
	move prepare.o	$(ObjDir)		
	
$(ObjDir)\printf.o : $(SrcDir)\printf.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\printf.c
	move printf.o	$(ObjDir)		
	
$(ObjDir)\random.o : $(SrcDir)\random.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\random.c
	move random.o	$(ObjDir)		
	
$(ObjDir)\select.o : $(SrcDir)\select.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\select.c
	move select.o	$(ObjDir)			
		
$(ObjDir)\table.o : $(SrcDir)\table.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\table.c
	move table.o	$(ObjDir)		
		
$(ObjDir)\tokenize.o : $(SrcDir)\tokenize.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\tokenize.c
	move tokenize.o	$(ObjDir)			
	
$(ObjDir)\trigger.o : $(SrcDir)\trigger.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\trigger.c
	move trigger.o	$(ObjDir)		
	
$(ObjDir)\update.o : $(SrcDir)\update.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\update.c
	move update.o	$(ObjDir)		
	
$(ObjDir)\utf.o : $(SrcDir)\utf.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\utf.c
	move utf.o	$(ObjDir)		
	
$(ObjDir)\util.o : $(SrcDir)\util.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\util.c
	move util.o	$(ObjDir)	
	
$(ObjDir)\vacuum.o : $(SrcDir)\vacuum.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\vacuum.c
	move vacuum.o	$(ObjDir)		
	
$(ObjDir)\vdbe.o : $(SrcDir)\vdbe.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\vdbe.c
	move vdbe.o	$(ObjDir)	
	
$(ObjDir)\vdbeapi.o : $(SrcDir)\vdbeapi.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\vdbeapi.c
	move vdbeapi.o	$(ObjDir)		
	
$(ObjDir)\vdbeaux.o : $(SrcDir)\vdbeaux.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\vdbeaux.c
	move vdbeaux.o	$(ObjDir)			
	
$(ObjDir)\vdbeblob.o : $(SrcDir)\vdbeblob.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\vdbeblob.c
	move vdbeblob.o	$(ObjDir)		
	
$(ObjDir)\vdbefifo.o : $(SrcDir)\vdbefifo.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\vdbefifo.c
	move vdbefifo.o	$(ObjDir)		
	
$(ObjDir)\vdbemem.o : $(SrcDir)\vdbemem.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\vdbemem.c
	move vdbemem.o	$(ObjDir)	
	
$(ObjDir)\vtab.o : $(SrcDir)\vtab.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\vtab.c
	move vtab.o	$(ObjDir)	
	
$(ObjDir)\where.o : $(SrcDir)\where.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\where.c
	move where.o	$(ObjDir)							
					
