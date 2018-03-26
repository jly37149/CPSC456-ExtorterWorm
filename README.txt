Joseph Ly
flamehunt7149@csu.fullerton.edu

Default login and pw are used for all
VMs. ubuntu and 123456

VM1 will be the starting point of all worms.
All worms and other necessary files will placed
in the home directory for easy access and viewing

The /tmp/ directory will also be used in the creation
of various files, so keep an eye on that.

A /cpsc/Documents/ directory was also placed in the home directory
This folder will be relevant to the extorter worms.

All the necessary preparation and setup should have already been done.
However, if the python worm does not run successfully, run the
install.sh (provided in class) script on all VMs

To run python worms (MUST BE EXACT):

python replicator_worm.py 192.168.1.5
python extorter_worm.py 192.168.1.5
python passwordthief_worm.py 192.168.1.5

All the necessary preparation and setup should have already been done.
However, if the bonus worm does not run successfully, install
g++-5 and libssh-dev on all VMs

To compile bonus c++ worms (MUST BE EXACT):
g++-5 -std=c++11 ./replicator_bonus.cpp -o replicator_bonus -lssh
g++-5 -std=c++11 ./extorter_bonus.cpp -o extorter_bonus -lssh
g++-5 -std=c++11 ./pwthief_bonus.cpp -o pwthief_bonus -lssh

To run bonus c++ worms (MUST BE EXACT):
./replicator_bonus 33
./extorter_bonus 33
./pwthief_bonus 33

Note: The second argv for the bonus c++ worms in truth can be almost anything
They are not used anywhere other than signaling if the worm ran from an
attacker or victim

Note: To re-run any worms, the /tmp/infected.txt file on VM2 and VM3 WILL have to be deleted.
Other files created during the worm run for VM2 and VM3 should be deleted as well.

Note: On victim runs, the worm will also output an log in the home directory
