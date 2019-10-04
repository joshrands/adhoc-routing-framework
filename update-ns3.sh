# bash script to copy aodv into ns3 module 

# remove log dirs if they exist
[ -d "./ns-allinone-3.28/ns-3.28/src/custom-aodv/model" ] && rm -r ./ns-allinone-3.28/ns-3.28/src/custom-aodv/model
mkdir ./ns-allinone-3.28/ns-3.28/src/custom-aodv/model

cp -r ./aodv ./ns-allinone-3.28/ns-3.28/src/custom-aodv/model
mv ./ns-allinone-3.28/ns-3.28/src/custom-aodv/model/aodv/* ./ns-allinone-3.28/ns-3.28/src/custom-aodv/model
rm ./ns-allinone-3.28/ns-3.28/src/custom-aodv/model/*.o
rm ./ns-allinone-3.28/ns-3.28/src/custom-aodv/model/test*

