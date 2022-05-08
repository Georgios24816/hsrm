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
signal a0Signal: unsigned(31 downto 0);
signal b0Signal: unsigned(31 downto 0);
signal c0Signal: unsigned(31 downto 0);
signal d0Signal: unsigned(31 downto 0);

signal a1Signal: unsigned(31 downto 0);
signal b1Signal: unsigned(31 downto 0);
signal c1Signal: unsigned(31 downto 0);
signal d1Signal: unsigned(31 downto 0);

signal a2Signal: unsigned(31 downto 0);
signal b2Signal: unsigned(31 downto 0);
signal c2Signal: unsigned(31 downto 0);
signal d2Signal: unsigned(31 downto 0);

signal a3Signal: unsigned(31 downto 0);
signal b3Signal: unsigned(31 downto 0);
signal c3Signal: unsigned(31 downto 0);
signal d3Signal: unsigned(31 downto 0);

signal a4Signal: unsigned(31 downto 0);
signal b4Signal: unsigned(31 downto 0);
signal c4Signal: unsigned(31 downto 0);
signal d4Signal: unsigned(31 downto 0);

begin
    a2 <= a4Signal;
    b2 <= b4Signal;
    c2 <= c4Signal;
    d2 <= d4Signal;
    done <= "1";
    
    process (clk)
    begin
        a0Signal <= a + b;
        d0Signal <= d xor a0Signal;
        d1Signal <= d0Signal(15 downto 0) & d0Signal(31 downto 16);
        c0Signal <= c + d1Signal;
        b0Signal <= b xor c0Signal;
        b1Signal <= b0Signal(19 downto 0) & b0Signal(31 downto 20);
        a1Signal <= a0Signal + b1Signal;
        d2Signal <= d1Signal xor a1Signal;
        d3Signal <= d2Signal(23 downto 0) & d2Signal(31 downto 24);
        c1Signal <= c0Signal + d3Signal;
        b2Signal <= b1Signal xor c1Signal;
        b3Signal <= b2Signal(24 downto 0) & b2Signal(31 downto 25);
    end process;
    
end behave;