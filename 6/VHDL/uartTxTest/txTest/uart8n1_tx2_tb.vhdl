library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity uart8n1_tx2_tb is
    port 
    (
        hwclk     : in      std_ulogic;
        ledGreen  : out     unsigned(0 downto 0);
        led0      : out     unsigned(0 downto 0);
        ftdi_tx   : out     unsigned(0 downto 0)
    );
end uart8n1_tx2_tb;

architecture test of uart8n1_tx2_tb is
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

    component chacha_qr
        port 
        (
            clk : in    std_ulogic;
    
            a   : in    unsigned(31 downto 0);
            b   : in    unsigned(31 downto 0);
            c   : in    unsigned(31 downto 0);
            d   : in    unsigned(31 downto 0);
    
            a2  : out   unsigned(31 downto 0);
            b2  : out   unsigned(31 downto 0);
            c2  : out   unsigned(31 downto 0);
            d2  : out   unsigned(31 downto 0);
    
            done: out   unsigned(0 downto 0)
        );
    end component;

signal clk_9600     : std_ulogic            := '0';
signal uart_txbyte  : unsigned(7 downto 0)  := to_unsigned(0, 8);
signal uart_send    : unsigned(0 downto 0)  := "0";
signal uart_txed    : unsigned(0 downto 0)  := "0";
signal uart_tx      : unsigned(0 downto 0)  := "0";
signal cntr_9600    : unsigned(31 downto 0) := (others => '0');
signal period9600   : unsigned(31 downto 0) := to_unsigned(625, 32);

signal cntr_32    : unsigned(31 downto 0) := (others => '0');
signal txidleSignal : unsigned(0 downto 0);


signal a    : unsigned(31 downto 0) := to_unsigned(0, 32);
signal b    : unsigned(31 downto 0) := to_unsigned(0, 32);
signal c    : unsigned(31 downto 0) := to_unsigned(0, 32);
signal d    : unsigned(31 downto 0) := to_unsigned(0, 32);
signal a2   : unsigned(31 downto 0);
signal b2   : unsigned(31 downto 0);
signal c2   : unsigned(31 downto 0);
signal d2   : unsigned(31 downto 0);

signal done : unsigned(0 downto 0);
signal txDoneSignal : unsigned(0 downto 0);
--signal a3 : unsigned(31 downto 0) := "10101010101110111100110011011101";

--signal hwclk : std_logic := '0';
signal uart8n1_tx_en : unsigned(0 downto 0) := "1";

begin
    --hwclk <= not hwclk after 10 ns;

    uartInstance: uart8n1_tx 
    port map
    (
        clk => clk_9600,
        en => uart8n1_tx_en,
        txByte => uart_txbyte,
        sendData => "1",
        isIdle => txidleSignal,
        txDone => txDoneSignal,
        tx => ftdi_tx
    );

    chacha_qrInstance: chacha_qr
    port map
    (
        clk => hwclk,
        a => to_unsigned(1, 32),
        b => b,
        c => c,
        d => d,

        a2 => a2,
        b2 => b2,
        c2 => c2,
        d2 => d2,
        done => done
    );

    process (hwclk)
        variable state: unsigned(2 downto 0) := "000";
        variable counter: integer := 0;
    begin
        if rising_edge(hwclk) then
            ledGreen <= "0";
            cntr_9600 <= cntr_9600 + 1;
            cntr_32 <= cntr_32 + 1;

            if (cntr_9600 = period9600) then
                clk_9600 <= not clk_9600;
                cntr_9600 <= to_unsigned(0, 32);
            end if;

            --uart_txbyte <= to_unsigned(50, 8);
            --if (txidleSignal = "1" and done = "1") then
            --    uart_txbyte <= "00110011";
            --end if;

            if ((txDoneSignal = "1" and txidleSignal = "1") and done = "1") then
                if (state = "000") then
                    --uart8n1_tx_en <= "1";
                    led0 <= "0";
                    uart_txbyte <= to_unsigned(48, 8);--a2(7 downto 0) xor b2(7 downto 0) xor c2(7 downto 0) xor d2(7 downto 0);
                    state := "001";
                    report "The value of 'uart_txbyte' is " & integer'image(to_integer(uart_txbyte));

                elsif (state = "001") then
                    uart_txbyte <= to_unsigned(51, 8);--a2(15 downto 8) xor b2(15 downto 8) xor c2(15 downto 8) xor d2(15 downto 8);
                    state := "000";
                    report "The value of 'uart_txbyte' is " & integer'image(to_integer(uart_txbyte));
--
                --elsif (state = "010") then
                --    uart_txbyte <= to_unsigned(52, 8);--a2(23 downto 16) xor b2(23 downto 16) xor c2(23 downto 16) xor d2(23 downto 16);
                --    state := "011";
                --    report "The value of 'uart_txbyte' is " & integer'image(to_integer(uart_txbyte));
--
                --elsif (state = "011") then
                --    uart_txbyte <= to_unsigned(53, 8);--a2(31 downto 24) xor b2(31 downto 24) xor c2(31 downto 24) xor d2(31 downto 24);
                --    state := "111";
                --    report "The value of 'uart_txbyte' is " & integer'image(to_integer(uart_txbyte));
                --    report "end" severity failure;              
                end if;
            end if;
        end if;
    end process;

end test;