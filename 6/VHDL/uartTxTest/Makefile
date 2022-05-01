PROJ = uart8n1_rx2

PIN_DEF = $(PROJ).pcf
DEVICE = hx1k

all: $(PROJ).bin

%.json: $(PROJ)_tb.vhdl %.vhdl
	yosys -m /usr/local/share/yosys/plugins/ghdl.so -p 'ghdl $^ -e $(PROJ)_tb; synth_ice40 -json $@'

%.asc: %.json
	~/nextpnr/nextpnr-ice40 --package $(DEVICE) --pcf $(PIN_DEF) --json $< --asc $@

%.bin: %.asc
	icepack $< $@

prog: $(PROJ).bin
	iceprog $<

clean:
	rm -f $(PROJ).json $(PROJ).asc $(PROJ).bin

.SECONDARY:

.PHONY: all prog clean