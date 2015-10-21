PMCC    = vbccpm
PMAS    = pmas
IMG2PM  = img2pm
BIN2H   = bin2h
PMUSB   = PokeFlash
PMEMU   = PokeMini
TARGET  = main.c

BINARIES = 

SPRITES         = gfx.h
TILES 			= tiles.h
TILES2 			= tiles_2.h
TILES3 			= tiles_3.h
TILES4 			= tiles_4.h
TILES5 			= tiles_5.h
TILES6 			= tiles_6.h
SPR_3SHADES     = gfx/linkwalk.gif
GROUND   = gfx/terrain.bmp
LVLD   = gfx/trou.bmp
GAMEOVER   = gfx/gameover.bmp

all:  $(SPRITES) $(TILES) $(TILES2) $(TILES3) $(TILES4) $(TILES5) $(TILES6) $(TARGET:.c=.min) $(BITMAPS:.bmp=.h) $(BINARIES:.bin=.h) 

$(SPRITES): $(SPR_3SHADES) $(GROUND) 
	$(IMG2PM) -o $(SPRITES) -s2 $(SPR_3SHADES) -t2 $(GROUND) 
	
$(TILES): $(LVLD) 
	$(IMG2PM) -o $(TILES) -t2 $(LVLD) 
	
$(TILES2): $(GAMEOVER)
	$(IMG2PM) -o $(TILES2) -t2 $(GAMEOVER) 
	
$(TILES3): $(LVLD) 
	$(IMG2PM) -o $(TILES3) -t2 $(LVLD) 
	
$(TILES4): $(LVLD) 
	$(IMG2PM) -o $(TILES4) -t2 $(LVLD) 
	
$(TILES5): $(LVLD) 
	$(IMG2PM) -o $(TILES5) -t2 $(LVLD) 
	
$(TILES6): $(LVLD) 
	$(IMG2PM) -o $(TILES6) -t2 $(LVLD) 
	
	


# compile .c files to .asm and then assemble to .min
%.asm : %.c
	$(PMCC) $<
%.min : %.asm
	$(PMAS) $< $@
	
%.h : %.bin
	$(BIN2H) $< $@


.PHONY: clean again install

clean:
	rm -f $(TARGET).min $(TARGET).asm

again: clean $(TARGET).min

install: $(TARGET).min
	$(PMUSB) -f $(TARGET).min

run: $(TARGET).min
	$(PMEMU) $(TARGET).min
