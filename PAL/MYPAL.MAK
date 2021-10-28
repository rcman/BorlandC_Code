#
# Borland C++ IDE generated makefile
#
.AUTODEPEND


#
# Borland C++ tools
#
IMPLIB  = Implib
BCCDOS  = Bcc +BccDos.cfg 
TLINK   = TLink
TLIB    = TLib
TASM    = Tasm
#
# IDE macros
#


#
# Options
#
IDE_LFLAGSDOS =  -L\bc45\lib
IDE_BFLAGS = 
LLATDOS_mypaldexe =  -LC:\BORLANDC\LIB;C:\BC\LIB -c -Tde
RLATDOS_mypaldexe = 
BLATDOS_mypaldexe = 
CNIEAT_mypaldexe = -IC:\BORLANDC\INCLUDE;C:\BC\INCLUDE -D
LNIEAT_mypaldexe = -x
LEAT_mypaldexe = $(LLATDOS_mypaldexe)
REAT_mypaldexe = $(RLATDOS_mypaldexe)
BEAT_mypaldexe = $(BLATDOS_mypaldexe)

#
# Dependency List
#
Dep_mypal = \
   mypal.exe

mypal : BccDos.cfg $(Dep_mypal)
  echo MakeNode 

Dep_mypaldexe = \
   text.obj\
   anim.obj\
   fade_c.obj

mypal.exe : $(Dep_mypaldexe)
  $(TLINK)   @&&|
 /v $(IDE_LFLAGSDOS) $(LEAT_mypaldexe) $(LNIEAT_mypaldexe) +
C:\BORLANDC\LIB\c0m.obj+fade.obj
text.obj+
anim.obj+
fade_c.obj
$<,$*
C:\BORLANDC\LIB\graphics.lib+
C:\BORLANDC\LIB\emu.lib+
C:\BORLANDC\LIB\mathm.lib+
C:\BORLANDC\LIB\cm.lib

|

text.obj :  text.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_mypaldexe) $(CNIEAT_mypaldexe) -o$@ text.c
|

anim.obj :  anim.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_mypaldexe) $(CNIEAT_mypaldexe) -o$@ anim.c
|

fade_c.obj :  fade_c.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_mypaldexe) $(CNIEAT_mypaldexe) -o$@ fade_c.c
|

# Compiler configuration file
BccDos.cfg :
	Copy &&|
-W-
-R
-v
-vi
-H
-H=mypal.csm
-R-
-3
-i32
-O
-Og
-Oe
-Om
-Ov
-Ol
-Ob
-Op
-Oi
-Z
-k-
-vi-
-H-
-mm
-f
| $@


