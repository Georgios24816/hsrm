---------------------------------------------------------------------------------------------------
--
-- Titel:    Uebung 7
-- Autor:    Georgios Markou
-- Datum:    10. Januar 2020
--
---------------------------------------------------------------------------------------------------

------------------------------------------
--ENTITY Register_RE----------------------
library ieee;
	use ieee.std_logic_1164.all;
	use ieee.numeric_std.all;

library work;
	use work.all;
	
entity Register_RE is -- Register rising edge = D Flip Flop
	generic (WIDTH : integer := 8); -- Datenbreite
	port (
		clk	: in	std_logic; 
		ena	: in	std_logic; -- write Enable

		d	: in	std_logic_vector(WIDTH - 1 downto 0);

		q	: out	std_logic_vector(WIDTH - 1 downto 0)
	);
end Register_RE;

architecture rtl of Register_RE is
begin
	registerP: process (clk)
	begin
		if (rising_edge(clk) and ena = '1') then
			q <= d;
		end if;
	end process;
end rtl;
------------------------------------------
--END ENTITY Register_RE------------------

------------------------------------------
--ENTITY Multiplexer21--------------------
library ieee;
	use ieee.std_logic_1164.all;
	use ieee.numeric_std.all;

library work;
	use work.all;

entity Multiplexer21 is
	generic (WIDTH : integer := 8);
	port (
		in1	: in	std_logic_vector(WIDTH - 1 downto 0);
		in0	: in	std_logic_vector(WIDTH - 1 downto 0);
		sel	: in	std_logic;

		res	: out	std_logic_vector(WIDTH - 1 downto 0)
	);
end Multiplexer21;

architecture rtl of Multiplexer21 is
begin
	res <= in1 when (sel = '1') else in0;
end rtl;
------------------------------------------
--END ENTITY Multiplexer21----------------

------------------------------------------
--ENTITY Multiplexer31--------------------
library ieee;
	use ieee.std_logic_1164.all;
	use ieee.numeric_std.all;

library work;
	use work.all;

entity Multiplexer31 is
	generic (WIDTH : integer := 8);
	port (
		in2	: in	std_logic_vector(WIDTH - 1 downto 0);
		in1	: in	std_logic_vector(WIDTH - 1 downto 0);
		in0	: in	std_logic_vector(WIDTH - 1 downto 0);
		sel	: in	std_logic_vector(1 downto 0);

		res	: out	std_logic_vector(WIDTH - 1 downto 0)
	);
end Multiplexer31;

architecture rtl of Multiplexer31 is
begin
	res <= 	in2 when (sel = "10") else
		in1 when (sel = "01") else
		in0;
end rtl;
------------------------------------------
--END ENTITY Multiplexer31----------------

------------------------------------------
--ENTITY Increment------------------------
library ieee;
	use ieee.std_logic_1164.all;
	use ieee.numeric_std.all;

library work;
	use work.all;

entity Increment is
	generic (WIDTH : integer := 8);
	port (
		in1	: in	std_logic_vector(WIDTH - 1 downto 0);

		res	: out	std_logic_vector(WIDTH - 1 downto 0)
	);
end Increment;

architecture rtl of Increment is
begin
	res <= std_logic_vector(unsigned(in1) + 1); 
end rtl;
------------------------------------------
--END ENTITY Increment--------------------

------------------------------------------
--ENTITY CustomMuxSel---------------------
library ieee;
	use ieee.std_logic_1164.all;
	use ieee.numeric_std.all;

library work;
	use work.all;

entity CustomMuxSel is
	port (
		clk	: in	std_logic;
		
		pccontr	: in	std_logic_vector(5 downto 0);
		ov	: in	std_logic;
		zero	: in 	std_logic;
		
		selPC	: out	std_logic_vector(1 downto 0)
	);
end CustomMuxSel;

architecture rtl of CustomMuxSel is
alias bnez : std_logic is pccontr(0);
alias beqz : std_logic is pccontr(1);
alias bov  : std_logic is pccontr(2);
alias reti : std_logic is pccontr(3); 
alias jreg	 : std_logic is pccontr(4);
alias jal  : std_logic is pccontr(5);	
begin
	selPc <="10" when (reti = '1') else
		"01" when (((ov and ov) or (beqz and zero) or (bnez and (not zero)) or jal or jreg) = '1') else
		"00";
end rtl;
------------------------------------------
--END ENTITY CustomMuxSel-----------------

library ieee;
	use ieee.std_logic_1164.all;
	use ieee.numeric_std.all;

library work;
	use work.all;
	
entity pclogic is
	port (
		-- common
		clk 		: in  std_logic;
		reset		: in  std_logic;
		
		-- control flags
		pcwrite		: in  std_logic;
		pccontr		: in  std_logic_vector(5 downto 0);
		ov		: in  std_logic;
		zero		: in  std_logic;
		intr		: in  std_logic;
		
		-- program counter inputs
		pcnew		: in  std_logic_vector(11 downto 0);
		isra		: in  std_logic_vector(11 downto 0);
		isrr		: in  std_logic_vector(11 downto 0);

		-- program counter outputs
		pcakt		: out std_logic_vector(11 downto 0);
		pcinc		: out std_logic_vector(11 downto 0);
		pcnext		: out std_logic_vector(11 downto 0)
	);
end pclogic;

architecture rtl of pclogic is
signal PCREGres	: std_logic_vector(11 downto 0);
signal IRQMUXres: std_logic_vector(11 downto 0);
signal PCMUXres : std_logic_vector(11 downto 0);
signal INCres	: std_logic_vector(11 downto 0);
signal MUXSELres: std_logic_vector(1 downto 0);

begin
	PCREG: entity work.Register_RE
	generic map (WIDTH => 12)
	port map (
		clk => clk,
		ena => pcwrite,
		d => IRQMUXres,
		
		q => PCREGres
	);

	IRQMUX: entity work.Multiplexer21
	generic map (WIDTH => 12)
	port map (
		in1 => isra,
		in0 => PCMUXres,
		sel => intr,

		res => IRQMUXres
	);

	PCMUX: entity work.Multiplexer31
	generic map (WIDTH => 12)
	port map (
		in2 => isrr,
		in1 => pcnew,
		in0 => INCres,
		sel => MUXSELres,

		res => PCMUXres
	);

	INC: entity work.Increment
	generic map (WIDTH => 12)
	port map (
		in1 => PCREGres,

		res => INCres
	);

	MUXSEL: entity work.CustomMuxSel
	port map (
		clk => clk,
		pccontr => pccontr,
		ov => ov,
		zero => zero,

		selPc => MUXSELres
	);

	process (clk)
	begin
		if (rising_edge(clk)) then
			pcakt <= PCREGres;
			pcinc <= INCres;
			pcnext <= PCMUXres; 
		end if;

		if (reset = '1') then
			pcakt  <= "000000000000";
			pcinc  <= "000000000001";
			pcnext <= "000000000001"; 
		end if;
	end process;

end rtl;

