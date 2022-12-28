#!/bin/sh
mkdir test
touch test/f1
touch test/f2
touch test/f3
mkdir test/fr1
touch test/fr1/f4
touch test/fr1/f5
mkdir test/fr2
touch test/fr2/f6
rm test/fr1/f5
rm -r test/fr1
rm -r test
