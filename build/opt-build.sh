#! /bin/bash

set -xe

if [[ -z "${TMPDIR}" ]]; then
  TMPDIR=/tmp
fi

set -u

if [ "$#" -lt "1" ] ; then
  echo "Please provide an installation path such as /opt/ICGC"
  exit 1
fi

# get path to this script
SCRIPT_PATH=`dirname $0`;
SCRIPT_PATH=`(cd $SCRIPT_PATH && pwd)`

# get the location to install to
INST_PATH=$1
mkdir -p $1
INST_PATH=`(cd $1 && pwd)`
echo $INST_PATH

# get current directory
INIT_DIR=`pwd`

CPU=`grep -c ^processor /proc/cpuinfo`
if [ $? -eq 0 ]; then
  if [ "$CPU" -gt "6" ]; then
    CPU=6
  fi
else
  CPU=1
fi
echo "Max compilation CPUs set to $CPU"

SETUP_DIR=$INIT_DIR/install_tmp
mkdir -p $SETUP_DIR/distro # don't delete the actual distro directory until the very end
mkdir -p $INST_PATH/bin
cd $SETUP_DIR

# make sure tools installed can see the install loc of libraries
set +u
export LD_LIBRARY_PATH=`echo $INST_PATH/lib:$LD_LIBRARY_PATH | perl -pe 's/:\$//;'`
export LIBRARY_PATH=`echo $INST_PATH/lib:$LIBRARY_PATH | perl -pe 's/:\$//;'`
export C_INCLUDE_PATH=`echo $INST_PATH/include:$C_INCLUDE_PATH | perl -pe 's/:\$//;'`
export PATH=`echo $INST_PATH/bin:$PATH | perl -pe 's/:\$//;'`
export MANPATH=`echo $INST_PATH/man:$INST_PATH/share/man:$MANPATH | perl -pe 's/:\$//;'`
export PERL5LIB=`echo $INST_PATH/lib/perl5:$PERL5LIB | perl -pe 's/:\$//;'`
set -u

# htslib
if [ ! -e $SETUP_DIR/htslib.success ]; then
  curl -sSL --retry 10 -o dist.tar.bz2 https://github.com/samtools/htslib/releases/download/${VER_HTSLIB}/htslib-${VER_HTSLIB}.tar.bz2
  mkdir dist
  mkdir -p $INST_PATH/fqtools/htslib
  tar --strip-components 1 -C dist -xjf dist.tar.bz2
  cd dist
  autoheader
  autoconf
  ./configure --prefix=$INST_PATH/fqtools/htslib
  make
  make install
  cd $SETUP_DIR
  rm -r dist*
  touch $SETUP_DIR/htslib.success
fi

# fqtools
if [ ! -e $SETUP_DIR/fqtools.success ]; then
  curl -sSL --retry 10 -o dist.tar.gz https://github.com/alastair-droop/fqtools/archive/${VER_FQTOOLS}.tar.gz
  mkdir dist
  tar --strip-components 1 -C $INST_PATH/fqtools -xvf dist.tar.gz
  cd $INST_PATH/fqtools
  export CPPFLAGS=-I$INST_PATH/fqtools/htslib/include
  export LDFLAGS=-L$INST_PATH/fqtools/lib
  make
  cd $SETUP_DIR
  rm -r dist*
  touch $SETUP_DIR/fqtools.success
fi
