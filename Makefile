OBJ_LIST = hash.obj
CFLAGS = /nologo /W4 /EHsc /Za /DWIN /D_CRT_SECURE_NO_DEPRECATE

DLLNAMES = hash.dll

build: tema1.exe

tema1.exe: tema1.obj hash.lib
	cl tema1.obj hash.lib
	
tema1.obj:	tema1.c
	cl $(CFLAGS) /c tema1.c

clean : 
	del *.obj
	del tema1.exe
