CWD=$(shell cygpath -w $(shell pwd))

all : hhupd.exe.extract/hh.exe

hhupd.exe.extract/hh.exe : hhupd.exe
	mkdir -p hhupd.exe.extract
	./hhupd.exe /T:"$(CWD)\hhupd.exe.extract" /C

clean :
	rm -rf hhupd.exe.extract
