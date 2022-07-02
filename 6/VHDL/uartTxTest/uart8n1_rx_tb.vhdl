library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity uart8n1_rx_tb is
    port 
        (
            led0       :out     unsigned(0 downto 0); 
            led1       :out     unsigned(0 downto 0);
            led2       :out     unsigned(0 downto 0);
            led3       :out     unsigned(0 downto 0);
            ledGreen       :out     unsigned(0 downto 0);

            hwclk     : in      std_ulogic;
            ftdi_tx   : out     unsigned(0 downto 0)
        );
end uart8n1_rx_tb;

architecture test of uart8n1_tx_tb is
    component uart8n1_tx
        port 
        (
            clk     : in    std_ulogic;
            txByte  : in    unsigned(7 downto 0);
            sendData: in    unsigned(0 downto 0);
    
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

begin
    led0 <= "0";
    led1 <= "0";
    led2 <= "0";
    led3 <= "0";
    ledGreen <= ledGreenSignal;

    uartInstance: uart8n1_tx 
    port map
    (
        clk => clk_9600,
        txByte => to_unsigned(48, 8),
        sendData => "1",
        txDone => uart_txed,
        tx => ftdi_tx
    );

    process (hwclk)
    begin
        if rising_edge(hwclk) then
            cntr_9600 <= cntr_9600 + 1;
            cntr_32 <= cntr_32 + 1;

            if (cntr_9600 = period9600) then
                clk_9600 <= not clk_9600;
                cntr_9600 <= to_unsigned(0, 32);

            elsif (cntr_32 = to_unsigned(12000000, 32)) then
                ledGreenSignal <= not ledGreenSignal;
                cntr_32 <= to_unsigned(0, 32);
            end if;
        end if;
    end process;

end test;