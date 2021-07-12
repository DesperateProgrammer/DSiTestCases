
#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
all: 
	$(MAKE) -C NWRAM && cp ./NWRAM/NWRAM.nds ./builds/NWRAM.nds -f
	$(MAKE) -C NWRAM_RegisterMasks && cp ./NWRAM_RegisterMasks/NWRAM_RegisterMasks.nds ./builds/NWRAM_RegisterMasks.nds -f

#---------------------------------------------------------------------------------
clean:
	$(MAKE) -C NWRAM clean
	$(MAKE) -C NWRAM_RegisterMasks clean
	rm -f ./builds/*.nds
