# CGSF 2019
CGSF visual studio 2019 version.
CFSF support only 64 BIT.

Build Instruction
-------
To build project, we need visual studio 2019 and vcpkg

Install Library using [vcpkg](https://github.com/microsoft/vcpkg)
* ace
* protobuf
* openssl
* pthreads
* zlib
* libmysql
* freetype
* expat
* boost

Example
* vcpkg install ace:x64-windows
* vcpkg install openssl:x64-windows
* .......
* vcpkg integrate install

Download Binary From Github Release Tab
* Extract ThirdParty.rar to CGSF folder
* Extract Resources.rar to CGSF/BIN64 folder
* [Directx 2010 june version] (http://www.microsoft.com/en-us/download/details.aspx?id=6812)

Finally, In VSProject Folder
* Open ThirdParty.sln and Build it
* Open AllProject.sln and Build it