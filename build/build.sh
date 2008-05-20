#!/bin/sh
#
# PHREEQCI package build script
#
# $Id$
#
# Package maintainers: if the original source is not distributed as a
# (possibly compressed) tarball, set the value of ${src_orig_pkg_name},
# and redefine the unpack() helper function appropriately.
# Also, if the Makefile rule to run the test suite is not "test", change
# the definition of ${test_rule} below.

# echo everything
set -x

# find out where the build script is located
tdir=`echo "$0" | sed 's%[\\/][^\\/][^\\/]*$%%'`
test "x$tdir" = "x$0" && tdir=.
scriptdir=`cd $tdir; pwd`
# find src directory.
# If scriptdir ends in SPECS, then topdir is $scriptdir/..
# If scriptdir ends in CYGWIN-PATCHES, then topdir is $scriptdir/../..
# Otherwise, we assume that topdir = scriptdir
topdir1=`echo ${scriptdir} | sed 's%/SPECS$%%'`
topdir2=`echo ${scriptdir} | sed 's%/CYGWIN-PATCHES$%%'`
if [ "x$topdir1" != "x$scriptdir" ] ; then # SPECS
  topdir=`cd ${scriptdir}/..; pwd`
else
  if [ "x$topdir2" != "x$scriptdir" ] ; then # CYGWIN-PATCHES
    topdir=`cd ${scriptdir}/../..; pwd`
  else
    topdir=`cd ${scriptdir}; pwd`
  fi
fi

tscriptname=`basename $0 .sh`
export PKG=`echo $tscriptname | sed -e 's/\-[^\-]*\-[^\-]*$//'`
export VER=`echo $tscriptname | sed -e "s/${PKG}\-//" -e 's/\-[^\-]*$//'`
export REL=`echo $tscriptname | sed -e "s/${PKG}\-${VER}\-//"`
export BASEPKG=${PKG}-${VER}-${REL}
export FULLPKG=${BASEPKG}
export DIFF_IGNORE="-x *.aps -x *.ncb -x *.opt -x *.dep -x *.mak -x *.chm"
export TOUCH_STAMP=`date -d ${DATE} "+%Y%m%d0000"`

# determine correct decompression option and tarball filename
export src_orig_pkg_name=
if [ -e "${src_orig_pkg_name}" ] ; then
  export opt_decomp=? # Make sure tar punts if unpack() is not redefined
###elif [ -e ${BASEPKG}.tar.bz2 ] ; then
###  export opt_decomp=j
###  export src_orig_pkg_name=${BASEPKG}.tar.bz2
elif [ -e ${BASEPKG}.tar.gz ] ; then
  export opt_decomp=z
  export src_orig_pkg_name=${BASEPKG}.tar.gz
elif [ -e ${BASEPKG}.tgz ] ; then
  export opt_decomp=z
  export src_orig_pkg_name=${BASEPKG}.tgz
elif [ -e ${BASEPKG}.tar ] ; then
  export opt_decomp=
  export src_orig_pkg_name=${BASEPKG}.tar
else
  echo Cannot find original package.
  exit 1
fi

export src_orig_pkg=${topdir}/${src_orig_pkg_name}

# determine correct names for generated files
export src_pkg_name=${FULLPKG}-src.tar.bz2
export src_patch_name=${FULLPKG}.patch
export bin_pkg_name=${FULLPKG}.tar.bz2

export src_pkg=${topdir}/${src_pkg_name}
export src_patch=${topdir}/${src_patch_name}
export bin_pkg=${topdir}/${bin_pkg_name}
export srcdir=${topdir}/${BASEPKG}
export objdir=${srcdir}/.build
export instdir=${srcdir}/.inst
export srcinstdir=${srcdir}/.sinst
export checkfile=${topdir}/${FULLPKG}.check

##{{
# InstallShield settings (based on exported build file
# IS_COMPILER=`locate Compile.exe | grep InstallShield`
# IS_BUILDER="`locate ISBuild.exe | grep InstallShield`"
IS_COMPILER="/cygdrive/c/Program Files/Common Files/InstallShield/IScript/Compile.exe"
IS_BUILDER="/cygdrive/c/Program Files/InstallShield/Professional - Standard Edition/Program/ISBuild.exe"

IS_INSTALLPROJECT=`cygpath -w "${objdir}/setup/setup.ipr"`
IS_CURRENTBUILD=PackageForTheWeb

IS_HOME=`echo "${IS_BUILDER}" | sed -e 's^/Program/ISBuild.exe$^^'`
IS_HOME=`cygpath -w "${IS_HOME}"`

IS_INCLUDEIFX=${IS_HOME}\\Script\\IFX\\Include
IS_INCLUDEISRT=${IS_HOME}\\Script\\ISRT\\Include
IS_INCLUDESCRIPT=`cygpath -w "${objdir}/setup/Script Files"`
IS_LINKPATH1="-LibPath${IS_HOME}\\Script\\IFX\\Lib"
IS_LINKPATH2="-LibPath${IS_HOME}\\Script\\ISRT\\Lib"
IS_RULFILES=`cygpath -w "${objdir}/setup/Script Files/Setup.rul"`
IS_LIBRARIES="isrt.obl ifx.obl"
IS_DEFINITIONS=""
IS_SWITCHES="-w50 -e50 -v3 -g"
##}}


prefix=/usr
sysconfdir=/etc
localstatedir=/var
if [ -z "$MY_CFLAGS" ]; then
  MY_CFLAGS="-O2"
fi
if [ -z "$MY_LDFLAGS" ]; then
  MY_LDFLAGS=
fi

export install_docs="\
	ABOUT-NLS \
	ANNOUNCE \
	AUTHORS \
	BUG-REPORTS \
	CHANGES \
	CONTRIBUTORS \
	COPYING \
	COPYRIGHT \
	CREDITS \
	CHANGELOG \
	ChangeLog* \
	FAQ \
	HOW-TO-CONTRIBUTE \
	INSTALL \
	KNOWNBUG \
	LEGAL \
	LICENSE \
	NEWS \
	NOTES \
	PROGLIST \
	README \
	RELEASE_NOTES \
	THANKS \
	TODO \
"
export install_docs="`for i in ${install_docs}; do echo $i; done | sort -u`"
export test_rule=test
if [ -z "$SIG" ]; then
  export SIG=0	# set to 1 to turn on signing by default
fi

# helper function
# unpacks the original package source archive into ./${BASEPKG}/
# change this if the original package was not tarred
# or if it doesn't unpack to a correct directory
unpack() {
  tar xv${opt_decomp}f "$1"
}

mkdirs() {
  (cd ${topdir} && \
  rm -fr ${objdir} ${instdir} ${srcinstdir} && \
  mkdir -p ${objdir} && \
  mkdir -p ${instdir} && \
  mkdir -p ${srcinstdir} )
}
prep() {
  (cd ${topdir} && \
  unpack ${src_orig_pkg} && \
  cd ${topdir} && \
  if [ -f ${src_patch} ] ; then \
    patch -p0 --binary < ${src_patch} ;\
  fi && \
  mkdirs )
}
conf() {
  (cd ${objdir} && \
  CFLAGS="${MY_CFLAGS}" LDFLAGS="${MY_LDFLAGS}" \
# copy links to ${objdir} for building
  find ${srcdir} -mindepth 1 -maxdepth 1 ! -name .build ! -name .inst ! -name .sinst -exec cp -al {} . \; )
}
reconf() {
  (cd ${topdir} && \
  rm -fr ${objdir} && \
  mkdir -p ${objdir} && \
  conf )
}
build() {
  (cd ${objdir} && \
  msdev `cygpath -w ./phreeqci2.dsw` /MAKE "phreeqci2 - Win32 Release" && \
  touch -t "${TOUCH_STAMP}" "${objdir}/phreeqc/database/"* && \
  touch -t "${TOUCH_STAMP}" "${objdir}/fs-031-02/"*.pdf && \
  touch -t "${TOUCH_STAMP}" "${objdir}/phreeqc/doc/"*.pdf && \
  touch -t "${TOUCH_STAMP}" "${objdir}/fs-031-02/"*.chm && \
  touch -t "${TOUCH_STAMP}" "${objdir}/Help/"*.chm && \
  touch -t "${TOUCH_STAMP}" "${objdir}/examples/"*.pqi && \
  touch -t "${TOUCH_STAMP}" "${objdir}/examples/"*.dat && \
  touch -t "${TOUCH_STAMP}" "${objdir}/phreeqci.eng" && \
  touch -t "${TOUCH_STAMP}" "${objdir}/Release/"*.exe && \
  touch -t "${TOUCH_STAMP}" "${objdir}/SRCDBPG/Release/"*.ocx )
}
check() {
  (cd ${objdir} && \
  make ${test_rule} | tee ${checkfile} 2>&1 )
}
clean() {
  (cd ${objdir} && \
  make clean )
}
install() {
  (rm -fr ${instdir}/* && \
  /usr/bin/install -m 644 ${srcdir}/phreeqc/src/revisions \
    ${srcdir}/phreeqc/src/REVISIONS.txt && \
  /usr/bin/unix2dos ${srcdir}/phreeqc/src/REVISIONS.txt && \  
# InstallShield compile
  "${IS_COMPILER}" "${IS_RULFILES}" -I"${IS_INCLUDEIFX}" -I"${IS_INCLUDEISRT}" \
    -I"${IS_INCLUDESCRIPT}" "${IS_LINKPATH1}" "${IS_LINKPATH2}" ${IS_LIBRARIES} \
    ${IS_DEFINITIONS} ${IS_SWITCHES} && \
# InstallShield build
  "${IS_BUILDER}" -p"${IS_INSTALLPROJECT}" -m"${IS_CURRENTBUILD}" && \
# VisualStudio Logs
  /usr/bin/install -m 644 "${objdir}/phreeqci2.plg" \
  ${instdir}/. && \
# InstallShield Logs  
  /usr/bin/install -m 644 "${objdir}/setup/Media/PackageForTheWeb/Log Files/"* \
  ${instdir}/. && \
  /usr/bin/install -m 644 "${objdir}/setup/Media/PackageForTheWeb/Report Files/"* \
  ${instdir}/. && \
# Setup application
  /usr/bin/install -m 755 "${objdir}/setup/Media/PackageForTheWeb/Disk Images/Disk1/setup.exe" \
    ${instdir}/${FULLPKG}.exe && \
  if [ -x /usr/bin/md5sum ]; then \
    cd ${instdir} && \
    find . -type f ! -name md5sum | sed 's/^/\"/' | sed 's/$/\"/' | xargs md5sum > md5sum ; \
  fi )
}
strip() {
  (cd ${instdir} && \
  echo 'SKIPPING find . -name "*.dll" -or -name "*.exe" | xargs strip 2>&1' ; \
  true )
}
list() {
  (cd ${instdir} && \
  find . -name "*" ! -type d | sed 's%^\.%  %' ; \
  true )
}
depend() {
  (cd ${instdir} && \
  find ${instdir} -name "*.exe" -o -name "*.dll" | xargs cygcheck | \
  sed -e '/\.exe/d' -e 's,\\,/,g' | sort -bu | xargs -n1 cygpath -u \
  | xargs cygcheck -f | sed 's%^%  %' | sort -u ; \
  true )
}
pkg() {
  (cd ${instdir} && \
  tar cvjf ${bin_pkg} * )
}
mkpatch() {
  (cd ${srcdir} && \
  find . -name "autom4te.cache" | xargs rm -rf ; \
  unpack ${src_orig_pkg} && \
  cd ${srcdir} && \
  mv ${BASEPKG} ../${BASEPKG}-orig && \
  cd ${topdir} && \
  diff -urN -x '.build' -x '.inst' -x '.sinst' \
    ${DIFF_IGNORE} \
    ${BASEPKG}-orig ${BASEPKG} > \
    ${srcinstdir}/${src_patch_name} ; \
  rm -rf ${BASEPKG}-orig )
}
# Note: maintainer-only functionality
acceptpatch() {
  cp --backup=numbered ${srcinstdir}/${src_patch_name} ${topdir}
}
spkg() {
  (mkpatch && \
  if [ "${SIG}" -eq 1 ] ; then \
    name=${srcinstdir}/${src_patch_name} text="PATCH" sigfile ; \
  fi && \
  cp ${src_orig_pkg} ${srcinstdir}/${src_orig_pkg_name} && \
  if [ -e ${src_orig_pkg}.sig ] ; then \
    cp ${src_orig_pkg}.sig ${srcinstdir}/ ; \
  fi && \
  cp $0 ${srcinstdir}/`basename $0` && \
  cp Makefile ${srcinstdir}/. && \
  name=$0 text="SCRIPT" sigfile && \
  if [ "${SIG}" -eq 1 ] ; then \
    cp $0.sig ${srcinstdir}/ ; \
  fi && \
  cd ${srcinstdir} && \
  tar cvjf ${src_pkg} * )
}
finish() {
  rm -rf ${srcdir}
}
sigfile() {
  if [ \( "${SIG}" -eq 1 \) -a \( -e $name \) -a \( \( ! -e $name.sig \) -o \( $name -nt $name.sig \) \) ]; then \
    if [ -x /usr/bin/gpg ]; then \
      echo "$text signature need to be updated"; \
      rm -f $name.sig; \
      /usr/bin/gpg --detach-sign $name; \
    else \
      echo "You need the gnupg package installed in order to make signatures."; \
    fi; \
  fi
}
checksig() {
  if [ -x /usr/bin/gpg ]; then \
    if [ -e ${src_orig_pkg}.sig ]; then \
      echo "ORIGINAL PACKAGE signature follows:"; \
      /usr/bin/gpg --verify ${src_orig_pkg}.sig ${src_orig_pkg}; \
    else \
      echo "ORIGINAL PACKAGE signature missing."; \
    fi; \
    if [ -e $0.sig ]; then \
      echo "SCRIPT signature follows:"; \
      /usr/bin/gpg --verify $0.sig $0; \
    else \
      echo "SCRIPT signature missing."; \
    fi; \
    if [ -e ${src_patch}.sig ]; then \
      echo "PATCH signature follows:"; \
      /usr/bin/gpg --verify ${src_patch}.sig ${src_patch}; \
    else \
      echo "PATCH signature missing."; \
    fi; \
  else
    echo "You need the gnupg package installed in order to check signatures." ; \
  fi
}
while test -n "$1" ; do
  case $1 in
    prep)		prep ; STATUS=$? ;;
    mkdirs)		mkdirs ; STATUS=$? ;;
    conf)		conf ; STATUS=$? ;;
    configure)		conf ; STATUS=$? ;;
    reconf)		reconf ; STATUS=$? ;;
    build)		build ; STATUS=$? ;;
    make)		build ; STATUS=$? ;;
    check)		check ; STATUS=$? ;;
    test)		check ; STATUS=$? ;;
    clean)		clean ; STATUS=$? ;;
    install)		install ; STATUS=$? ;;
    list)		list ; STATUS=$? ;;
    depend)		depend ; STATUS=$? ;;
    strip)		strip ; STATUS=$? ;;
    package)		pkg ; STATUS=$? ;;
    pkg)		pkg ; STATUS=$? ;;
    mkpatch)		mkpatch ; STATUS=$? ;;
    acceptpatch)	acceptpatch ; STATUS=$? ;;
    src-package)	spkg ; STATUS=$? ;;
    spkg)		spkg ; STATUS=$? ;;
    finish)		finish ; STATUS=$? ;;
    checksig)		checksig ; STATUS=$? ;;
    first)		mkdirs && spkg && finish ; STATUS=$? ;;
    upto-conf)  checksig && prep && conf; STATUS=$? ;;
    upto-build)  checksig && prep && conf && build; STATUS=$? ;;
    upto-install)  checksig && prep && conf && build && install; STATUS=$? ;;
    upto-pkg)  checksig && prep && conf && build && install && \
			strip && pkg ; \
			STATUS=$? ;;    
    upto-spkg)  checksig && prep && conf && build && install && \
			strip && pkg && spkg ; \
			STATUS=$? ;;    
    all)		checksig && prep && conf && build && install && \
			strip && pkg && spkg && finish ; \
			STATUS=$? ;;
    *) echo "Error: bad arguments" ; exit 1 ;;
  esac
  ( exit ${STATUS} ) || exit ${STATUS}
  shift
done

