all: 1.vhdl _testbench_FullAdder_TB.vhdl
	ghdl -s 1.vhdl _testbench_FullAdder_TB.vhdl
	ghdl -a 1.vhdl _testbench_FullAdder_TB.vhdl
	ghdl -e FullAdder
	ghdl -r FullAdder
	ghdl -r FullAdder --vcd=FullAdder.vcd
