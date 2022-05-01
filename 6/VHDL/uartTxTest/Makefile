PROJ = uart8n1_tx

PIN_DEF = uart8n1_tx.pcf
DEVICE = hx1k

all: $(PROJ).bin

%.json: uart8n1_tx_tb.vhdl %.vhdl
	yosys -m /usr/local/share/yosys/plugins/ghdl.so -p 'ghdl $^ -e uart8n1_tx_tb; synth_ice40 -json $@'

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

#all:
#	yosys -m /usr/local/share/yosys/plugins/ghdl.so -p 'ghdl uart8n1_tx.vhdl uart8n1_tx_tb.vhdl -e uart8n1_tx_tb; synth_ice40 -json uart8n1_tx.json'