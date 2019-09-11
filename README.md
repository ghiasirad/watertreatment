# WaterTreatment Facility Simulation
The repository aims to provide a powerful framework using Unity3D to give a more realistic impression of a working Water Treatment facility. This model consists of two main parts. The front end is built using Uniy3D 2018 game engine, and to build the backend, different tools have been utilized to give the user power of communicating with OpenPLC using the Modbus protocol. The system simulator will be as below:

![Image of Main Page](https://github.com/ghiasirad/watertreatment/blob/master/Builds/Embeded/WT_MainPage.png)

![Image of Operation](https://github.com/ghiasirad/watertreatment/blob/master/Builds/Embeded/WT_Operation.png)

## Build from scratch
1. To build, you will need three different machines along with the host. Please download the latest version of VirtualBox by https://www.virtualbox.org. Different hosts will be able to run the Unity3D simulator based on the platform. The /Sim/Builds/ folder contains three different games for Linux, Mac, and Windows. Please use the one based on your preference.

2. After installing the three different Ubuntu 16.04 virtual machines, you may want to set up a new Host-Only network with IP addresses starting 192.168.95.1 with Subnet Mask of 255.255.255.0 and assign the IP addresses to the machines. Please make sure to keep the IP addresses of 192.168.95.20 to 192.168.95.35 free for the modbus use.

3. Use the "Sim" directory and run the backend simulation in one of the machines. You may start the modbus protocol using Sim/remote_io/Final/run_all.sh script. You may also set up the static IPs required for Modbus by "interfaces" in "Sim" directore.

4. Use the second guest machine with the designated IP and create the OpenPLC server using the scripts provided in "OpenPLC" directory.

5. By your choice, you can rung your HMI in the third machine to complete the process.

## Build from the provided machines
You can download the provided machines and process using the scripts in the repository.

1. Download the machine from https://drive.google.com/open?id=1bfFZCLnCNgs6fgoMiV3_qp5phrubMppg. There are required simulation 
information and libraries pre-installed on the machine to run the simulator and the Modbus protocols.

2. Download the OpenPLC machine from https://drive.google.com/open?id=1Red9pg89Bi0Ia5caOlfm7cUBZw87s4EG.

3. Use any other machine with the designated IP and your choice of HMI for the third machine. For this project, a machine is provided and is accessible from the link which includes an HMI for the water treatment facility and is developed by Advance HMI tool (https://www.advancedhmi.com).

## Loging in
the credentials for every machine is user:password
