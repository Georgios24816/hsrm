// --------------------------------------------------------------------
// >>>>>>>>>>>>>>>>>>>>>>>>> COPYRIGHT NOTICE <<<<<<<<<<<<<<<<<<<<<<<<<
// --------------------------------------------------------------------
// Copyright (c) 2006 by Lattice Semiconductor Corporation
// --------------------------------------------------------------------
//
// Permission:
//
//   Lattice Semiconductor grants permission to use this code for use
//   in synthesis for any Lattice programmable logic product.  Other
//   use of this code, including the selling or duplication of any
//   portion is strictly prohibited.
//
// Disclaimer:
//
//   This VHDL or Verilog source code is intended as a design reference
//   which illustrates how these types of functions can be implemented.
//   It is the user's responsibility to verify their design for
//   consistency and functionality through the use of formal
//   verification methods.  Lattice Semiconductor provides no warranty
//   regarding the use or functionality of this code.
//
// --------------------------------------------------------------------
//           
//                     Lattice Semiconductor Corporation
//                     5555 NE Moore Court
//                     Hillsboro, OR 97214
//                     U.S.A
//
//                     TEL: 1-800-Lattice (USA and Canada)
//                          503-268-8001 (other locations)
//
//                     web: http://www.latticesemi.com/
//                     email: techsupport@latticesemi.com
// --------------------------------------------------------------------
//
// Revision History :


// UART TX over IrDA on iCEstick

module uart (
         
         input wire   clk_in        ,
         input wire   from_pc       , 
         output wire  to_ir         ,
         output wire  sd            ,
         input wire   i_serial_data ,
         output wire  o_serial_data ,
         
         output       test1         ,
         output       test2         ,
         output       test3         ,
         output [7:0] led   
         );

// parameters (constants)
parameter clk_freq = 27'd12000000;  // in Hz for 12MHz clock

reg [26:0]  rst_count ;
wire        i_rst ;
wire        CLKOP ;
wire        CLKOS ;        

wire [7:0]  o_rx_data       ; 
wire        o_rx_data_ready ;

wire [7:0]  i_tx_data       ;
wire        i_start_tx      ;
 
// internal reset generation
always @ (posedge clk_in)
    begin
        if (rst_count >= (clk_freq/2)) begin
        end else                       
            rst_count <= rst_count + 1;
    end

assign i_rst = ~rst_count[19] ;

// PLL instantiation
ice_pll ice_pll_inst(
     .REFERENCECLK ( clk_in        ),  // input 12MHz
     .PLLOUTCORE   ( CLKOP         ),  // output 38MHz
     .PLLOUTGLOBAL ( PLLOUTGLOBAL  ),
     .RESET        ( 1'b1  )
     );

reg [5:0] clk_count ; 
reg CLKOS ;

always @ (posedge CLKOP) begin
    if ( clk_count == 9 ) clk_count <= 0 ;
    else clk_count <= clk_count + 1 ;          
    end

always @ (posedge CLKOP) begin
    if ( clk_count == 9 ) CLKOS <= ~CLKOS ;    
    end

// UART RX instantiation
uart_rx_fsm uut1 (                   
     .i_clk                 ( CLKOP           ),
     .i_rst                 ( i_rst           ),
     .i_rx_clk              ( CLKOS           ),
     .i_start_rx            ( 1'b1            ),
     .i_loopback_en         ( 1'b0            ),
     .i_parity_even         ( 1'b0            ),
     .i_parity_en           ( 1'b0            ),               
     .i_no_of_data_bits     ( 2'b10           ),  
     .i_stick_parity_en     ( 1'b0            ),
     .i_clear_linestatusreg ( 1'b0            ),               
     .i_clear_rxdataready   ( 1'b0            ),
     .o_rx_data             ( o_rx_data       ), 
     .o_timeout             (                 ),               
     .bit_sample_en         ( bit_sample_en   ), 
     .o_parity_error        (                 ),
     .o_framing_error       (                 ),
     .o_break_interrupt     (                 ),               
     .o_rx_data_ready       ( o_rx_data_ready ),
     .i_int_serial_data     (                 ),
     .i_serial_data         ( from_pc         ) // from_pc UART signal
    );

reg [3:0] count ;
reg [15:0] shift_reg1 ;
reg [19:0] shift_reg2 ;

always @ (posedge CLKOS) count <= count + 1 ;
always @ (posedge CLKOP) shift_reg2[19:0] <= {shift_reg2[18:0], o_rx_data_ready} ; 
always @ (posedge CLKOS) shift_reg1[15:0] <= {shift_reg1[14:0], rx_rdy} ; 
assign rx_rdy = |shift_reg2 ;
assign i_start_tx = |shift_reg1 ;
assign i_tx_data = {o_rx_data[7:6], ~o_rx_data[5], o_rx_data[4:0]} ; // o_rx_data

// UART TX instantiation
uart_tx_fsm uut2(                                
    .i_clk                 ( count[3]         ),   
    .i_rst                 ( i_rst         ),   
    .i_tx_data             ( i_tx_data     ),   
    .i_start_tx            ( i_start_tx    ),   
    .i_tx_en               ( 1'b1          ),   
    .i_tx_en_div2          ( 1'b0          ),   
    .i_break_control       ( 1'b0          ),   
    .o_tx_en_stop          (  ),                
    .i_loopback_en         ( 1'b0          ),   
    .i_stop_bit_15         ( 1'b0          ),   
    .i_stop_bit_2          ( 1'b0          ),   
    .i_parity_even         ( 1'b0          ),   
    .i_parity_en           ( 1'b0          ),   
    .i_no_of_data_bits     ( 2'b11         ),   
    .i_stick_parity_en     ( 1'b0          ),   
    .i_clear_linestatusreg ( 1'b0          ),   
    .o_tsr_empty           (  ),                
    .o_int_serial_data     (  ),                
    .o_serial_data         ( o_serial_data )    
    );                                          

// LED display ASCII code
 assign led = ~i_tx_data ;                      

 reg [4:0] ir_tx_reg ;  
 wire ir_tx ;
 
 assign sd = 0 ;  // 0: enable  
 always @ (posedge CLKOP) ir_tx_reg[4:0] <= {ir_tx_reg[3:0], bit_sample_en} ; 
 assign ir_tx = |ir_tx_reg ;
 assign to_ir = ir_tx & ~from_pc ;
  
 // debug
 assign test1 =  to_ir ;
 assign test2 =  from_pc ;
 assign test3 =  i_rst ; 
  
endmodule





