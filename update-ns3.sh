# bash script to copy adhoc routing into ns3 module 
# remove log dirs if they exist

[ -d "./ns-allinone-3.28/ns-3.28/src/adhoc-routing/model" ] && rm -r ./ns-allinone-3.28/ns-3.28/src/adhoc-routing/model
mkdir ./ns-allinone-3.28/ns-3.28/src/adhoc-routing/model

# copy everything over to ns3 
cp -r ./aodv/*.h ./ns-allinone-3.28/ns-3.28/src/adhoc-routing/model
cp -r ./aodv/*.cc ./ns-allinone-3.28/ns-3.28/src/adhoc-routing/model
cp -r ./adhoc/*.h ./ns-allinone-3.28/ns-3.28/src/adhoc-routing/model
cp -r ./adhoc/*.cc ./ns-allinone-3.28/ns-3.28/src/adhoc-routing/model
cp -r ./rem/*.h ./ns-allinone-3.28/ns-3.28/src/adhoc-routing/model
cp -r ./rem/*.cc ./ns-allinone-3.28/ns-3.28/src/adhoc-routing/model
cp -r ./simulation/*.h ./ns-allinone-3.28/ns-3.28/src/adhoc-routing/model
cp -r ./simulation/*.cc ./ns-allinone-3.28/ns-3.28/src/adhoc-routing/model

rm ./ns-allinone-3.28/ns-3.28/src/adhoc-routing/model/test*

cp ./defines.h ./ns-allinone-3.28/ns-3.28/src/adhoc-routing/model
