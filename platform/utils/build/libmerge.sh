#!/bin/bash
#
# Quick script to merge static libs. $2 gets merged into $1

ANAME=`basename $2`
DNAME=$ANAME-contents
rm -rf /tmp/$DNAME
mkdir -p /tmp/$DNAME
cp $2 /tmp/$DNAME
pushd /tmp/$DNAME
ar x $ANAME
rm $ANAME
popd
ar rs $1 /tmp/$DNAME/*


