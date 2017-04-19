#!/bin/bash
# installs ipr family of programs
# specify installation directory
IDIR=/usr/bin
overwrite=0
if [ a"$1" = a"-o" ]; then
  overwrite=1
fi

echo Copying programs to $IDIR
target=$IDIR/ipr
if [ -s $target ]; then
  if [ $overwrite = 0 ]; then
    echo $target already exists. Use $0 -o to overwrite.
  else
    cp ipr $target
  fi
else
  echo cp ipr $target
  cp ipr $target
fi
SRC=$IDIR/ipr
for fname in ipq ipc iprm
  do
    target=$IDIR/$fname
    if [ -s $target ]; then
      if [ $overwrite = 0 ]; then
        echo $target already exists. Use $0 -o to overwrite.
      else
        echo ln -f -s $SRC $target
        ln -f -s $SRC $target
      fi
    else
      echo ln -f -s $SRC $target
      ln -f -s $SRC $target
    fi
  done
echo Copying templates to $IDIR. Existing files will be backed up.
for fname in ipp-template ipp-narrowjobs
  do
    target=$IDIR/$fname
    if [ -s $target ]; then
      if [ $overwrite = 0 ]; then
        echo $target already exists. Use $0 -o to overwrite.
        continue
      fi
    fi
    echo cp -b $fname $target
    cp -b $fname $target
  done

