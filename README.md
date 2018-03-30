# IGADPiGameServer
Game server created for educational purposes for IGAD (Breda University of Applied Sciences, the Netherlands)

<img src="https://insight.nhtv.nl/wp-content/uploads/2018/01/English-banner.jpg" />

## Build status
| Branch | Status                                                                                                                                                               |
| ------ |:-------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| master | [<img src="https://api.travis-ci.org/BertHeesakkers/IGADPiGameServer.svg?branch=master">](https://travis-ci.org/BertHeesakkers/IGADPiGameServer) [<img alt="Coverity Scan Build Status" src="https://img.shields.io/coverity/scan/15346.svg"/>](https://scan.coverity.com/projects/bertheesakkers-igadpigameserver") [![Coverage Status](https://coveralls.io/repos/github/BertHeesakkers/IGADPiGameServer/badge.svg?branch=master)](https://coveralls.io/github/BertHeesakkers/IGADPiGameServer?branch=master) [![Build status](https://ci.appveyor.com/api/projects/status/fde63as7eflbgq40/branch/master?svg=true)](https://ci.appveyor.com/project/BertHeesakkers/igadpigameserver/branch/master)|

## Documentation
For documentation references please check out the following folder [Documentation](/Documentation)

## How to use

In order to learn how to use the server, check out the documentation. There you will find help files for how to communicate with the server.
### Dependencies

- CMake 3.9.2 (latest version supported by Visual Studio 2017)
- RakNet build as static lib
### Building dependencies

This little bash script will help you build RakNet:
```bash
#!/bin/bash

# RakNet installation:
# Commands below provided by Bert Heesakkers (NHTV) and modified by Zandor Smith.
sudo apt-get install cmake -y

git clone https://github.com/facebookarchive/RakNet.git
cd RakNet
cmake .
make
make install

echo Removing old RakNet installation if it exists.
sudo rm -rf /usr/local/include/raknet

echo Moving RakNet to the correct directory.
sudo mv [RakNet path]/RakNet/Lib/LibStatic/libRakNetLibStatic.a /usr/local/lib
sudo mv [RakNet path]/RakNet/include/raknet /usr/local/include/

echo Cleaning up...
rm -rf [RakNet path]/RakNet

echo RakNet installed.
```

## Build

```
$ cd /server_root
$ cmake .
$ make install
```

## Run
You have to make sure that there is a log folder and that the app has the premission to write in it:
```
$ cd /server/path/bin/
$ mkdir -p log/Games
$ chmod 666 -R log
```
The you can run the application:
```
$ /server/path/bin/ServerApp
```

### Supported compiler

- GCC 4.9 - 7.2 (and possibly later) (because of the [json.hpp](https://github.com/nlohmann/json#supported-compilers))
- Clang 3.4 - 5.0 (and possibly later) (because of the [json.hpp](https://github.com/nlohmann/json#supported-compilers))
- Intel C++ Compiler 17.0.2 (and possibly later) (because of the [json.hpp](https://github.com/nlohmann/json#supported-compilers))
- Microsoft Visual C++ 2015 / Build Tools 14.0.25123.0 (and possibly later) (because of the [json.hpp](https://github.com/nlohmann/json#supported-compilers))
- Microsoft Visual C++ 2017 / Build Tools 15.5.180.51428 (and possibly later) (because of the [json.hpp](https://github.com/nlohmann/json#supported-compilers))
