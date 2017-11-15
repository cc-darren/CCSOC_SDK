"Zeus_Bin_Enc.exe" ./Objects/FPGA_M0p.bin -o ./Objects/FPGA_M0p_enc.bin

"../nrfutil/nrfutil.exe" pkg generate --hw-version 52 --sd-req 0x8C --application-version 0xff --application ./Objects/FPGA_M0p.bin --key-file ../key/priv.pem ./Objects/Zeus_M0.zip
"../nrfutil/nrfutil.exe" pkg generate --hw-version 52 --sd-req 0x8C --application-version 0xff --application ./Objects/FPGA_M0p_enc.bin --key-file ../key/priv.pem ./Objects/Zeus_M0_enc.zip

