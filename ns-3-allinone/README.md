# This is ns-3-allinone.

If you have downloaded this in tarball release format, this directory
contains some released ns-3 version, along with 3rd party components
necessary to support all optional ns-3 features, such as Python
bindings and the Network Animator.  In this case, just run the
script build.py; all the components, plus ns-3 itself, will thus be
built.  This directory also contains the bake build tool, which allows
access to several additional modules including the Network Simulation
Cradle, Direct Code Execution environment, and click and openflow 
extensions for ns-3.

If you have downloaded this from mercurial, the download.py script will
download bake, netanim, pybindgen, and ns-3-dev.  The usage to use
basic ns-3 (netanim and ns-3-dev) is to type:

	./download.py
	./build.py

and cd into ns-3-dev for further work.  Consult the bake documentation
on how to use bake to access optional ns-3 components.

# Accessing Telemetry Data

### Buffer state (available bytes)
The amount of space available in the nodes packet buffer.

	long AODV::getPacketBufferAvailableBytes()
	
Example can be found in adhoc-routing-framework/ns-3-allinone/ns-3.28/scratch/adhoc-example/adhoc-routing-test.cc at line 94.

### Data sent over link (bit/s)
The amount of data that sent from node A to node B over the last second.

	int AdHocRoutingHelper::getLinkBitsSent(IP_ADDR destIP)
	
Example can be found in adhoc-routing-framework/ns-3-allinone/ns-3.28/scratch/adhoc-example/adhoc-routing-test.cc at line 96.

### Link status
Determine if there exists an active link between two nodes.

	bool AdhocRoutingProtocol::linkExists(IP_ADDR dest)
	
Example can be found in adhoc-routing-framework/ns-3-allinone/ns-3.28/scratch/adhoc-example/adhoc-routing-test.cc at line 106.

### Potential bandwidth per link (bits/s)
The potential bandwidth available between node A to node B. This divides the available bandwidth among all nodes connected to B, based on signal strength. The share of bandwidth that is allotted to A is then limited based on the distance between A and B using a quadratic loss model. 

	int AdHocRoutingHelper::getLinkBandwidthBits(AdHocRoutingHelper* targetHelper)
	
Example can be found in adhoc-routing-framework/ns-3-allinone/ns-3.28/scratch/adhoc-example/adhoc-routing-test.cc at line 96 and line 104.
