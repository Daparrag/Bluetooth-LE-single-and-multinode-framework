# BLE-Library for support Synchonous Multimedia Applications in BLE.

{<img src="https://travis-ci.org/Daparrag/Lib.svg?branch=master" alt="Build Status" />}[https://travis-ci.org/Daparrag/Lib]

This  is a firmware  which contain a collection of low level modules used to guarantee synchonization  in a multipoint connections for bluetooth LE 4.1. This should be used for synchonous multimedia applications.

### Motiavation 


- In this new era of IoT in which small devices are connected everywhere, cooperating and transmiting generally plane and small amount of data, well known as humidity, temperature, ligth, or vibrations had been widely explored by the market given that this kind of data does not requiere to much effort in its treatment or transmission. In a typical scenario the devices are connected to some access point, transmmit the data only where it is absolutely necessary possible in a regular periods of time; Thus the tranceiver is kept it off most of the time in order to save energy. 

- However, An special markets like home applications, not only requieres a simple data to be transmitted on the medium, also the multimedia application are get it interest, the principal drawback that exist for the multimedia is the special treatment needed for their traffic, as well the Iteroperability between the diferent protocols used for the transmission.

- Now, The well known Bluetooth 4.1 that allows theoretically a data rate up to > 1Mbps, that also support low energy and the recientversion also  multi-node connection where the devices could be master or pherispheral at the same time motivate me for implement a possible solution not only for mutimedia transmission, but also for applications who requieres synchonization between the devices.

- This implementation was thought for the board X-Nucleo-IDB0xA1 but it is easly to export to other platforms.

My interest in this project consist principally in allows synchronous multimedia transmissions in BLE 4.1 this repository only contain the basic firmware needed to implement this idea. 

In another repository I will post the solution for synchonous BLE using threfore this firmware. 


In the image you could see a simple example (**one node as a central an two as pherispheral**) after to be connected and after to exchange their services & charateristics. based on this firmware. 

<img src="https://github.com/Daparrag/Lib/blob/master/firmware/screenshots/connection_complete.gif" alt="connection complete" width="600px" />


## components description
This firmware is divided in Three Principal modules. 

1. **application module**.

2. **network module**.

3. **event handler module(depends of the platform)**. 

The follow figure show you the relation between those modules.  

<img src="https://github.com/Daparrag/Lib/blob/service_handler_branch/firmware/screenshots/Components_Structures.PNG" alt="Component Diagram" width="600px" />


- ### Application module:


This module brings the low level functionalities used for set up  profiles, service, attributes, configuration for connection(_**up to 8 connections, which should be started using independent configuration parameters (think in QoS)**_), advertisement, and scanning . 

- ### Network module:

This module handler the way in which the devices stablish a connection, this include all the connection steps; Therefore it is able to initialized the devices according to their respective role, then  start at time the corresponding connection, advertisement, or scanning. 

Aditionally the module  inclide a timer used to reconize if for example the connection is lost; As well have a mechanims able to filter the device's traffic reducing considerable the energy consumption. 

- ### event handler module:

this module basicaly capture all the BLE events, that posteriorly will be allocate in a event queue where the application, or network module could be access to processesing the events.

Recently, was include the time stamp for synchonization purpose, an a event queue handler to introduce priority to the queue and thus a basic QoS could be added in the future.  


## UseCase diagram

<img src="https://github.com/Daparrag/Lib/blob/service_handler_branch/firmware/screenshots/UseCaseDiagram.PNG" alt="Usecase Diagram" width="600px" />

