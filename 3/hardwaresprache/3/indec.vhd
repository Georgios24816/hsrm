---------------------------------------------------------------------------------------------------
--
-- Titel:    
-- Autor: Georgios Markou 
-- Datum:    
--
---------------------------------------------------------------------------------------------------

library ieee;
	use ieee.std_logic_1164.all;
	use ieee.numeric_std.all;
--library work;
--	use work.hadescomponents.all;
	
entity indec is
	port (
		-- instruction word input
		iword		: in  std_logic_vector(31 downto 0);
		
		-- register addresses
		aopadr		: out std_logic_vector(2 downto 0); --
		bopadr		: out std_logic_vector(2 downto 0); 
		wopadr		: out std_logic_vector(2 downto 0); --
		
		-- immediate value
		ivalid		: out std_logic; 					--
		iop			: out std_logic_vector(15 downto 0);--
		
		-- control flags
		opc			: out std_logic_vector(4 downto 0);--
		pccontr		: out std_logic_vector(10 downto 0);
		inop		: out std_logic;
		outop		: out std_logic;
		loadop		: out std_logic;
		storeop		: out std_logic;
		dmemop		: out std_logic;
		selxres		: out std_logic;
		dpma		: out std_logic;
		epma		: out std_logic
	);
end indec;

architecture rtl of indec is
begin
	ivalid <= iword(16);
	opc <= iword(27 downto 23);
	iop <= iword(15 downto 0);
	
	wopadr <= 	"000" when (iword(31 downto 28) = "0011") else
				"000" when (iword(31 downto 28) = "1001") else
                iword(22 downto 20);
	
	aopadr <= iword(19 downto 17);
	
	bopadr <= 	iword(22 downto 20) when ( (iword(31 downto 28) = "0011") or ((iword(31 downto 28) = "1001")) ) else
				iword(15 downto 13);
	
	--pccontr(0) <= not iword(31) and iword(30) and not iword(29) and iword(28);
	--pccontr(1) <= not iword(31) and iword(30) and iword(29) and not iword(28);
	
	--pccontr(2) <= not iword(31) and iword(30) and iword(29) and iword(28);
	--
	--pccontr(3) <= iword(31) and iword(30) and not iword(29) and iword(28);
	--pccontr(4) <= not iword(31) and iword(30) and not iword(29) and not iword(28);
	--pccontr(5) <= not iword(31) and not iword(30) and not iword(29) and iword(28);
	--pccontr(6) <= iword(31) and iword(30) and not iword(29) and not iword(28);
	--pccontr(7) <= iword(31) and not iword(30) and iword(29) and iword(28);
	--pccontr(8) <= iword(31) and not iword(30) and iword(29) and not iword(28);
	--pccontr(9) <= (not iword(31) and not iword(30) and not iword(29) and not iword(28)) and (iword(24) and not iword(23));
	--
	--pccontr(10) <= pccontr(0) or pccontr(1) or pccontr(2) or pccontr(3) or pccontr(8);
	
	--(pccontr(0) or pccontr(1) or pccontr(2) or pccontr(3) or pccontr(8)) &
	
	pccontr <= 	((not iword(31) and iword(30) and iword(29) and not iword(28)) or (not iword(31) and iword(30) and not iword(29) and iword(28)) or
				(iword(31) and iword(30) and not iword(29) and iword(28)) or (not iword(31) and iword(30) and iword(29) and iword(28)) or
				(iword(31) and not iword(30) and iword(29) and not iword(28))) &
				
				((not iword(31) and not iword(30) and not iword(29) and not iword(28)) and (iword(24) and not iword(23) and not iword(27) and not iword(26) and not iword(25))) &
				(iword(31) and not iword(30) and iword(29) and not iword(28)) &
				(iword(31) and not iword(30) and iword(29) and iword(28)) &
				(iword(31) and iword(30) and not iword(29) and not iword(28)) &
				(not iword(31) and not iword(30) and not iword(29) and iword(28)) &
				(not iword(31) and iword(30) and not iword(29) and not iword(28)) &
				(iword(31) and iword(30) and not iword(29) and iword(28)) &
				(not iword(31) and iword(30) and iword(29) and iword(28)) &
				(not iword(31) and iword(30) and iword(29) and not iword(28)) &
				(not iword(31) and iword(30) and not iword(29) and iword(28));
	
	inop <= iword(29) and not iword(31) and not iword(30) and not iword(28);
	outop <= iword(29) and not iword(31) and not iword(30) and iword(28);
	
	loadop <= iword(31) and not iword(30) and not iword(29) and not iword(28);
	storeop <= iword(31) and not iword(30) and not iword(29) and iword(28);
	
	--dmemop <= loadop or storeop;
	dmemop <= (iword(31) and not iword(30) and not iword(29) and not iword(28)) or (iword(31) and not iword(30) and not iword(29) and iword(28));
	
	--selxres <= dmemop or inop or outop;
	selxres <= (iword(31) and not iword(30) and not iword(29) and not iword(28)) or (iword(31) and not iword(30) and not iword(29) and iword(28)) or (iword(29) and not iword(31) and not iword(30) and not iword(28)) or (iword(29) and not iword(31) and not iword(30) and iword(28));
	
	dpma <= iword(29) and iword(31) and iword(30) and not iword(28);
	epma <= iword(29) and iword(31) and iword(30) and iword(28);
	
end rtl;