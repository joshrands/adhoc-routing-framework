echo "Moving csm-patch..."
mv ./ns-3.28 ./csm-patch
python3.7 download.py -n ns-3.28
echo "ns-3.28 installed. Adding csm-patch..."
cp -r ./csm-patch/src ./ns-3.28
cp -r ./csm-patch/scratch ./ns-3.28
