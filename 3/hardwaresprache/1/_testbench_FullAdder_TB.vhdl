library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library std;
use std.textio.all;

entity FullAdder_TB is
end FullAdder_TB;

architecture Behavioral of FullAdder_TB is

  -- Width of adder
  constant adderWidth : natural := 8;

  -- Clock
  signal clk : std_logic;
  
  -- Inputs
  signal a   : std_logic;
  signal b   : std_logic;
  signal cin : std_logic := '0';

  -- Outputs
  signal s    : std_logic;
  signal cout : std_logic;

  -- Clock period definitions
  constant clk_period : time := 10 ns;

begin

  -- Instantiate the Unit Under Test (UUT)
  uut : entity work.FullAdder(Structural)
    port map (a    => a,
              b    => b,
              cin  => cin,
              s    => s,
              cout => cout);

  -- Clock process definitions
  clk_process : process
  begin
    clk <= '0';
    wait for clk_period/2;
    clk <= '1';
    wait for clk_period/2;
  end process;

  -- Stimulus process
  stim_proc : process

    -- Text I/O
    variable lineBuffer : line;

  begin

    -- Wait for the first rising edge
    wait until rising_edge(clk);

    -- Print the top element
    write(lineBuffer, string'("Start simulation: "));
    writeline(output, lineBuffer);

    -- Set a stimulus
    a <= '1';
    b <= '1';
      
    -- Wait 10ns
    wait for 10 ns;

   -- Set another simulus
   a <= '0';
   b <= '1';

   -- Wait 10ns
   wait for 10 ns;
      
    -- Simply wait forever to terminate the simulation
    wait;
  end process;

end architecture;
