---------------------------------------------------------------------------------------------------
--
-- Titel:    
-- Autor:    
-- Datum:    
--
---------------------------------------------------------------------------------------------------

library ieee;
	use ieee.std_logic_1164.all;
	use ieee.numeric_std.all;
library xbus_common;
	use xbus_common.xtoolbox.all;
library work;
	use work.all;
	use work.hadescomponents.all;
	
entity mux2_1 is
	generic (WIDTH: integer := 32);
    port ( sel : in  std_logic;
           a   : in  std_logic_vector(WIDTH - 1 downto 0);
           b   : in  std_logic_vector(WIDTH - 1 downto 0);
		   
           res : out std_logic_vector(WIDTH - 1 downto 0);
		 );
end mux2_1;

architecture Behavioral of mux2_1 is
begin
    res <= b when (sel = '1') else a;
end Behavioral;
	
entity FlipFlop is
	generic (WIDTH: integer := 8);
	
	port(
		clk 	: in 	std_logic;  
		reset	: in 	std_logic;  
		d 		: in  	std_logic_vector(WIDTH - 1 downto 0);    
		
		q 		: out 	std_logic_vector(WIDTH - 1 downto 0)
	);
end FlipFlop;

architecture Behavioral of FlipFlop is  
begin  
	PROCESS(clk, reset)
	BEGIN 
		if(reset = '1') then 
			q <= '0';	
		elsif(rising_edge(Clk)) then
			q <= d; 
		end if;      
	END PROCESS;
end Behavioral;
	
entity datapath is
	port (
		-- common
		clk 		: in  std_logic;
		reset		: in  std_logic;
		
		-- control
		opc			: in  std_logic_vector(4 downto 0);
		regwrite	: in  std_logic;
		
		-- input data
		aop			: in  std_logic_vector(31 downto 0);
		bop			: in  std_logic_vector(31 downto 0);
		iop			: in  std_logic_vector(15 downto 0);
		ivalid		: in  std_logic;
		
		-- output data
		wop			: out std_logic_vector(31 downto 0);
		
		-- XBus
		selxres		: in  std_logic;
		xdatain		: in  std_logic_vector(31 downto 0);
		xdataout	: out std_logic_vector(31 downto 0);
		xadr		: out std_logic_vector(12 downto 0);
		
		-- status flags
		zero		: out std_logic;
		ov			: out std_logic;
		
		-- program counter
		jal			: in  std_logic;
		rela		: in  std_logic;
		pcinc		: in  std_logic_vector(11 downto 0);
		pcnew		: out std_logic_vector(11 downto 0);
		sisalvl     : out std_logic_vector(1 downto 0)
	);
end datapath;

architecture rtl of datapath is
	signal aluResult 	: std_logic_vector(31 downto 0);
	--signal aluZero 		: std_logic;
	--signal aluOv		: std_logic;
	
	signal imuxRes		: std_logic;
	signal pcmuxRes		: std_logic_vector(31 downto 0);
	
	signal opregRes		: std_logic_vector( 4 downto 0);
	signal aregRes		: std_logic_vector(31 downto 0);
	signal bcregRes		: std_logic_vector(31 downto 0);
	signal bregRes		: std_logic_vector(31 downto 0);
	
	signal xdatainRes	: std_logic_vector(31 downto 0);
	
begin
	imux : mux2_1
	generic map (WIDTH => 32);
	port map(
		sel => ivalid,
		a => aop,
		b => bop,
		res => imuxRes
	);
	
	pcmux : mux2_1
	generic map (WIDTH => 32);
	port map(
		sel => jal,
		a => aluResult,
		b => pcinc,
		res => pcmuxRes
	);
	
	relamux : mux2_1
	generic map (WIDTH => 32);
	port map(
		sel => rela,
		a => aluResult,
		b => aluResult,
		res => pcnew
	);
	
	dmux : mux2_1
	generic map (WIDTH => 32);
	port map(
		sel => selxres,
		a => pcmuxRes,
		b => xdatainRes,
		res => wop
	);
	
	opreg : FlipFlop 
	generic map (WIDTH => 5);
	port map(
		clk => clk,
		reset => reset,
		d => opc,
		res => opRegRes
	);
	
	areg : FlipFlop 
	generic map (WIDTH => 32);
	port map(
		clk => clk,
		reset => reset,
		d => aop,
		res => aregRes
	);
	
	bcreg : FlipFlop 
	generic map (WIDTH => 32);
	port map(
		clk => clk,
		reset => reset,
		d => imuxRes,
		res => bcregRes
	);
	
	breg : FlipFlop 
	generic map (WIDTH => 32);
	port map(
		clk => clk,
		reset => reset,
		d => bop,
		res => xdataout
	);
	
	rreg : FlipFlop 
	generic map (WIDTH => 32);
	port map(
		clk => clk,
		reset => reset,
		d => bop,
		res => aluResult
	);
	
	zff : FlipFlop 
	generic map (WIDTH => 1);
	port map(
		clk => clk,
		reset => reset,
		d => '0',
		q => zero
	);
	
	xreg : FlipFlop 
	generic map (WIDTH => 32);
	port map(
		clk => clk,
		reset => reset,
		d => xdatain,
		q => xdatainRes
	);	
	
	alu_ : alu
	port map (
		clk => clk,		
		reset => reset,
		
		opcode => opc,	
		regwrite => regwrite,
		
		achannel => aregRes,
		bchannel => bcregRes,
		
		result => aluResult,
		zero => zero,	
		overflow => ov
	);
	
	datapathP : PROCESS (clk, reset)
	BEGIN
		--if (rising_edge(clk)) then
		--	if (opc = "01110") then --sisa Instruktion
		--		sisalvl = (aluResult(14) & aluResult(15));
		--	end if;
		--	
		--	elsif (jal = '1') then
		--		
		--	end if;
	
	END PROCESS datapathP;
end rtl;
