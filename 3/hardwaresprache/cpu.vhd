---------------------------------------------------------------------------------------------------
--
-- Titel:    
-- Autor:    Georgios Markou
-- Datum:    
--
---------------------------------------------------------------------------------------------------

library ieee;
	use ieee.std_logic_1164.all;
	use ieee.numeric_std.all;
library work;
	use work.all;
	use work.hadescomponents.all;
	
entity cpu is
	generic (
		INIT		: string := "UNUSED"
	);
	port (
		-- common
		clk 		: in  std_logic;
		reset		: in  std_logic;
		
		-- XBus
		xread		: out std_logic;
		xwrite		: out std_logic;
		xadr		: out std_logic_vector(12 downto 0);
		xdatain		: in  std_logic_vector(31 downto 0);
		xdataout	: out std_logic_vector(31 downto 0);
		xpresent	: in  std_logic;
		xack		: in  std_logic;
		dmemop		: out std_logic;
		dmembusy	: in  std_logic;
		xperintr	: in  std_logic;
		xmemintr	: in  std_logic
	);
end cpu;

architecture rtl of cpu is
signal pmemory_radr_in 		: std_logic_vector(11 downto 0);
signal pmemory_loadir_in	: std_logic;
signal pmemory_datain_in	: std_logic_vector(31 downto 0);
signal pmemory_wadr_in 		: std_logic_vector(12 downto 0);
signal pmemory_pwrite_in	: std_logic;
signal pmemory_dataout_out	: std_logic_vector(31 downto 0);

signal indec_aopadr_out		: std_logic_vector(2 downto 0);
signal indec_bopadr_out		: std_logic_vector(2 downto 0);
signal indec_wopadr_out		: std_logic_vector(2 downto 0);
signal indec_ivalid_out		: std_logic;
signal indec_iop_out		: std_logic_vector(15 downto 0);
signal indec_opc_out		: std_logic_vector(4 downto 0);
signal indec_pccontr_out	: std_logic_vector(10 downto 0);
signal indec_inop_out		: std_logic;
signal indec_outop_out		: std_logic;
signal indec_loadop_out		: std_logic;
signal indec_storeop_out	: std_logic;
--signal indec_dmemop_out	: std_logic;
signal indec_selxres_out	: std_logic;
signal indec_dpma_out		: std_logic;
signal indec_epma_out		: std_logic;

--signal haregs_regwrite_in	: std_logic;
signal haregs_wop_in		: std_logic_vector(31 downto 0);
signal haregs_aop_out		: std_logic_vector(31 downto 0);
signal haregs_bop_out		: std_logic_vector(31 downto 0);

signal control_loadir_out	: std_logic;
signal control_regwrite_out	: std_logic;
signal control_pcwrite_out	: std_logic;
signal control_pwrite_out	: std_logic;
--signal control_xread_out	: std_logic;
--signal control_xwrite_out	: std_logic;
signal control_xnaintr_out	: std_logic;

signal datapath_zero_out	: std_logic;
signal datapath_ov_out		: std_logic;
signal datapath_pcnew_out	: std_logic_vector(11 downto 0);
signal datapath_sisalvl_out	: std_logic_vector(1 downto 0);

signal pclogic_pcinc_out	: std_logic_vector(11 downto 0);

begin
	pmemory_E: entity work.pmemory
	port map (
		clk => clk,
		reset => reset,

		radr => pmemory_radr_in,
		loadir => pmemory_loadir_in,
		datain => pmemory_datain_in,
		wadr => pmemory_wadr_in(11 downto 0),
		pwrite => pmemory_pwrite_in,

		dataout => pmemory_dataout_out
	);

	indec_E: entity work.indec
	port map (
		iword => pmemory_dataout_out,
		
		aopadr => indec_aopadr_out,
		bopadr => indec_bopadr_out,
		wopadr => indec_wopadr_out,
		ivalid => indec_ivalid_out,
		iop => indec_iop_out,
		opc => indec_opc_out,
		pccontr => indec_pccontr_out,
		inop => indec_inop_out,
		outop => indec_outop_out,		
		loadop => indec_loadop_out,
		storeop => indec_storeop_out,
		dmemop => dmemop, --indec_dmemop_out,
		selxres => indec_selxres_out,
		dpma => indec_dpma_out,
		epma => indec_epma_out
	);

	haregs_E: entity work.haregs
	port map (
		clk => clk,
		reset => reset,

		aopadr => indec_aopadr_out,
		bopadr => indec_bopadr_out,
		wopadr => indec_wopadr_out,
		wop => haregs_wop_in,
		regwrite => control_regwrite_out, --haregs_regwrite_in,

		aop => haregs_aop_out,
		bop => haregs_bop_out
	);

	control_E: entity work.control
	port map (
		clk => clk,
		reset => reset,

		inop => indec_inop_out,
		outop => indec_outop_out,
		loadop => indec_loadop_out,
		storeop => indec_storeop_out,
		dpma => indec_dpma_out,
		epma => indec_epma_out,
		xack => xack,
		xpresent => xpresent,
		dmembusy => dmembusy,
	
		loadir => pmemory_loadir_in, --control_loadir_out,
		regwrite => control_regwrite_out,
		pcwrite => control_pcwrite_out,
		pwrite => pmemory_pwrite_in, --control_pwrite_out,
		xread => xread, --control_xread_out,
		xwrite => xwrite, -- control_xwrite_out,
		xnaintr => control_xnaintr_out
	);

	datapath_E: entity work.datapath
	port map (
		clk => clk,
		reset => reset,

		aop => haregs_aop_out,
		bop => haregs_bop_out,
		iop => indec_iop_out,
		xdatain => xdatain,
		ivalid => indec_ivalid_out,
		opc => indec_opc_out,
		jal => indec_pccontr_out(8),
		rela => indec_pccontr_out(10),
		selxres => indec_selxres_out,
		pcinc => pclogic_pcinc_out,
		regwrite => control_regwrite_out,

		wop => haregs_wop_in,
		xadr => pmemory_wadr_in,
		xdataout => xdataout,
		pcnew => datapath_pcnew_out,
		sisalvl => datapath_sisalvl_out,
		ov => datapath_ov_out,
		zero => datapath_zero_out
	);

	pclogic_E: entity work.pcblock
	port map (
		clk => clk,
		reset => reset,

		pcwrite => control_pcwrite_out,
		pccontr => indec_pccontr_out,
		pcnew => datapath_pcnew_out,
		sisalvl => datapath_sisalvl_out,
		ov => datapath_ov_out,
		zero => datapath_zero_out,
		xperintr => xperintr,
		xnaintr => control_xnaintr_out,
		xmemintr => xmemintr,

		pcakt => pmemory_radr_in,
		pcinc => pclogic_pcinc_out
	);
	

	-- dmemop <= indec_dmemop_out; -- in indec_E
	-- xread <= control_xread_out; -- in control_E
	-- xwrite <= control_xwrite_out; -- in control_E

	--xadr <= pmemory_wadr_in; -- datapath
end rtl;

