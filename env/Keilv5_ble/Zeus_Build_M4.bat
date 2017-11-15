"Zeus_Bin_Enc.exe" ./Objects/FPGA_M4f.bin -o ./Objects/FPGA_M4f_enc.bin

"../nrfutil/nrfutil.exe" pkg generate --hw-version 52 --sd-req 0x8C --application-version 0xff --application ./Objects/FPGA_M4f.bin --key-file ../key/priv.pem ./Objects/Zeus_M4.zip
"../nrfutil/nrfutil.exe" pkg generate --hw-version 52 --sd-req 0x8C --application-version 0xff --application ./Objects/FPGA_M4f_enc.bin --key-file ../key/priv.pem ./Objects/Zeus_M4_enc.zip
