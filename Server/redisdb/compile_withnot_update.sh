#!/bin/bash

target=libredisdb.a

   echo 'Now begin compile the project:'
   eval chmod +x convert.py
   eval pwd
   eval make distclean
   eval rm -f *.o
   autoscan
   aclocal
   autoconf
   eval ./convert.py
   sed 's:$(ALLLIBS):-lrt -lssl @PTHREAD_LIBS@:g' Makefile.am > 111tmp.file
   mv -f 111tmp.file Makefile.am
   touch NEWS
   touch README
   touch AUTHORS
   touch ChangeLog
   automake --add-missing
   ./configure
#   sed 's/CFLAGS = -g -O2/CFLAGS = -g -O -DDEBUG/g' Makefile > 222tmp.file
#   sed 's/CXXFLAGS = -g -O2/CXXFLAGS = -g -O -DDEBUG/g' 222tmp.file> 333tmp.file
#   mv -f 333tmp.file Makefile
#   rm -f 111tmp.file, 222tmp.file, 333tmp.file
   make -j4
   if [ -f $target ];then
      echo 'The LoginServer have compiled suscess, you can get it in ../lib/libredisdb.a'
      eval mv -f libredisdb.a ../lib/libredisdb.a
   else
      echo 'The libredisdb.a compile error.'
   fi
