# bash script to copy aodv into ns3 module 

cp -r ./aodv ./ns-allinone-3.28/ns-3.28/src/custom-aodv/model
mv ./ns-allinone-3.28/ns-3.28/src/custom-aodv/model/aodv/* ./ns-allinone-3.28/ns-3.28/src/custom-aodv/model
rm ./ns-allinone-3.28/ns-3.28/src/custom-aodv/model/*.o
rm ./ns-allinone-3.28/ns-3.28/src/custom-aodv/model/test*

