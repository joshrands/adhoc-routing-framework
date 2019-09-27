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

cd ../

ar -cvq libaodv.a aodv/*.o
ar -cvq libsocket.a socket/*.o

g++ -o do-adhoc -std=c++11 -pthread do-adhoc.cc libaodv.a libsocket.a
