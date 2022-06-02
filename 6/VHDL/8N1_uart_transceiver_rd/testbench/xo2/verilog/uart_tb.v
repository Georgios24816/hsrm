`timescale 1ns/10ps
`include "baudrates.v"
 
module uart_tb ();

 
  // Testbench uses a 12 MHz clock
  // Want to interface to 9600 baud UART
  parameter CLOCK0_PERIOD_NS = 83.3291669; //(1/(Freq + %shift))x 10e9, e.g. (1/(12MHz + 600Hz) x 10e9 
  parameter CLOCK1_PERIOD_NS = 83.3375002; //(1/(Freq + %shift))x 10e9, e.g. (1/(12MHz - 600Hz) x 10e9
  parameter BAUDRATE    		= `B9600;
  
  reg resetn;
  reg r0_Clock = 0;
  reg r1_Clock = 0;
  reg tx_data_valid = 0;
  wire tx_ready;
  reg [7:0] tx_data = 0;
  wire rx_serial;
  wire [7:0] rx_data;
  reg [7:0] ram [0:511];
   
//Instantiating Tx Module   
uart_tranceiver #(.BAUDRATE(BAUDRATE),
					.TX_FIFO_MODE(1)
)	uart_tx_u0(
	.clk(				r0_Clock		),
	.resetn(			resetn		),

	/////////////TX ports///////////////
	.i_tx_data(			tx_data	),
	.i_tx_data_valid(	tx_data_valid		),
	.o_tx_serial(		rx_serial	), 
	.o_tx_ready(		tx_ready	)

);


uart_tranceiver #(.BAUDRATE(BAUDRATE),
					.RX_FIFO_MODE(1)
)	uart_rx_u0(
	.clk(				r1_Clock		),
	.resetn(			resetn		),

	/////////////RX ports///////////////
	.i_rx_serial(		rx_serial	),
	.o_rx_data(			rx_data	), 
	.o_rx_data_valid(				)
);
 
   
	always
		#(CLOCK0_PERIOD_NS/2) r0_Clock <= !r0_Clock;
	
	
	always
		#(CLOCK1_PERIOD_NS/2) r1_Clock <= !r1_Clock;
	
	
  initial
	begin
	$readmemh("../../../testbench/xo2/verilog/ram_init_file.mem",ram);
	end




   
  // Main Testing:
	integer ii;
	integer num_of_char = 8; 
  
	initial
		begin
			resetn = 0;
			#100;
			resetn = 1;
			#100;


			// Send Test Pattern
			@(posedge r0_Clock);
			for (ii=0; ii<num_of_char; ii=ii+1)
				begin
					tx_data <= ram[ii];
					tx_data_valid <= 1'b1;
					@(posedge r0_Clock);
				end

			tx_data_valid <= 1'b0;
		   
		end
	
	
	PUR PUR_INST(.PUR(1'b1));
	GSR GSR_INST(.GSR(1'b1));

endmodule   

