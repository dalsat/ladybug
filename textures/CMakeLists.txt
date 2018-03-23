set (TEXTURES cocci_128x128_RGB.raw grass_512x512_RGB.raw water_512x512_RGB.raw clouds_1024x512_RGB.raw)

# copying textures
#configure_file (${TEXTURES} texture COPYONLY)
file (COPY ${TEXTURES} DESTINATION .)