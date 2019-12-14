#!/usr/bin/sh

mydir=$(dirname $0)
builddir=${mydir}/build
srcdir=${mydir}/src

args=""
if [ "$1" == "clean" ]; then
    args+="--target clean"
fi

if [ ! -d "${builddir}" ]; then
    mkdir -p ${builddir}
    cmake -B ${builddir} -S ${srcdir}
fi

cmake --build ${mydir}/build/ ${args} "${@}"
