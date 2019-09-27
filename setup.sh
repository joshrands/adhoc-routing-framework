ar -cvq libaodv.a aodv/*.o
g++ -o do-adhoc do-adhoc.cc libaodv.a

