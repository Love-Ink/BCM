firmwareFile=$(echo $1 | sed 's/^\([a-zA-Z]\)\(.*\)/\2/g; s/://g' | sed 's/\\/\//g')
d:/MounRiver/MounRiver_Studio/toolchain/OpenOCD/bin/openocd.exe -f d:/MounRiver/MounRiver_Studio/toolchain/OpenOCD/bin/wch-riscv.cfg \
    -c init -c halt \
    -c "flash erase_sector wch_riscv 0 last" \
    -c "program ${firmwareFile}" \
    -c "verify_image ${firmwareFile}" \
    -c wlink_reset_resume -c exit