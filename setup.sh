cd aodv
make clean
make

cd ../socket
make clean
make

cd ../

ar -cvq libaodv.a aodv/*.o
ar -cvq libsocket.a socket/*.o

g++ -o do-adhoc -std=c++11 -pthread do-adhoc.cc libaodv.a libsocket.a
