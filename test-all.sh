#make clean-all
#make all

echo "TESTING SUBMODULES..."

./aodv/test | grep "FAIL\|PASS"
./hardware/test | grep "FAIL\|PASS"
./rem/test | grep "FAIL\|PASS"
./hello_monitor/test | grep "FAIL\|PASS"
./socket/test | grep "FAIL\|PASS"
./data_structures/test | grep "FAIL\|PASS"

