echo "Moving csm-patch..."
mv ./ns-3.28 ./csm-patch
python3.7 download.py -n ns-3.28
echo "ns-3.28 installed. Adding csm-patch..."
cp -r ./csm-patch/src ./ns-3.28
cp -r ./csm-patch/scratch ./ns-3.28
rm -r csm-patch

echo ""
echo "CSM ns3 patch installed. Please configure, build, and install ns3 modules: "
echo "cd ns-3.28"
echo "./waf configure --build-profile=optimized"
echo "./waf"
echo "sudo ./waf install"
