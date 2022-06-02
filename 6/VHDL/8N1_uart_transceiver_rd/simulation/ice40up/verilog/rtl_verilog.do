

set PROJ_DIR "<ENTER> Simulation DIRECTORY PATH HERE"

# Example: 
# set PROJ_DIR "C:\8N1_uart_tranceiver_rd\simulation"

cd $PROJ_DIR/ice40up/verilog/

if {![file exists rtl_verilog]} {
    vlib rtl_verilog 
}
endif

design create rtl_verilog .
design open rtl_verilog
adel -all

cd $PROJ_DIR/ice40up/verilog/

 

alog -O2 -sve  -work rtl_verilog ../../../source/verilog/common/uart_transceiver.v \
../../../source/verilog/common/baudrates.v \
../../../source/verilog/common/uart_tx.v \
../../../source/verilog/common/uart_rx.v \
../../../source/verilog/ice40up/dp_ram_fifo.v \
../../../testbench/ice40up/verilog/uart_tb.v \
../../../projects/ice40up/ice40up_verilog/dpram/rtl/dpram.v \
C:/lscc/radiant/2.0/ip/pmi/pmi_iCE40UP.v 




asim -O5 -L iCE40UP -L ovi_iCE40UP -PL ovi_iCE40UP -PL iCE40UP +access +w_nets +access +r +access +r+w uart_tb



add wave -named_row "TX" -bold -height 24
add wave -noreg -logic {/uart_tb/uart_tx_u0/clk}
add wave -noreg -logic {/uart_tb/uart_tx_u0/resetn}
add wave -noreg -hexadecimal -literal {/uart_tb/uart_tx_u0/i_tx_data}
add wave -noreg -logic {/uart_tb/uart_tx_u0/i_tx_data_valid}
add wave -noreg -logic {/uart_tb/uart_tx_u0/genblk1/tx_fifo_we}
add wave -noreg -hexadecimal -literal {/uart_tb/uart_tx_u0/genblk1/tx_fifo_dout}
add wave -noreg -logic {/uart_tb/uart_tx_u0/genblk1/tx_fifo_rd}
add wave -noreg -logic {/uart_tb/uart_tx_u0/o_tx_serial}
add wave -noreg -logic {/uart_tb/uart_tx_u0/o_tx_ready}
add wave -named_row "RX" -bold -height 24
add wave -noreg -logic {/uart_tb/uart_rx_u0/clk}
add wave -noreg -logic {/uart_tb/uart_rx_u0/resetn}
add wave -noreg -logic {/uart_tb/uart_rx_u0/i_rx_serial}
add wave -noreg -hexadecimal -literal {/uart_tb/uart_rx_u0/genblk2/rx_fifo_din}
add wave -noreg -logic {/uart_tb/uart_rx_u0/genblk2/rx_fifo_din_valid}
add wave -noreg -logic {/uart_tb/uart_rx_u0/genblk2/rx_fifo_we}
add wave -noreg -logic {/uart_tb/uart_rx_u0/genblk2/rx_fifo_rd}
add wave -noreg -logic {/uart_tb/uart_rx_u0/genblk2/rx_data_valid}
add wave -noreg -hexadecimal -literal {/uart_tb/uart_rx_u0/o_rx_data}
add wave -noreg -logic {/uart_tb/uart_rx_u0/o_rx_data_valid}


#log -r /*
run 11200 us

##============================================
##End of Simulation
##============================================

