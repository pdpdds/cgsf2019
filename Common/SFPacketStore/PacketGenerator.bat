set DST_DIR=C:\Dev\Level0\CGSF\trunk\Common\SFPacketStore
set SRC_DIR=C:\Dev\Level0\CGSF\trunk\Common\SFPacketStore
protoc -I=%SRC_DIR% --cpp_out=%DST_DIR% %SRC_DIR%/PacketCore.proto
protoc -I=%SRC_DIR% --cpp_out=%DST_DIR% %SRC_DIR%/SFPacketStore.proto
protoc -I=%SRC_DIR% --cpp_out=%DST_DIR% %SRC_DIR%/ProtocolPacket.proto
protoc -I=%SRC_DIR% --cpp_out=%DST_DIR% %SRC_DIR%/SevenGamePacket.proto
protoc -I=%SRC_DIR% --cpp_out=%DST_DIR% %SRC_DIR%/FPSPacket.proto
pause