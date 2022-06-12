/* Top level module for keypad + UART demo */
module top (
    // input hardware clock (12 MHz)
    hwclk, 
    // all LEDs
    //led1, led0, ledGreen, led3, led2, 
    // UART lines
    ftdi_tx, //ftdi_rx
    );

    /* Clock input */
    input hwclk;

    /* LED outputs 
    output led0;
    output led1;
    output led2;
    output led3;
    output ledGreen;
	*/

    /* FTDI I/O */
    output ftdi_tx;
    //input ftdi_rx;

    /* 9600 Hz clock generation (from 12 MHz) */
    reg clk_9600 = 0;
    reg [31:0] cntr_9600 = 32'b0;
    parameter period_9600 = 625;

    /* 1 Hz clock generation (from 12 MHz) */
    reg clk_1 = 0;
    reg [31:0] cntr_1 = 32'b0;
    parameter period_1 = 6000000;

    // Note: could also use "0" or "9" below, but I wanted to
    // be clear about what the actual binary value is.
    parameter ASCII_0 = 8'd48;
    parameter ASCII_9 = 8'd57;

    /* UART registers */
    reg [7:0] uart_txbyte = ASCII_0;
    reg uart_send = 0;
    wire uart_txed;
	reg uart_send_en = 0;
    /* LED register */
    reg ledval = 0;

    /////
    
    reg [3 : 0] UartStateCounter = 15;
    reg receivedByte = 0;

    reg rxDvSignal;
    reg isIdleSignal = 1;
    reg [7:0] uart_rxbyte;
    reg rxEnSignal = 0;

    //uart8n1_rx2 tarns(
    //    .clk (hwclk),
    //    .en (rxEnSignal),
    //    .rxIn (ftdi_rx),
    //    .xDv (rxDvSignal),
    //    .isIdle (isIdleSignal),
    //    .rxByte (uart_rxbyte)
    //);
    //

    /* UART transmitter module designed for
       8 bits, no parity, 1 stop bit. 
    */
    uart_tx_8n1 transmitter (
        // 9600 baud rate clock
        .clk (clk_9600),
        // byte to be transmitted
        .txbyte (uart_txbyte),
        // trigger a UART transmit on baud clock
        .senddata (uart_send),
        // input: tx is finished
        .txdone (uart_txed),
        // output UART tx pin
        .tx (ftdi_tx),
	    .en (uart_send_en)
	    //.debugLed(led2)
    );

    reg chachaStart = 0;
    reg [255 : 0] chachaKey = 256'h657870616e642033322d62797465206b657870616e642033322d62797465206b;
    reg [63 : 0] chachaIndex = 0;
    reg [63 : 0] chachaNonce = 64'h6578706164312887;
    wire chachaDone;
    wire [511 : 0] chachaOut;

    chacha20 chacha (
        .clock(hwclk),
        .start(chachaStart),
        .key(chachaKey),
        .index(chachaIndex),
        .nonce(chachaNonce),
        .done(chachaDone),
        .out(chachaOut)
    );
    
    reg [511 : 0] TestReg;

    /* Wiring */
    
    /* Low speed clock generation */
    always @ (posedge hwclk) begin
        /* generate 9600 Hz clock */
        cntr_9600 <= cntr_9600 + 1;
        if (cntr_9600 == period_9600) begin
            clk_9600 <= ~clk_9600;
            cntr_9600 <= 32'b0;
            chachaStart <= 0;
        end

        /* generate 1 Hz clock */
        cntr_1 <= cntr_1 + 1;
        if (cntr_1 == period_1) begin
            clk_1 <= ~clk_1;
            cntr_1 <= 32'b0;
        end

        if (UartStateCounter == 0) begin

		    if (uart_txed == 1 && chachaDone == 1) begin
				chachaStart <= 0;
				UartStateCounter <= 1;
                	//led1 <= 1;
			end 

	    end else if (UartStateCounter == 1) begin
			if (uart_txed == 1) begin
				UartStateCounter <= 2;
				uart_txbyte <= chachaOut[7 : 0];
			end

        //begin transmission
  	    end else if (UartStateCounter == 2) begin
		    uart_send_en <= 1;
		    uart_send <= 1;
		    UartStateCounter <= 3;

        //wait for transmission to start
	    end else if (UartStateCounter == 3) begin
		    if (uart_txed == 0) begin
			    UartStateCounter <= 4;
		    end

        //wait for transmission to end
  	    end else if (UartStateCounter == 4) begin
		    if (uart_txed == 1) begin
			    uart_send_en <= 0;
			    uart_send <= 0;
		        //led3 <= ~led3;
		        UartStateCounter <= 0;
		    end

        //init leds
	    end else if (UartStateCounter == 15) begin
			/*
			led3 <= 0;
			led0 <= 0;
			led1 <= 0;
			led2 <= 0;
			ledGreen <= 0;
			*/
            		chachaStart <= 1;
			UartStateCounter <= 0;
	    end

end

endmodule
