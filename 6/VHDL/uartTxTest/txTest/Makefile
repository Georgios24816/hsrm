PROJ = uart8n1_tx2

PIN_DEF = $(PROJ).pcf
DEVICE = hx1k

all: $(PROJ).bin

%.json: $(PROJ)_tb.vhdl %.vhdl
	yosys -m /usr/local/share/yosys/plugins/ghdl.so -p 'ghdl chacha_qr.vhdl uart8n1_tx.vhdl $^ -e $(PROJ)_tb; synth_ice40 -json $@'

%.asc: %.json
	~/nextpnr/nextpnr-ice40 --package $(DEVICE) --pcf $(PIN_DEF) --json $< --asc $@

%.bin: %.asc
	icepack $< $@

prog: $(PROJ).bin
	iceprog $<

clean:
	rm -f $(PROJ).json $(PROJ).asc $(PROJ).bin

sim.vcd: uart8n1_tx.vhdl uart8n1_tx2.vhdl uart8n1_tx2_tb.vhdl chacha_qr.vhdl
	ghdl -a uart8n1_tx.vhdl uart8n1_tx2.vhdl uart8n1_tx2_tb.vhdl chacha_qr.vhdl
	ghdl -e uart8n1_tx2_tb
	ghdl -r uart8n1_tx2_tb --vcd=sim.vcd


.SECONDARY:

.PHONY: all prog clean
