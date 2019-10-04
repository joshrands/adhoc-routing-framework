rm libaodv.a
rm libsocket.a
rm do-aodv.o

cd aodv
make clean
make
rm test.o

cd ../socket
make clean
make
rm test.o

cd ../hardware
make clean
make
rm test.o

cd ../

g++ -o do-adhoc -std=c++11 -pthread do-adhoc.cc aodv/libaodv.a socket/libsocket.a hardware/libhardware.a
