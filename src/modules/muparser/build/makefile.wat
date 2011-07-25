# =========================================================================
#     This makefile was generated by
#     Bakefile 0.2.1 (http://bakefile.sourceforge.net)
#     Do not modify, all changes will be overwritten!
# =========================================================================



# -------------------------------------------------------------------------
# These are configurable options:
# -------------------------------------------------------------------------

# C++ compiler 
CXX = wpp386

# Standard flags for C++ 
CXXFLAGS = 

# Standard preprocessor flags (common for CC and CXX) 
CPPFLAGS = 

# Standard linker flags 
LDFLAGS = 

# Set to 1 to build debug version [0,1]
#   0 - Release
#   1 - Debug
DEBUG = 0

# Set to 1 to build shared (DLL) version [0,1]
#   0 - Static
#   1 - DLL
SHARED = 0

# Set to 1 to compile samples [0,1]
SAMPLES = 1



# -------------------------------------------------------------------------
# Do not modify the rest of this file!
# -------------------------------------------------------------------------

# Speed up compilation a bit:
!ifdef __LOADDLL__
!  loaddll wcc      wccd
!  loaddll wccaxp   wccdaxp
!  loaddll wcc386   wccd386
!  loaddll wpp      wppdi86
!  loaddll wppaxp   wppdaxp
!  loaddll wpp386   wppd386
!  loaddll wlink    wlink
!  loaddll wlib     wlibd
!endif

# We need these variables in some bakefile-made rules:
WATCOM_CWD = $+ $(%cdrive):$(%cwd) $-

### Conditionally set variables: ###

DEBUGBUILDPOSTFIX =
!ifeq DEBUG 0
DEBUGBUILDPOSTFIX = rel
!endif
!ifeq DEBUG 1
DEBUGBUILDPOSTFIX = dbg
!endif
SHAREDBUILDPOSTFIX =
!ifeq SHARED 0
SHAREDBUILDPOSTFIX = static
!endif
!ifeq SHARED 1
SHAREDBUILDPOSTFIX = shared
!endif
DEBUG_BUILD_POSTFIX =
!ifeq DEBUG 0
DEBUG_BUILD_POSTFIX = 
!endif
!ifeq DEBUG 1
DEBUG_BUILD_POSTFIX = d
!endif
__muParser_lib___depname =
!ifeq SHARED 0
__muParser_lib___depname = ..\lib\muparser$(DEBUG_BUILD_POSTFIX).lib
!endif
__muParser_dll___depname =
!ifeq SHARED 1
__muParser_dll___depname = ..\lib\muparser$(DEBUG_BUILD_POSTFIX).dll
!endif
__example1___depname =
!ifeq SAMPLES 1
!ifeq SHARED 0
__example1___depname = ..\samples\example1\example1.exe
!endif
!endif
__example3___depname =
!ifeq SAMPLES 1
!ifeq SHARED 1
__example3___depname = ..\samples\example3\example3.exe
!endif
!endif
____SHARED =
!ifeq SHARED 0
____SHARED = 
!endif
!ifeq SHARED 1
____SHARED = -dMUPARSER_DLL
!endif
____SHARED_0 =
!ifeq SHARED 0
____SHARED_0 = 
!endif
!ifeq SHARED 1
____SHARED_0 = -dMUPARSERLIB_EXPORTS
!endif
____DEBUG =
!ifeq DEBUG 0
____DEBUG = -dNDEBUG
!endif
!ifeq DEBUG 1
____DEBUG = 
!endif
____DEBUG_3 =
!ifeq DEBUG 0
____DEBUG_3 = -ot -ox
!endif
!ifeq DEBUG 1
____DEBUG_3 = -od
!endif
____DEBUG_6 =
!ifeq DEBUG 0
____DEBUG_6 = -d0
!endif
!ifeq DEBUG 1
____DEBUG_6 = -d2
!endif
____DEBUG_7 =
!ifeq DEBUG 0
____DEBUG_7 = 
!endif
!ifeq DEBUG 1
____DEBUG_7 = debug all
!endif
____DEBUG_8 =
!ifeq DEBUG 0
____DEBUG_8 = 
!endif
!ifeq DEBUG 1
____DEBUG_8 = -wx
!endif
____SHARED_1 =
!ifeq SHARED 0
____SHARED_1 = 
!endif
!ifeq SHARED 1
____SHARED_1 = -dUSINGDLL
!endif

### Variables: ###

MUPARSER_LIB_CXXFLAGS = $(____DEBUG) $(____DEBUG_3) $(____DEBUG_6) &
	$(____DEBUG_8) $(____SHARED) $(____SHARED_0) -d_WIN32 -i=..\include &
	$(CPPFLAGS) $(CXXFLAGS)
MUPARSER_LIB_OBJECTS =  &
	obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_lib_muParserBytecode.obj &
	obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_lib_muParserTest.obj &
	obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_lib_muParser.obj &
	obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_lib_muParserDLL.obj &
	obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_lib_muParserInt.obj &
	obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_lib_muParserTokenReader.obj &
	obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_lib_muParserError.obj &
	obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_lib_muParserCallback.obj &
	obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_lib_muParserBase.obj
MUPARSER_DLL_CXXFLAGS = -bd $(____DEBUG) $(____DEBUG_3) $(____DEBUG_6) &
	$(____DEBUG_8) $(____SHARED) $(____SHARED_0) -d_WIN32 -i=..\include &
	$(CPPFLAGS) $(CXXFLAGS)
MUPARSER_DLL_OBJECTS =  &
	obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_dll_muParserBytecode.obj &
	obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_dll_muParserTest.obj &
	obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_dll_muParser.obj &
	obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_dll_muParserDLL.obj &
	obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_dll_muParserInt.obj &
	obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_dll_muParserTokenReader.obj &
	obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_dll_muParserError.obj &
	obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_dll_muParserCallback.obj &
	obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_dll_muParserBase.obj
EXAMPLE1_CXXFLAGS = $(____DEBUG) $(____DEBUG_3) $(____DEBUG_6) $(____DEBUG_8) &
	-i=..\include $(____SHARED_1) $(CPPFLAGS) $(CXXFLAGS)
EXAMPLE1_OBJECTS =  &
	obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\example1_Example1.obj
EXAMPLE3_CXXFLAGS = $(____DEBUG) $(____DEBUG_3) $(____DEBUG_6) $(____DEBUG_8) &
	-i=..\include $(____SHARED_1) $(CPPFLAGS) $(CXXFLAGS)
EXAMPLE3_OBJECTS =  &
	obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\example3_Example3.obj


all : obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)
obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX) :
	-if not exist obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX) mkdir obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)

### Targets: ###

all : .SYMBOLIC $(__muParser_lib___depname) $(__muParser_dll___depname) $(__example1___depname) $(__example3___depname)

clean : .SYMBOLIC 
	-if exist obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\*.obj del obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\*.obj
	-if exist obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\*.res del obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\*.res
	-if exist obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\*.lbc del obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\*.lbc
	-if exist obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\*.ilk del obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\*.ilk
	-if exist obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\*.pch del obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\*.pch
	-if exist ..\lib\muparser$(DEBUG_BUILD_POSTFIX).lib del ..\lib\muparser$(DEBUG_BUILD_POSTFIX).lib
	-if exist ..\lib\muparser$(DEBUG_BUILD_POSTFIX).dll del ..\lib\muparser$(DEBUG_BUILD_POSTFIX).dll
	-if exist ..\lib\muparser$(DEBUG_BUILD_POSTFIX).lib del ..\lib\muparser$(DEBUG_BUILD_POSTFIX).lib
	-if exist ..\samples\example1\example1.exe del ..\samples\example1\example1.exe
	-if exist ..\samples\example3\example3.exe del ..\samples\example3\example3.exe

!ifeq SHARED 0
..\lib\muparser$(DEBUG_BUILD_POSTFIX).lib :  $(MUPARSER_LIB_OBJECTS)
	@%create obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_lib.lbc
	@for %i in ($(MUPARSER_LIB_OBJECTS)) do @%append obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_lib.lbc +%i
	wlib -q -p4096 -n -b $^@ @obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_lib.lbc
!endif

!ifeq SHARED 1
..\lib\muparser$(DEBUG_BUILD_POSTFIX).dll :  $(MUPARSER_DLL_OBJECTS)
	@%create obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_dll.lbc
	@%append obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_dll.lbc option quiet
	@%append obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_dll.lbc name $^@
	@%append obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_dll.lbc option caseexact
	@%append obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_dll.lbc $(LDFLAGS) $(____DEBUG_7)
	@for %i in ($(MUPARSER_DLL_OBJECTS)) do @%append obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_dll.lbc file %i
	@for %i in () do @%append obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_dll.lbc library %i
	@%append obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_dll.lbc
	@%append obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_dll.lbc system nt_dll
	wlink @obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_dll.lbc
	wlib -q -n -b ..\lib\muparser$(DEBUG_BUILD_POSTFIX).lib +$^@
!endif

!ifeq SAMPLES 1
!ifeq SHARED 0
..\samples\example1\example1.exe :  $(EXAMPLE1_OBJECTS) $(__muParser_lib___depname)
	@%create obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\example1.lbc
	@%append obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\example1.lbc option quiet
	@%append obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\example1.lbc name $^@
	@%append obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\example1.lbc option caseexact
	@%append obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\example1.lbc $(LDFLAGS) $(____DEBUG_7) libpath ..\lib
	@for %i in ($(EXAMPLE1_OBJECTS)) do @%append obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\example1.lbc file %i
	@for %i in ( ..\lib\muparser$(DEBUG_BUILD_POSTFIX).lib) do @%append obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\example1.lbc library %i
	@%append obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\example1.lbc
	@for %i in () do @%append obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\example1.lbc option stack=%i
	wlink @obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\example1.lbc
!endif
!endif

!ifeq SAMPLES 1
!ifeq SHARED 1
..\samples\example3\example3.exe :  $(EXAMPLE3_OBJECTS) $(__muParser_lib___depname)
	@%create obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\example3.lbc
	@%append obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\example3.lbc option quiet
	@%append obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\example3.lbc name $^@
	@%append obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\example3.lbc option caseexact
	@%append obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\example3.lbc $(LDFLAGS) $(____DEBUG_7) libpath ..\lib
	@for %i in ($(EXAMPLE3_OBJECTS)) do @%append obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\example3.lbc file %i
	@for %i in ( ..\lib\muparser$(DEBUG_BUILD_POSTFIX).lib) do @%append obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\example3.lbc library %i
	@%append obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\example3.lbc
	@for %i in () do @%append obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\example3.lbc option stack=%i
	wlink @obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\example3.lbc
!endif
!endif

lib : .SYMBOLIC $(__muParser_lib___depname) $(__muParser_dll___depname)

samples : .SYMBOLIC $(__example1___depname) $(__example3___depname)

documentation :  
	( cd ..\docs && doxygen )

obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_lib_muParserBytecode.obj :  .AUTODEPEND ..\src\muParserBytecode.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MUPARSER_LIB_CXXFLAGS) $<

obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_lib_muParserTest.obj :  .AUTODEPEND ..\src\muParserTest.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MUPARSER_LIB_CXXFLAGS) $<

obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_lib_muParser.obj :  .AUTODEPEND ..\src\muParser.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MUPARSER_LIB_CXXFLAGS) $<

obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_lib_muParserDLL.obj :  .AUTODEPEND ..\src\muParserDLL.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MUPARSER_LIB_CXXFLAGS) $<

obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_lib_muParserInt.obj :  .AUTODEPEND ..\src\muParserInt.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MUPARSER_LIB_CXXFLAGS) $<

obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_lib_muParserTokenReader.obj :  .AUTODEPEND ..\src\muParserTokenReader.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MUPARSER_LIB_CXXFLAGS) $<

obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_lib_muParserError.obj :  .AUTODEPEND ..\src\muParserError.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MUPARSER_LIB_CXXFLAGS) $<

obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_lib_muParserCallback.obj :  .AUTODEPEND ..\src\muParserCallback.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MUPARSER_LIB_CXXFLAGS) $<

obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_lib_muParserBase.obj :  .AUTODEPEND ..\src\muParserBase.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MUPARSER_LIB_CXXFLAGS) $<

obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_dll_muParserBytecode.obj :  .AUTODEPEND ..\src\muParserBytecode.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MUPARSER_DLL_CXXFLAGS) $<

obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_dll_muParserTest.obj :  .AUTODEPEND ..\src\muParserTest.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MUPARSER_DLL_CXXFLAGS) $<

obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_dll_muParser.obj :  .AUTODEPEND ..\src\muParser.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MUPARSER_DLL_CXXFLAGS) $<

obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_dll_muParserDLL.obj :  .AUTODEPEND ..\src\muParserDLL.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MUPARSER_DLL_CXXFLAGS) $<

obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_dll_muParserInt.obj :  .AUTODEPEND ..\src\muParserInt.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MUPARSER_DLL_CXXFLAGS) $<

obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_dll_muParserTokenReader.obj :  .AUTODEPEND ..\src\muParserTokenReader.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MUPARSER_DLL_CXXFLAGS) $<

obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_dll_muParserError.obj :  .AUTODEPEND ..\src\muParserError.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MUPARSER_DLL_CXXFLAGS) $<

obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_dll_muParserCallback.obj :  .AUTODEPEND ..\src\muParserCallback.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MUPARSER_DLL_CXXFLAGS) $<

obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\muParser_dll_muParserBase.obj :  .AUTODEPEND ..\src\muParserBase.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MUPARSER_DLL_CXXFLAGS) $<

obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\example1_Example1.obj :  .AUTODEPEND ..\samples\example1\Example1.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(EXAMPLE1_CXXFLAGS) $<

obj\wat_$(SHAREDBUILDPOSTFIX)_$(DEBUGBUILDPOSTFIX)\example3_Example3.obj :  .AUTODEPEND ..\samples\example3\Example3.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(EXAMPLE3_CXXFLAGS) $<

