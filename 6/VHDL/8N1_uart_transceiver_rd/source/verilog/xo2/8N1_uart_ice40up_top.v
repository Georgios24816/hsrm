`include "baudrates.v"

module uart_ice40up_top(
	input wire resetn,
	input wire i_rx_serial,
	output wire o_tx_serial
);

parameter BAUDRATE = `B9600;

wire clk;
wire [7:0] data;
wire data_valid;

uart_tranceiver #(.BAUDRATE(BAUDRATE),
				.TX_FIFO_MODE(1),
				.RX_FIFO_MODE(1)
) uart_transceiver_u0(
	.clk(clk),
	.resetn(resetn),


	/////////////TX ports///////////////
	.i_tx_data(data),
	.i_tx_data_valid(data_valid),
	.o_tx_serial(o_tx_serial),
	.o_tx_ready(),


	/////////////RX ports///////////////
	.i_rx_serial(i_rx_serial),
	.o_rx_data(data),
	.o_rx_data_valid(data_valid)
);

defparam OSCH_inst.NOM_FREQ = "24.18";	
OSCH OSCH_inst( .STDBY(1'b0 ), 		// 0=Enabled, 1=Disabled also Disabled with Bandgap=OFF
                .OSC(clk_i),
                .SEDSTDBY());		//  this signal is not required if not using SED - see TN1199 for more details.
								
CLKDIVC CLKDIVC_inst(.CLKI(clk_i),
					.CDIVX(clk));


endmodule