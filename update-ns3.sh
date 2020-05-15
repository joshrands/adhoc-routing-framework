# bash script to copy adhoc routing into ns3 module 
# remove log dirs if they exist

[ -d "./ns-3-allinone/ns-3.28/src/adhoc-routing/model" ] && rm -r ./ns-3-allinone/ns-3.28/src/adhoc-routing/model
mkdir ./ns-3-allinone/ns-3.28/src/adhoc-routing/model

# copy everything over to ns3 
cp -r ./aodv/*.h ./ns-3-allinone/ns-3.28/src/adhoc-routing/model
cp -r ./aodv/*.cc ./ns-3-allinone/ns-3.28/src/adhoc-routing/model
cp -r ./adhoc/*.h ./ns-3-allinone/ns-3.28/src/adhoc-routing/model
cp -r ./adhoc/*.cc ./ns-3-allinone/ns-3.28/src/adhoc-routing/model
cp -r ./rem/*.h ./ns-3-allinone/ns-3.28/src/adhoc-routing/model
cp -r ./rem/*.cc ./ns-3-allinone/ns-3.28/src/adhoc-routing/model
cp -r ./simulation/*.h ./ns-3-allinone/ns-3.28/src/adhoc-routing/model
cp -r ./simulation/*.cc ./ns-3-allinone/ns-3.28/src/adhoc-routing/model
cp -r ./hello_monitor/*.h ./ns-3-allinone/ns-3.28/src/adhoc-routing/model
cp -r ./hello_monitor/*.cc ./ns-3-allinone/ns-3.28/src/adhoc-routing/model

rm ./ns-3-allinone/ns-3.28/src/adhoc-routing/model/test*

cp ./defines.h ./ns-3-allinone/ns-3.28/src/adhoc-routing/model
