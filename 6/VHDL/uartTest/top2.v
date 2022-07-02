module top(
        output led0, 
        output led1,
        output led2,
        output led3,
        output ledGreen ,
        output ftdi_tx,

        input hwclk,
        input ftdi_rx,
    );
    reg rxDvSignal;
    reg isIdleSignal;
    reg [7:0] uart_rxbyte;
    reg rxEnSignal = 0;

    uart8n1_rx2 tarns(
        .clk (hwclk),
        .en (rxEnSignal),
        .rxIn (ftdi_rx),
        .xDv (rxDvSignal),
        .isIdle (isIdleSignal),
        .rxByte (uart_rxbyte)
    );


    parameter ASCII_0 = 8'd48;
    parameter ASCII_9 = 8'd57;
    reg [7:0] uart_txbyte = ASCII_0;
    reg uart_send = 0;
    reg uart_txed; 
    reg clk_9600 = 0;
    reg [31:0] cntr_9600 = 32'b0;
    parameter period_9600 = 625;

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

	.debugLed(led3)
    );

    reg clk_1 = 0;
    reg [31:0] cntr_1 = 32'b0;
    reg pulse = 0;
    reg receivedByte = 0;
    parameter period_1 = 6000000;


    reg [3 : 0] UartStateCounter = 15;

    always @ (posedge hwclk ) begin
	    pulse <= !pulse;

            cntr_9600 <= cntr_9600 + 1;
            cntr_32 <= cntr_32 + 1;
            cntr_1 <= cntr_1 + 1;

            if (cntr_9600 == period9600) begin
                clk_9600 <= !clk_9600;
                cntr_9600 <= 0;
            end;

            if (cntr_1 == period_1) begin
                clk_1 <= ~clk_1;
                cntr_1 <= 32'b0;
            end
	    
	    /*
	    if (UartStateCounter == 0) begin
		if (isIdleSignal == 1 && uart_txed == 1) begin
			if (ftdi_rx == 0) begin
				rxEnSignal <= 1;
				receivedByte <= 0;

				UartStateCounter <= 1;
			end 
		end

	    end else if (UartStateCounter == 1) begin
			if (ftdi_rx == 1 && isIdleSignal == 1 && uart_txed == 1) begin
				rxEnSignal <= 0;
				receivedByte <= 0;
				
				if (uart_rxbyte == 48) begin
					UartStateCounter <= 2;
					ledGreen <= ~ledGreen;
				end else begin
					UartStateCounter <= 0;
				end
			end

  	    end else if (UartStateCounter == 2) begin
			uart_send <= 1;
			UartStateCounter <= 3;

  	    end else if (UartStateCounter == 3) begin
			uart_send <= 0;
		    if (uart_txed == 1 && isIdleSignal == 1) begin
		        //led3 <= ~led3;
		        UartStateCounter <= 0;
		    end

  	    end else if (UartStateCounter == 15) begin
			led3 <= 0;
			ledGreen <= 0;
			UartStateCounter <= 0;
	    end
	    */

    end
	    /*
	    	if (isIdleSignal == 1 && uart_txed == 1) begin
			if (ftdi_rx == 0) begin
				rxEnSignal = 1;

			end else if (ftid_rx == 1) begin
				rxEnSignal <= 0;
				
				if (uart_rxbyte != 0 && receivedByte == 0) begin
					led3 <= 1;
					receivedByte <= 0;
				end

			end else begin
				senddata <= 1;
			end

	    	end else if (isIdleSignal == 0) begin
			led3 <= 0;
			receivedByte <= 1;

		end else if (uart_txed == 0) begin
			senddata <= 0;
			receivedByte <= 0;
		end
		*/

	//	end
	    	/*
		//if (ftdi_rx == 1 && ftdi_tx == 0 && txdone == 1) begin
		if (pulse == 0 && isIdleSignal == 1 && uart_txed == 1) begin
			rxEnSignal = 1;
			led3 <= 1;
			led0 <= 0;
            		if (uart_rxbyte == 48) begin
            		    ledGreen <= 1;

            		end else begin
            		    ledGreen <= 0;
            		end;

		//end else if (ftdi_tx == 1 && ftdi_rx == 0 && isIdleSignal == 0) begin
		end else if (pulse == 1 && isIdleSignal == 1 && uart_txed == 1) begin
			senddata <= 1;
			rxEnSignal <= 0;
			led3 <= 0;
			led0 <= 1;
		end
		*/

    endmodule
