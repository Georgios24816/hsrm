---------------------------------------------------------------------------------------------------
--
-- Titel: INDEC    
-- Autor: Felix Gnatzy   
-- Datum: 30.11.2020   
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
        -- Register address of the operand A
        aopadr <= iword(19 downto 17);

        -- Register address of the operand B
        bopadr <= iword(22 downto 20) when (iword(31 downto 28) = "0011") else --OUT
                  iword(22 downto 20) when (iword(31 downto 28) = "1001") else --STORE
                  iword(15 downto 13);
        
        --Register address of the destination operand
        wopadr <= "000" when (iword(31 downto 28) = "0011") else --OUT
                  "000" when (iword(31 downto 28) = "1001") else --STORE
                  iword(22 downto 20);

         -- Immediate value
         -- "0000" & zum auffüllen der fehlenden Bits, iop muss 16 Bits groß sein
        iop <=  "0000" & iword(11 downto 0) when (iword(31 downto 28) = "0101") else --BNEZ
                "0000" & iword(11 downto 0) when (iword(31 downto 28) = "0110") else --BEQZ
                "0000" & iword(11 downto 0) when (iword(31 downto 28) = "0111") else --BOV
                "0000" & iword(11 downto 0) when (iword(31 downto 28) = "1010") else --JAL
                "0000" & iword(11 downto 0) when (iword(31 downto 28) = "1101") else --SISA
                iword(15 downto 0);

	--Immediate operand validity
        ivalid <= iword(16);

        --Op-code for the ALU
	opc <= iword(27 downto 23);

        -- Program counter control for PC and interrupt
        pccontr <= "10100000000" when (iword(31 downto 28) = "1010") else -- JAL
                   "10000001000" when (iword(31 downto 28) = "1101") else -- SISA
                   "10000000100" when (iword(31 downto 28) = "0111") else -- BOV
                   "10000000010" when (iword(31 downto 28) = "0110") else -- BEQZ
                   "10000000001" when (iword(31 downto 28) = "0101") else -- BNEZ
                   "00000010000" when (iword(31 downto 28) = "0100") else -- DEI
                   "00000100000" when (iword(31 downto 28) = "0001") else -- ENI
                   "00001000000" when (iword(31 downto 28) = "1100") else -- RETI
                   "00010000000" when (iword(31 downto 28) = "1011") else -- JREG
                   "01000000000" when (iword(27 downto 23) = "00010") else -- SWI (Selektion über AluOpCode(okay?))
                   "00000000000";

        -- IN instruction
        inop <= '1' when (iword(31 downto 28) = "0010") else --IN
                '0';

        -- OUT instruction
        outop <= '1' when (iword(31 downto 28) = "0011") else -- OUT
                 '0';

        -- LOAD intstruction
        loadop <= '1' when (iword(31 downto 28) = "1000") else --LOAD
                  '0';

        -- STORE intstruction
        storeop <= '1' when (iword(31 downto 28) = "1001") else --STORE
                   '0';

        -- STORE or LOAD intstruction
        dmemop <= '1' when (iword(31 downto 28) = "1000") else --LOAD
                  '1' when (iword(31 downto 28) = "1001") else --STORE
                  '0';

        -- XBus will be used
        selxres <= '1' when (iword(31 downto 28) = "0010") else --IN
                   '1' when (iword(31 downto 28) = "0011") else --OUT
                   '1' when (iword(31 downto 28) = "1000") else --LOAD
                   '1' when (iword(31 downto 28) = "1001") else --STORE
                   '0';
        
        -- DPMA intstruction
        dpma <= '1' when (iword(31 downto 28) = "1110") else --DPMA
                '0';

        -- EPMA instruction
        epma <= '1' when (iword(31 downto 28) = "1111") else --EPMA
                '0';
end rtl;
