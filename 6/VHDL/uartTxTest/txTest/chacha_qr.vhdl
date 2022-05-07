library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity chacha_qr is
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
end chacha_qr;

architecture behave of chacha_qr is
signal a2Signal: unsigned(31 downto 0);
signal b2Signal: unsigned(31 downto 0);
signal c2Signal: unsigned(31 downto 0);
signal d2Signal: unsigned(31 downto 0);

begin
    a2 <= a2Signal;
    b2 <= b2Signal;
    c2 <= c2Signal;
    d2 <= d2Signal;

    process (clk)
        variable state: unsigned(1 downto 0) := "00";
        variable index: unsigned(4 downto 0) := "00000";
    begin
        if (rising_edge(clk)) then
            if (state = "00") then
                a2Signal <= a + b;
                b2Signal <= b;
                c2Signal <= c;
                d2Signal <= (d xor a2Signal) sll 16;

                done <= "0";
                state := "01";

            elsif (state = "01") then
                --c += d; b ^= c; b <<<= 12;
                c2Signal <= c2Signal + d2Signal;
                b2Signal <= (b2Signal xor c2Signal) sll 12;
                state := "10";

            elsif (state = "10") then
                --a += b; d ^= a; d <<<= 8;
                a2Signal <= a2Signal + b2Signal;
                d2Signal <= (d2Signal xor a2Signal) sll 8;
                state := "11";

            else
                --c += d; b ^= c; b <<<= 7;
                c2Signal <= c2Signal + d2Signal;
                b2Signal <= (b2Signal xor c2Signal) sll 7;

                state := "00";
            end if;

            if (index = "10100" and state = "11") then
                done <= "1";
                index := "00000";

            else
                index := index + 1;
            end if;

        end if;
    end process;
end behave;