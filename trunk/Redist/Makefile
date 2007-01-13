CWD=$(shell cygpath -w $(shell pwd))

all : hhupd.exe.extract

hhupd.exe.extract : hhupd.exe
	mkdir -p hhupd.exe.extract
	./hhupd.exe /T:$(CWD)\hhupd.exe.extract /C