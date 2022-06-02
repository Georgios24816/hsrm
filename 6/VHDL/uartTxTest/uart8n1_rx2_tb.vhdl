library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity uart8n1_rx2_tb is
    port 
    (
        led0        :out     unsigned(0 downto 0); 
        led1        :out     unsigned(0 downto 0);
        led2        :out     unsigned(0 downto 0);
        led3        :out     unsigned(0 downto 0);
        ledGreen    :out     unsigned(0 downto 0);

        hwclk     : in      std_ulogic;
        ftdi_rx   : in     unsigned(0 downto 0)
    );
end uart8n1_rx2_tb;

architecture test of uart8n1_rx2_tb is
    component uart8n1_rx2
        port 
        (
            clk     : in    std_ulogic;
            en      : in    unsigned(0 downto 0);
            rxIn    : in    unsigned(0 downto 0);
    
            rxDv    : out   unsigned(0 downto 0);
            isIdle  : out   unsigned(0 downto 0);
            rxByte  : out   unsigned(7 downto 0)
        );
    end component;

    component uart8n1_tx
    port 
        (
            clk     : in    std_ulogic;
            en      : in    unsigned(0 downto 0);
            txByte  : in    unsigned(7 downto 0);
            sendData: in    unsigned(0 downto 0);
    
            isIdle  : out   unsigned(0 downto 0);
            txDone  : out   unsigned(0 downto 0);
            tx      : out   unsigned(0 downto 0)
        );
    end component;

signal clk_9600     : std_ulogic            := '0';
signal uart_txbyte  : unsigned(7 downto 0)  := "00000000";
signal uart_send    : unsigned(0 downto 0)  := "0";
signal uart_txed    : unsigned(0 downto 0)  := "0";
signal uart_tx      : unsigned(0 downto 0)  := "0";
signal cntr_9600    : unsigned(31 downto 0) := (others => '0');
signal period9600   : unsigned(31 downto 0) := to_unsigned(625, 32);

signal ledGreenSignal : unsigned(0 downto 0) := "0";
signal cntr_32    : unsigned(31 downto 0) := (others => '0');

signal rxInSignal : unsigned(0 downto 0) := "0";
signal rxDvSignal : unsigned(0 downto 0) := "0";

signal led0Signal : unsigned(0 downto 0) := "0";
signal isIdleSignal : unsigned(0 downto 0);

signal uartInstanceEn : unsigned(0 downto 0) := "1";
signal txInstanceEn : unsigned(0 downto 0) := "0";
signal txInstanceSendData : unsigned(0 downto 0) := "0";
signal txInstanceIsIdle : unsigned(0 downto 0);
signal txInstanceTxDone : unsigned(0 downto 0);
signal txInstanceTx : unsigned(0 downto 0);

begin
    led2 <= isIdleSignal;
    led3 <= isIdleSignal;
    ledGreen <= ledGreenSignal;

    uartInstance: uart8n1_rx2 
    port map
    (
        clk => hwclk,
        en => uartInstanceEn,
        rxIn => ftdi_rx,
        rxDv => rxDvSignal,
        isIdle => isIdleSignal,
        rxByte => uart_txbyte
    );

    txInstance: uart8n1_tx
    port map
    (
        clk => clk_9600,
        en => txInstanceEn,
        txByte => to_unsigned(48, 8),
        sendData => txInstanceSendData,
        isIdle => txinstanceIsIdle,
        txDone => txInstanceTxDone,
        tx => txInstanceTx
    );

    process (hwclk)
    begin
        if rising_edge(hwclk) then
            cntr_9600 <= cntr_9600 + 1;
            cntr_32 <= cntr_32 + 1;

            if (cntr_9600 = period9600) then
                clk_9600 <= not clk_9600;
                cntr_9600 <= to_unsigned(0, 32);
            end if;

            if (ftdi_rx = "1" and isIdleSignal = "1") then
                led1 <= "1";
                uartInstanceEn <= "1";
                txInstanceEn <= "0";
            
            elsif (txinstanceIsIdle = "1" and txInstanceTxDone = "1") then
                uartInstanceEn <= "0";
                txInstanceEn <= "1"; 
                led0 <= "1";

            end if;

            --if (uart_txbyte = to_unsigned(48, 8)) then
            --    ledGreenSignal <= "1";
            --else
            --    ledGreenSignal <= "0";
            --end if;

        end if;
    end process;

end test;