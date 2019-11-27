# bash script to copy adhoc routing into ns3 module 
# remove log dirs if they exist

[ -d "./ns-allinone-3.28/ns-3.28/src/adhoc-routing/model" ] && rm -r ./ns-allinone-3.28/ns-3.28/src/adhoc-routing/model
mkdir ./ns-allinone-3.28/ns-3.28/src/adhoc-routing/model
mkdir ./ns-allinone-3.28/ns-3.28/src/adhoc-routing/model/aodv
mkdir ./ns-allinone-3.28/ns-3.28/src/adhoc-routing/model/monitoring

# copy aodv 
cp -r ./aodv/*.h ./ns-allinone-3.28/ns-3.28/src/adhoc-routing/model/aodv
cp -r ./aodv/*.cc ./ns-allinone-3.28/ns-3.28/src/adhoc-routing/model/aodv
cp -r ./monitoring/*.h ./ns-allinone-3.28/ns-3.28/src/adhoc-routing/model/monitoring
cp -r ./monitoring/*.cc ./ns-allinone-3.28/ns-3.28/src/adhoc-routing/model/monitoring

rm ./ns-allinone-3.28/ns-3.28/src/adhoc-routing/model/aodv/test*
rm ./ns-allinone-3.28/ns-3.28/src/adhoc-routing/model/monitoring/test*

cp ./defines.h ./ns-allinone-3.28/ns-3.28/src/adhoc-routing/model
cp ./adhoc_routing.* ./ns-allinone-3.28/ns-3.28/src/adhoc-routing/model
rm ./ns-allinone-3.28/ns-3.28/src/adhoc-routing/model/adhoc_routing.o

#cp -r ./aodv ./ns-allinone-3.28/ns-3.28/src/custom-aodv/model
#cp defines.h ./ns-allinone-3.28/ns-3.28/src/custom-aodv
#mv ./ns-allinone-3.28/ns-3.28/src/custom-aodv/model/aodv/* ./ns-allinone-3.28/ns-3.28/src/custom-aodv/model
#rm ./ns-allinone-3.28/ns-3.28/src/custom-aodv/model/*.o
#rm ./ns-allinone-3.28/ns-3.28/src/custom-aodv/model/test*

#[ -d "./ns-allinone-3.28/ns-3.28/src/custom-aodv/model" ] && rm -r ./ns-allinone-3.28/ns-3.28/src/custom-aodv/model
#mkdir ./ns-allinone-3.28/ns-3.28/src/custom-aodv/model

#cp -r ./aodv ./ns-allinone-3.28/ns-3.28/src/custom-aodv/model
#cp defines.h ./ns-allinone-3.28/ns-3.28/src/custom-aodv
#mv ./ns-allinone-3.28/ns-3.28/src/custom-aodv/model/aodv/* ./ns-allinone-3.28/ns-3.28/src/custom-aodv/model
#rm ./ns-allinone-3.28/ns-3.28/src/custom-aodv/model/*.o
#rm ./ns-allinone-3.28/ns-3.28/src/custom-aodv/model/test*
