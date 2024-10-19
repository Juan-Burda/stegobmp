#!/bin/bash

./stegobmp.sh -extract -p resources/ana/ladoLSB1.bmp -out resources/output/archivo_1.png -steg LSB1

./stegobmp.sh -extract -p resources/ana/ladoLSB4.bmp -out resources/output/archivo_4.png -steg LSB4

./stegobmp.sh -extract -p resources/ana/ladoLSBI.bmp -out resources/output/archivo_i.png -steg LSBI

./stegobmp.sh -extract -p resources/enc/ladoLSBIaes256ofb.bmp -out resources/output/archivo_lsbi_aes256ofb.png -a aes256 -m ofb -pass margarita -steg LSBI

./stegobmp.sh -embed -p resources/ray.BMP -out resources/ray_w_flag_i.bmp -in resources/flag.BMP -steg LSBI

./stegobmp.sh -extract -p resources/ray_w_flag_i.bmp -out resources/output/flag_i.bmp -steg LSBI

./stegobmp.sh -embed -p resources/ray.BMP -out resources/ray_w_flag_1.bmp -in resources/flag.BMP -steg LSB1

./stegobmp.sh -extract -p resources/ray_w_flag_1.bmp -out resources/output/flag_1.bmp -steg LSB1

./stegobmp.sh -embed -p resources/ray.BMP -out resources/ray_w_flag_4.bmp -in resources/flag.BMP -steg LSB4

./stegobmp.sh -extract -p resources/ray_w_flag_4.bmp -out resources/output/flag_4.bmp -steg LSB4
