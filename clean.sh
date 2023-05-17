#/bin/bash
touch /tmp/garbage.txt 
killall officer > /tmp/garbage.txt
killall teller > /tmp/garbage.txt
killall palestinian > /tmp/garbage.txt
killall graphics > /tmp/garbage.txt
find /dev/mqueue -type f -name 'rtp*' -exec rm {} +
find /dev/shm -type f -name 'rtp*' -exec rm {} +
find /dev/shm -type f -name 'sem.rtp*' -exec rm {} +
exit
