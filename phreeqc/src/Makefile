# 
# Make file for PHREEQC
#
PROGRAM=phreeqc
EXE=$(TOPDIR)/bin/$(PROGRAM)
#EXE=$(PROGRAM)

# Do not print commands before executing
#.SILENT:

# Provides compatibility with GNU make
#.SUFFIXES:

# Change to pawd if using automounter
PWD=pwd
#PWD=pawd

# Change to C compiler on your system
CC=gcc
#CC=/opt/SUNWspro/bin/cc   # Sun

# Change to C compiler options on your system
CCFLAGS=-O3 -Wall -ansi -pedantic # -pg
#CCFLAGS=-Wall -ansi -g 
#CCFLAGS=-fast             #Sun


LOADFLAGS= -lm # -pg

# Location to copy scripts on installation
BINDIR=$(HOME)/bin

# Prevent error before make is recursively invoked
DISTFILE=foo

# Locations to save compressed tar file for distribution
#DIST_DIR=/var/anonymous/ftp/geochem/unix/phreeqc
DIST_DIR=/home/dlpark/temp

# Save directories
TOPDIR=..
# list of files for distribution
FILES=  \
	README.TXT \
	NOTICE.TXT \
	RELEASE.TXT \
	src/Makefile \
	src/advection.c \
	src/basic.c \
	src/basicsubs.c \
	src/cl1.c \
	src/cvdense.c \
	src/cvode.c \
	src/dense.c \
	src/input.c \
	src/integrate.c \
	src/inverse.c \
	src/isotopes.c \
	src/kinetics.c \
	src/main.c \
	src/mainsubs.c \
	src/model.c \
	src/nvector.c \
	src/nvector_serial.c \
	src/output.c \
	src/p2clib.c \
	src/parse.c \
	src/phqalloc.c \
	src/phreeqc_files.c \
	src/prep.c \
	src/print.c \
	src/read.c \
	src/readtr.c \
	src/smalldense.c \
	src/spread.c \
	src/step.c \
	src/structures.c \
	src/sundialsmath.c \
	src/tally.c \
	src/tidy.c \
	src/transport.c \
	src/utilities.c \
	src/cvdense.h \
	src/cvode.h \
	src/dense.h \
	src/global.h \
	src/input.h \
	src/kinetics.h \
	src/nvector.h \
	src/nvector_serial.h \
	src/output.h \
	src/p2c.h \
	src/phqalloc.h \
	src/phrqproto.h \
	src/phrqtype.h \
	src/smalldense.h \
	src/sundialsmath.h \
	src/sundialstypes.h \
	database/llnl.dat \
	database/minteq.dat \
	database/phreeqc.dat \
	database/wateq4f.dat \
	database/iso.dat \
	examples/ex1 examples/ex1.out \
	examples/ex2 examples/ex2.out examples/ex2.sel \
	examples/ex3 examples/ex3.out \
	examples/ex4 examples/ex4.out \
	examples/ex5 examples/ex5.out examples/ex5.sel \
	examples/ex6 examples/ex6.out examples/ex6A-B.sel examples/ex6C.sel \
	examples/ex7 examples/ex7.out examples/ex7.sel \
	examples/ex8 examples/ex8.out examples/ex8.sel \
	examples/ex9 examples/ex9.out examples/ex9.sel \
	examples/ex10 examples/ex10.out examples/ex10.sel \
	examples/ex11 examples/ex11.out examples/ex11adv.sel examples/ex11trn.sel \
	examples/ex12 examples/ex12.out examples/ex12.sel \
	examples/ex13a examples/ex13a.out examples/ex13a.sel \
	examples/ex13b examples/ex13b.out examples/ex13b.sel \
	examples/ex13c examples/ex13c.out examples/ex13c.sel \
	examples/ex14 examples/ex14.out examples/ex14.sel \
	examples/ex15 examples/ex15.dat examples/ex15.out examples/ex15.sel \
	examples/ex16 examples/ex16.out \
	examples/ex17 examples/ex17.out \
	examples/ex18 examples/ex18.out \
	doc/manual.pdf \
	doc/wrir02-4172.pdf \
	doc/phreeqc.txt \
	bin/phreeqc.orig \
	test/test.sh \
	test/clean.sh \
	test/check.sh 

OBJECTS=	main.o \
		advection.o \
		basic.o \
		basicsubs.o \
		cl1.o \
		input.o \
		integrate.o \
		inverse.o \
		isotopes.o \
		kinetics.o \
		mainsubs.o \
		output.o \
		model.o \
		p2clib.o \
		parse.o \
		phreeqc_files.o \
		phqalloc.o \
		prep.o \
		print.o \
		read.o \
		readtr.o \
		spread.o \
		step.o \
		structures.o \
		tally.o \
		tidy.o \
		transport.o \
		utilities.o \
		cvdense.o \
		cvode.o \
		dense.o \
		nvector.o \
		nvector_serial.o \
		smalldense.o \
		sundialsmath.o 

OUTPUT_FILES = \
	ex1.out \
	ex2.out ex2.sel \
	ex3.out \
	ex4.out \
	ex5.out ex5.sel \
	ex6.out ex6A-B.sel ex6C.sel \
	ex7.out ex7.sel \
	ex8.out ex8.sel \
	ex9.out ex9.sel \
	ex10.out ex10.sel \
	ex11.out ex11adv.sel ex11trn.sel \
	ex12.out ex12.sel \
	ex13a.out ex13a.sel \
	ex13b.out ex13b.sel \
	ex13c.out ex13c.sel \
	ex14.out ex14.sel \
	ex15.out ex15.sel \
	ex16.out \
	ex17.out \
	ex18.out 

all: $(PROGRAM)

$(PROGRAM): $(TOPDIR)/bin/$(PROGRAM)

install: 
#
#    Create directory for binary and scripts if necessary
#
	if [ ! -d $(BINDIR) ]; \
           then \
              mkdir $(BINDIR); \
              echo Created directory $(BINDIR); \
        fi
#
#    Put path name of current directory into script for
#    locating data files, put script in top directory,
#    put symbolic link in BINDIR
#
	cd $(TOPDIR); dir1=`$(PWD)`/bin; cd $(BINDIR); if [ `$(PWD)` = $$dir1 ]; \
			then \
				echo "Can not install to $(BINDIR). Choose another directory."; \
				exit 4 ; \
			fi
	cd $(TOPDIR); \
		rm -f $(BINDIR)/$(PROGRAM); \
		rm -f $(PROGRAM); \
		sed "s?TOPDIR=.\{0,80\}?TOPDIR=`$(PWD)`?" bin/$(PROGRAM).orig > $(PROGRAM); \
		chmod 755 $(PROGRAM)
	cd $(TOPDIR); dir1=`$(PWD)`; cd $(BINDIR); if [ `$(PWD)` != $$dir1 ]; then \
			ln -s $$dir1/$(PROGRAM) $(BINDIR); \
			echo Symbolic link for $(PROGRAM) has been placed in $(BINDIR).	; \
		fi
#
#     Check that all necessary files are in place.
#
	if [ -f $(BINDIR)/$(PROGRAM) -a \
             -f $(TOPDIR)/bin/$(PROGRAM) -a \
             -f $(TOPDIR)/$(PROGRAM) ]; \
           then echo "Installation complete."; \
           else echo "Installation incomplete."; \
              for FILE in $(BINDIR)/$(PROGRAM) \
                 $(TOPDIR)/bin/$(PROGRAM) $(TOPDIR)/$(PROGRAM) ; \
              do \
                 if [ ! -f $$FILE ]; then echo $$FILE is missing.; fi; \
              done; \
           fi
	echo "Add directory $(BINDIR) to PATH."

diff: 
	cd $(TOPDIR); rcsdiff RELEASE.TXT # rm -f RELEASE.TXT; co -l RELEASE.TXT; cp src/revisions RELEASE.TXT
	cd $(TOPDIR); for FILE in $(FILES); do rcsdiff $$FILE ; done
	cd $(TOPDIR); for FILE in $(OUTPUT_FILES); do rcsdiff test/$$FILE; done
	cd $(TOPDIR); for FILE in $(OUTPUT_FILES); do rcsdiff test.sun/$$FILE; done
	cd $(TOPDIR); rcsdiff win/README.TXT win/check.bat win/phreeqc.bat win/test.bat win/clean.bat

dist: 
	error=0; for FILE in $(OUTPUT_FILES); do \
	   if [ ! -f ../test/$$FILE ]; then echo test/$$FILE is missing.; error=1; fi; \
	   if [ ! -f ../test.sun/$$FILE ]; then echo test.sun/$$FILE is missing.; error=1; fi; \
	   done; if [ $$error -eq 1 ]; then echo Stopping because of missing files; exit 4; fi;
	rm -f $(PROGRAM).rev
	cd $(TOPDIR); for FILE in $(FILES); do rlog -h $$FILE >> src/$(PROGRAM).rev; done
	ci -q -mUpdate $(PROGRAM).rev
	co -l $(PROGRAM).rev
	rm -f Makefile.internaldist; cp Makefile Makefile.internaldist
	make -C $(TOPDIR) -f src/Makefile.internaldist internaldist VERSION=`rlog $(PROGRAM).rev | grep head|tr -d '[a-z][A-Z] :'`
	rm -f Makefile.internaldist
	echo 
	echo Did you update the date in main.c???
	echo
	echo Did you make the Linux output files in test???
	echo
	echo Did you make the Sun output files in test.sun???
	echo
	echo Did you update the date in win/README.TXT???
	echo
	echo Did you update the date in RELEASE.TXT???
	echo
	echo Did you update History in phreeqc.txt???
	echo

internaldist: clean_dist dist_linux dist_sun dist_unix
	cp README.TXT $(DIST_DIR)
	cp $(DIST_DIR)/$(PROGRAM)$(VERSION).Linux.tar.gz ../ftp
	cp $(DIST_DIR)/$(PROGRAM)$(VERSION).SunOS.tar.gz ../ftp
	cp $(DIST_DIR)/$(PROGRAM)$(VERSION).source.tar.gz ../ftp

dist_linux: $(FILES)
	tar -cf $(PROGRAM).tar src/$(PROGRAM).rev 
	for FILE in $(FILES); do tar -rf $(PROGRAM).tar $$FILE; done
	tar -rf $(PROGRAM).tar bin/$(PROGRAM)
	rm -rf $(PROGRAM)$(VERSION)
	mkdir $(PROGRAM)$(VERSION)
	mv $(PROGRAM).tar $(PROGRAM)$(VERSION)
	cd $(PROGRAM)$(VERSION); tar -xf $(PROGRAM).tar; rm $(PROGRAM).tar
	for FILE in $(OUTPUT_FILES); do cp test/$$FILE $(PROGRAM)$(VERSION)/examples; done;
	tar -czf $(PROGRAM).Linux.tar.gz $(PROGRAM)$(VERSION)
	mv $(PROGRAM).Linux.tar.gz $(DIST_DIR)/$(PROGRAM)$(VERSION).Linux.tar.gz
	echo $(PROGRAM)$(VERSION).Linux.tar.gz saved in $(DIST_DIR).
#	rm -rf $(PROGRAM)$(VERSION)

dist_sun: $(FILES)
	tar -cf $(PROGRAM).tar src/$(PROGRAM).rev 
	for FILE in $(FILES); do tar -rf $(PROGRAM).tar $$FILE; done
	rm -rf $(PROGRAM)$(VERSION)
	mkdir $(PROGRAM)$(VERSION)
	mv $(PROGRAM).tar $(PROGRAM)$(VERSION)
	cd $(PROGRAM)$(VERSION); tar -xf $(PROGRAM).tar; rm $(PROGRAM).tar
	cp bin/$(PROGRAM).sun $(PROGRAM)$(VERSION)/bin/$(PROGRAM)
	rm -f $(PROGRAM)$(VERSION)/src/Makefile
	cp src/Makefile $(PROGRAM)$(VERSION)/src/Makefile
	for FILE in $(OUTPUT_FILES); do cp test.sun/$$FILE $(PROGRAM)$(VERSION)/examples; done;
	tar -czf $(PROGRAM).SunOS.tar.gz $(PROGRAM)$(VERSION)
	mv $(PROGRAM).SunOS.tar.gz $(DIST_DIR)/$(PROGRAM)$(VERSION).SunOS.tar.gz
	echo $(PROGRAM)$(VERSION).SunOS.tar.gz saved in $(DIST_DIR).
	rm -rf $(PROGRAM)$(VERSION)

dist_unix: $(FILES)
	tar -cf $(PROGRAM).tar src/$(PROGRAM).rev 
	for FILE in $(FILES); do tar -rf $(PROGRAM).tar $$FILE; done
	rm -rf $(PROGRAM)$(VERSION)
	mkdir $(PROGRAM)$(VERSION)
	mv $(PROGRAM).tar $(PROGRAM)$(VERSION)
	cd $(PROGRAM)$(VERSION); tar -xf $(PROGRAM).tar; rm $(PROGRAM).tar
	for FILE in $(OUTPUT_FILES); do cp test/$$FILE $(PROGRAM)$(VERSION)/examples; done;
	tar -czf $(PROGRAM).source.tar.gz $(PROGRAM)$(VERSION)
	mv $(PROGRAM).source.tar.gz $(DIST_DIR)/$(PROGRAM)$(VERSION).source.tar.gz
	echo $(PROGRAM)$(VERSION).source.tar.gz saved in $(DIST_DIR).
	rm -rf $(PROGRAM)$(VERSION)

clean:
	rm -f $(BINDIR)/$(PROGRAM)
	rm -f $(TOPDIR)/$(PROGRAM)
	rm -f $(TOPDIR)/bin/$(PROGRAM) *.o 
	echo Removed files generated by make.

clean_dist:
	rm -f $(DIST_DIR)/README.TXT
	rm -f $(DIST_DIR)/$(PROGRAM)*gz
	echo Removed README.TXT and tar.gz files for $(PROGRAM) from $(DIST_DIR).

$(TOPDIR)/bin/$(PROGRAM): $(OBJECTS) 
	$(CC) -o $(EXE) $(OBJECTS) $(LOADFLAGS) # -L/z/estespark/home/dlpark/packages/efence -lefence
	echo Compilation complete, $(EXE).

#$(PROGRAM): $(OBJECTS) 
#	$(CC) -o $(PROGRAM) $(OBJECTS) $(LOADFLAGS)  -lefence
#	echo Compilation complete, ./$(PROGRAM).

cvdense.h: cvode.h sundialstypes.h dense.h nvector.h
	touch cvdense.h

cvode.h: sundialstypes.h nvector.h 
	touch cvode.h

dense.h: smalldense.h
	touch dense.h

global.h: phrqtype.h
	touch global.h

nvector.h: sundialstypes.h
	touch nvector.h

nvector_serial.h: sundialstypes.h
	touch nvector_serial.h

sundialstypes.h: phrqtype.h
	touch sundialstypes.h

advection.o: advection.c global.h phqalloc.h output.h phrqproto.h
	$(CC) $(CCFLAGS) advection.c -c

basic.o: basic.c global.h phqalloc.h output.h phrqproto.h p2c.h
	$(CC) $(CCFLAGS) basic.c -c

basicsubs.o: basicsubs.c global.h phqalloc.h output.h phrqproto.h
	$(CC) $(CCFLAGS) basicsubs.c -c

cl1.o: cl1.c phqalloc.h output.h phrqtype.h
	$(CC) $(CCFLAGS) cl1.c -c

cvdense.o: cvdense.c cvdense.h cvode.h dense.h sundialstypes.h nvector.h sundialsmath.h output.h
	$(CC) $(CCFLAGS) cvdense.c -c

cvode.o: cvode.c cvode.h sundialstypes.h nvector.h sundialsmath.h output.h kinetics.h
	$(CC) $(CCFLAGS) cvode.c -c

dense.o: dense.c sundialstypes.h sundialsmath.h dense.h smalldense.h output.h
	$(CC) $(CCFLAGS) dense.c -c

input.o: input.c global.h input.h output.h phrqproto.h phqalloc.h 
	$(CC) $(CCFLAGS) input.c -c

integrate.o: integrate.c global.h phqalloc.h output.h phrqproto.h 
	$(CC) $(CCFLAGS) integrate.c -c

inverse.o: inverse.c global.h phqalloc.h output.h phrqproto.h 
	$(CC) $(CCFLAGS) inverse.c -c

isotopes.o: isotopes.c global.h phqalloc.h output.h phrqproto.h
	$(CC) $(CCFLAGS) isotopes.c -c

kinetics.o: kinetics.c global.h phqalloc.h output.h phrqproto.h sundialstypes.h cvode.h cvdense.h nvector_serial.h dense.h kinetics.h
	$(CC) $(CCFLAGS) kinetics.c -c

main.o: main.c global.h phqalloc.h output.h phrqproto.h input.h 
	$(CC) $(CCFLAGS) main.c -c

mainsubs.o: mainsubs.c global.h phqalloc.h output.h phrqproto.h input.h
	$(CC) $(CCFLAGS) mainsubs.c -c

model.o: model.c global.h phqalloc.h output.h phrqproto.h 
	$(CC) $(CCFLAGS) model.c -c

nvector.o: nvector.c nvector.h output.h 
	$(CC) $(CCFLAGS) nvector.c -c

nvector_serial.o: nvector_serial.c nvector_serial.h  sundialstypes.h sundialsmath.h output.h
	$(CC) $(CCFLAGS) nvector_serial.c -c

output.o: output.c global.h output.h phrqproto.h phqalloc.h 
	$(CC) $(CCFLAGS) output.c -c

p2clib.o: p2clib.c p2c.h output.h
	$(CC) $(CCFLAGS) p2clib.c -c

parse.o: parse.c global.h phqalloc.h output.h phrqproto.h
	$(CC) $(CCFLAGS) parse.c -c

phreeqc_files.o: phreeqc_files.c global.h phqalloc.h output.h phrqproto.h input.h
	$(CC) $(CCFLAGS) phreeqc_files.c -c

phqalloc.o: phqalloc.c global.h output.h
	$(CC) $(CCFLAGS) phqalloc.c -c

prep.o: prep.c global.h phqalloc.h output.h phrqproto.h
	$(CC) $(CCFLAGS) prep.c -c

print.o: print.c global.h phqalloc.h output.h phrqproto.h
	$(CC) $(CCFLAGS) print.c -c

read.o: read.c global.h phqalloc.h output.h phrqproto.h
	$(CC) $(CCFLAGS) read.c -c

readtr.o: readtr.c global.h phqalloc.h output.h phrqproto.h
	$(CC) $(CCFLAGS) readtr.c -c

smalldense.o: smalldense.c smalldense.h sundialstypes.h sundialsmath.h output.h
	$(CC) $(CCFLAGS) smalldense.c -c

spread.o: spread.c global.h phqalloc.h output.h phrqproto.h
	$(CC) $(CCFLAGS) spread.c -c

step.o: step.c global.h phqalloc.h output.h phrqproto.h
	$(CC) $(CCFLAGS) step.c -c

structures.o: structures.c global.h phqalloc.h output.h phrqproto.h 
	$(CC) $(CCFLAGS) structures.c -c

sundialsmath.o: sundialsmath.c sundialsmath.h sundialstypes.h output.h
	$(CC) $(CCFLAGS) sundialsmath.c -c

tally.o: tally.c global.h phqalloc.h output.h phrqproto.h
	$(CC) $(CCFLAGS) tally.c -c

tidy.o: tidy.c global.h phqalloc.h output.h phrqproto.h
	$(CC) $(CCFLAGS) tidy.c -c

transport.o: transport.c global.h phqalloc.h output.h phrqproto.h
	$(CC) $(CCFLAGS) transport.c -c

utilities.o: utilities.c global.h phqalloc.h output.h phrqproto.h
	$(CC) $(CCFLAGS) utilities.c -c

win: 
	cd ..; tar -cf $(PROGRAM).tar src/$(PROGRAM).rev 
	cd ..; for FILE in $(FILES); do tar -rf $(PROGRAM).tar $$FILE; done
	cd ..; rm -rf $(PROGRAM).win
	cd ..; mkdir $(PROGRAM).win
	cd ..; mv $(PROGRAM).tar $(PROGRAM).win
	cd ../$(PROGRAM).win; tar -xf $(PROGRAM).tar; rm $(PROGRAM).tar
	# remove example output
	cd ..; rm -f $(PROGRAM).win/examples/*.out $(PROGRAM).win/examples/*.sel
	# remove database, bin directories
	cd ..; rm -rf $(PROGRAM).win/database $(PROGRAM).win/bin
	# copy Windows files
	cd ..; cp database/phreeqc.dat database/llnl.dat database/wateq4f.dat database/minteq.dat database/iso.dat $(PROGRAM).win/
	cd ..; cp win/phreeqc.bat $(PROGRAM).win/
	cd ..; rm -f $(PROGRAM).win/test/*
	cd ..; cp win/clean.bat win/check.bat win/test.bat $(PROGRAM).win/test
	cd ..; rm -f $(PROGRAM).win/README.TXT
	cd ..; cp win/README.TXT $(PROGRAM).win/
	cd ..; rm -f $(PROGRAM).Windows.tar.gz
	cd ..; tar -czf $(PROGRAM).Windows.tar.gz $(PROGRAM).win
	cd ..; echo $(PROGRAM).Windows.tar.gz created.
	cd ..; rm -rf $(PROGRAM).win


clean_dos:
	rm -f $(TOPDIR)/msdos/src/*
	rm -f $(TOPDIR)/msdos/examples/*
	rm -f $(TOPDIR)/msdos/doc/*
	rm -f $(TOPDIR)/msdos/database/*
	rm -f $(TOPDIR)/msdos/*.dat
	rm -f $(TOPDIR)/msdos/uu
	rm -f $(TOPDIR)/msdos/phreeqc.zip
	echo "$(TOPDIR)/msdos cleaned."

dos: clean_dos	
	/home/dlpark/bin/textcp dos Makefile 		$(TOPDIR)/msdos/Makefile
	/home/dlpark/bin/textcp dos advection.c 	$(TOPDIR)/msdos/advectio.c
	/home/dlpark/bin/textcp dos basic.c 		$(TOPDIR)/msdos/basic.c
	/home/dlpark/bin/textcp dos basicsubs.c		$(TOPDIR)/msdos/basicsubs.c
	/home/dlpark/bin/textcp dos cl1.c 		$(TOPDIR)/msdos/cl1.c
	/home/dlpark/bin/textcp dos cvdense.c 		$(TOPDIR)/msdos/cvdense.c
	/home/dlpark/bin/textcp dos cvode.c 		$(TOPDIR)/msdos/cvode.c
	/home/dlpark/bin/textcp dos dense.c 		$(TOPDIR)/msdos/dense.c
	/home/dlpark/bin/textcp dos input.c	 	$(TOPDIR)/msdos/input.c
	/home/dlpark/bin/textcp dos integrate.c 	$(TOPDIR)/msdos/integrat.c
	/home/dlpark/bin/textcp dos inverse.c 		$(TOPDIR)/msdos/inverse.c
	/home/dlpark/bin/textcp dos isotopes.c 		$(TOPDIR)/msdos/isotopes.c
	/home/dlpark/bin/textcp dos kinetics.c 		$(TOPDIR)/msdos/kinetics.c
	/home/dlpark/bin/textcp dos main.c 		$(TOPDIR)/msdos/main.c
	/home/dlpark/bin/textcp dos mainsubs.c 		$(TOPDIR)/msdos/mainsubs.c
	/home/dlpark/bin/textcp dos model.c 		$(TOPDIR)/msdos/model.c
	/home/dlpark/bin/textcp dos nvector.c 		$(TOPDIR)/msdos/nvector.c
	/home/dlpark/bin/textcp dos nvector_serial.c 	$(TOPDIR)/msdos/nvector_serial.c
	/home/dlpark/bin/textcp dos output.c 		$(TOPDIR)/msdos/output.c
	/home/dlpark/bin/textcp dos p2clib.c 		$(TOPDIR)/msdos/p2clib.c
	/home/dlpark/bin/textcp dos parse.c 		$(TOPDIR)/msdos/parse.c
	/home/dlpark/bin/textcp dos phqalloc.c 		$(TOPDIR)/msdos/phqalloc.c
	/home/dlpark/bin/textcp dos phreeqc_files.c 	$(TOPDIR)/msdos/phreeqc_files.c
	/home/dlpark/bin/textcp dos prep.c 		$(TOPDIR)/msdos/prep.c
	/home/dlpark/bin/textcp dos print.c 		$(TOPDIR)/msdos/print.c
	/home/dlpark/bin/textcp dos read.c 		$(TOPDIR)/msdos/read.c
	/home/dlpark/bin/textcp dos readtr.c 		$(TOPDIR)/msdos/readtr.c
	/home/dlpark/bin/textcp dos smalldense.c 	$(TOPDIR)/msdos/smalldense.c
	/home/dlpark/bin/textcp dos spread.c 		$(TOPDIR)/msdos/spread.c
	/home/dlpark/bin/textcp dos step.c 		$(TOPDIR)/msdos/step.c
	/home/dlpark/bin/textcp dos structures.c 	$(TOPDIR)/msdos/struct.c
	/home/dlpark/bin/textcp dos sundialsmath.c 	$(TOPDIR)/msdos/sundialsmath.c
	/home/dlpark/bin/textcp dos tally.c 		$(TOPDIR)/msdos/tally.c
	/home/dlpark/bin/textcp dos tidy.c 		$(TOPDIR)/msdos/tidy.c
	/home/dlpark/bin/textcp dos transport.c 	$(TOPDIR)/msdos/trans.c
	/home/dlpark/bin/textcp dos utilities.c 	$(TOPDIR)/msdos/util.c
	/home/dlpark/bin/textcp dos cvdense.h		$(TOPDIR)/msdos/cvdense.h
	/home/dlpark/bin/textcp dos cvode.h		$(TOPDIR)/msdos/cvode.h
	/home/dlpark/bin/textcp dos dense.h		$(TOPDIR)/msdos/dense.h
	/home/dlpark/bin/textcp dos global.h 		$(TOPDIR)/msdos/global.h
	/home/dlpark/bin/textcp dos hst.h		$(TOPDIR)/msdos/hst.h
	/home/dlpark/bin/textcp dos input.h		$(TOPDIR)/msdos/input.h
	/home/dlpark/bin/textcp dos kinetics.h 		$(TOPDIR)/msdos/kinetics.h
	/home/dlpark/bin/textcp dos nvector.h		$(TOPDIR)/msdos/nvector.h
	/home/dlpark/bin/textcp dos nvector_serial.h	$(TOPDIR)/msdos/nvector_serial.h
	/home/dlpark/bin/textcp dos output.h 		$(TOPDIR)/msdos/output.h
	/home/dlpark/bin/textcp dos p2c.h 		$(TOPDIR)/msdos/p2c.h
	/home/dlpark/bin/textcp dos phast_files.h	$(TOPDIR)/msdos/phast_files.h
	/home/dlpark/bin/textcp dos phqalloc.h 		$(TOPDIR)/msdos/phqalloc.h
	/home/dlpark/bin/textcp dos phrqproto.h		$(TOPDIR)/msdos/phrqproto.h
	/home/dlpark/bin/textcp dos phrqtype.h		$(TOPDIR)/msdos/phrqtype.h
	/home/dlpark/bin/textcp dos smalldense.h	$(TOPDIR)/msdos/smalldense.h
	/home/dlpark/bin/textcp dos sundialsmath.h	$(TOPDIR)/msdos/sundialsmath.h
	/home/dlpark/bin/textcp dos sundialstypes.h	$(TOPDIR)/msdos/sundialstypes.h
	/home/dlpark/bin/textcp dos $(TOPDIR)/database/llnl.dat			$(TOPDIR)/msdos/llnl.dat
	/home/dlpark/bin/textcp dos $(TOPDIR)/database/iso.dat			$(TOPDIR)/msdos/iso.dat
	/home/dlpark/bin/textcp dos $(TOPDIR)/database/minteq.dat		$(TOPDIR)/msdos/wateq4f.dat
	/home/dlpark/bin/textcp dos $(TOPDIR)/database/phreeqc.dat		$(TOPDIR)/msdos/phreeqc.dat
	/home/dlpark/bin/textcp dos $(TOPDIR)/database/wateq4f.dat		$(TOPDIR)/msdos/wateq4f.dat
	/home/dlpark/bin/textcp dos $(TOPDIR)/examples/ex1 	$(TOPDIR)/msdos/examples/ex1
	/home/dlpark/bin/textcp dos $(TOPDIR)/examples/ex2 	$(TOPDIR)/msdos/examples/ex2
	/home/dlpark/bin/textcp dos $(TOPDIR)/examples/ex3	$(TOPDIR)/msdos/examples/ex3
	/home/dlpark/bin/textcp dos $(TOPDIR)/examples/ex4	$(TOPDIR)/msdos/examples/ex4
	/home/dlpark/bin/textcp dos $(TOPDIR)/examples/ex5	$(TOPDIR)/msdos/examples/ex5
	/home/dlpark/bin/textcp dos $(TOPDIR)/examples/ex6	$(TOPDIR)/msdos/examples/ex6
	/home/dlpark/bin/textcp dos $(TOPDIR)/examples/ex7	$(TOPDIR)/msdos/examples/ex7
	/home/dlpark/bin/textcp dos $(TOPDIR)/examples/ex8	$(TOPDIR)/msdos/examples/ex8
	/home/dlpark/bin/textcp dos $(TOPDIR)/examples/ex9	$(TOPDIR)/msdos/examples/ex9
	/home/dlpark/bin/textcp dos $(TOPDIR)/examples/ex10	$(TOPDIR)/msdos/examples/ex10
	/home/dlpark/bin/textcp dos $(TOPDIR)/examples/ex11	$(TOPDIR)/msdos/examples/ex11
	/home/dlpark/bin/textcp dos $(TOPDIR)/examples/ex12	$(TOPDIR)/msdos/examples/ex12
	/home/dlpark/bin/textcp dos $(TOPDIR)/examples/ex13a	$(TOPDIR)/msdos/examples/ex13a
	/home/dlpark/bin/textcp dos $(TOPDIR)/examples/ex13b	$(TOPDIR)/msdos/examples/ex13b
	/home/dlpark/bin/textcp dos $(TOPDIR)/examples/ex13c	$(TOPDIR)/msdos/examples/ex13c
	/home/dlpark/bin/textcp dos $(TOPDIR)/examples/ex14	$(TOPDIR)/msdos/examples/ex14
	/home/dlpark/bin/textcp dos $(TOPDIR)/examples/ex15	$(TOPDIR)/msdos/examples/ex15
	/home/dlpark/bin/textcp dos $(TOPDIR)/examples/ex15.dat	$(TOPDIR)/msdos/examples/ex15.dat
	/home/dlpark/bin/textcp dos $(TOPDIR)/examples/ex16	$(TOPDIR)/msdos/examples/ex16
	/home/dlpark/bin/textcp dos $(TOPDIR)/examples/ex17	$(TOPDIR)/msdos/examples/ex17
	/home/dlpark/bin/textcp dos $(TOPDIR)/examples/ex18	$(TOPDIR)/msdos/examples/ex18
	cd $(TOPDIR)/msdos; sed "s?NO_DOS?DOS?" global.h > temp; mv temp global.h
	cd $(TOPDIR)/msdos; zip -r phreeqc.zip *
	cd $(TOPDIR)/msdos; uuencode phreeqc.zip phreeqc.zip > uu

co:
	co main.c
	co advection.c
	co basic.c
	co basicsubs.c
	co cl1.c
	co integrate.c
	co inverse.c
	co isotopes.c
	co kinetics.c
	co mainsubs.c
	co model.c
	co output.c
	co p2clib.c
	co parse.c
	co phqalloc.c
	co prep.c
	co print.c
	co read.c
	co readtr.c
	co spread.c
	co step.c
	co structures.c
	co tally.c
	co tidy.c
	co transport.c
	co utilities.c
	co global.h
	co kinetics.h
	co output.h
	co p2c.h

sun: clean_sun $(TOPDIR)/bin/$(PROGRAM).sun

clean_sun:
	rm -f $(TOPDIR)/sun/*.c $(TOPDIR)/sun/*.h $(TOPDIR)/sun/*.o $(TOPDIR)/bin/phreeqc.sun

$(TOPDIR)/bin/$(PROGRAM).sun: $(OBJECTS)
	cp Makefile *.c *.h $(TOPDIR)/sun
	rsh u450rcolkr "cd ~dlpark/programs/unix/phreeqc/sun; make -j 4 EXE=$(TOPDIR)/bin/$(PROGRAM).sun"
 
