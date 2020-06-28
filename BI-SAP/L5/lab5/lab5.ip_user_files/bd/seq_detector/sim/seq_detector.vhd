--Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
----------------------------------------------------------------------------------
--Tool Version: Vivado v.2018.2 (lin64) Build 2258646 Thu Jun 14 20:02:38 MDT 2018
--Date        : Fri Apr 10 12:24:48 2020
--Host        : JaroslavUrban running 64-bit Linux Mint 19.1 Tessa
--Command     : generate_target seq_detector.bd
--Design      : seq_detector
--Purpose     : IP block netlist
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity seq_detector is
  port (
    BTN : in STD_LOGIC;
    CE : in STD_LOGIC;
    CLK : in STD_LOGIC;
    DETECT : out STD_LOGIC;
    RESET : in STD_LOGIC;
    S0 : out STD_LOGIC;
    S1 : out STD_LOGIC;
    S2 : out STD_LOGIC;
    SWITCH : in STD_LOGIC
  );
  attribute CORE_GENERATION_INFO : string;
  attribute CORE_GENERATION_INFO of seq_detector : entity is "seq_detector,IP_Integrator,{x_ipVendor=xilinx.com,x_ipLibrary=BlockDiagram,x_ipName=seq_detector,x_ipVersion=1.00.a,x_ipLanguage=VHDL,numBlks=6,numReposBlks=6,numNonXlnxBlks=4,numHierBlks=0,maxHierDepth=0,numSysgenBlks=0,numHlsBlks=0,numHdlrefBlks=2,numPkgbdBlks=0,bdsource=USER,synth_mode=Global}";
  attribute HW_HANDOFF : string;
  attribute HW_HANDOFF of seq_detector : entity is "seq_detector.hwdef";
end seq_detector;

architecture STRUCTURE of seq_detector is
  component seq_detector_next_state_logic_0_0 is
  port (
    q0 : in STD_LOGIC;
    q1 : in STD_LOGIC;
    q2 : in STD_LOGIC;
    sw : in STD_LOGIC;
    d0 : out STD_LOGIC;
    d1 : out STD_LOGIC;
    d2 : out STD_LOGIC
  );
  end component seq_detector_next_state_logic_0_0;
  component seq_detector_output_logic_0_0 is
  port (
    q0 : in STD_LOGIC;
    q1 : in STD_LOGIC;
    q2 : in STD_LOGIC;
    sw : in STD_LOGIC;
    y : out STD_LOGIC
  );
  end component seq_detector_output_logic_0_0;
  component seq_detector_dff_0_0 is
  port (
    d : in STD_LOGIC;
    clk : in STD_LOGIC;
    ce : in STD_LOGIC;
    reset : in STD_LOGIC;
    q : out STD_LOGIC
  );
  end component seq_detector_dff_0_0;
  component seq_detector_dff_0_1 is
  port (
    d : in STD_LOGIC;
    clk : in STD_LOGIC;
    ce : in STD_LOGIC;
    reset : in STD_LOGIC;
    q : out STD_LOGIC
  );
  end component seq_detector_dff_0_1;
  component seq_detector_dff_0_2 is
  port (
    d : in STD_LOGIC;
    clk : in STD_LOGIC;
    ce : in STD_LOGIC;
    reset : in STD_LOGIC;
    q : out STD_LOGIC
  );
  end component seq_detector_dff_0_2;
  component seq_detector_debounce_0_0 is
  port (
    clk : in STD_LOGIC;
    tl_in : in STD_LOGIC;
    tl_out : out STD_LOGIC
  );
  end component seq_detector_debounce_0_0;
  signal Net : STD_LOGIC;
  signal Net1 : STD_LOGIC;
  signal Net2 : STD_LOGIC;
  signal clk_0_1 : STD_LOGIC;
  signal dff_0_q : STD_LOGIC;
  signal dff_2_q : STD_LOGIC;
  signal next_state_logic_0_d0 : STD_LOGIC;
  signal next_state_logic_0_d1 : STD_LOGIC;
  signal next_state_logic_0_d2 : STD_LOGIC;
  signal output_logic_0_y : STD_LOGIC;
  signal q1_0_1 : STD_LOGIC;
  signal sw_0_1 : STD_LOGIC;
  signal tl_in_0_1 : STD_LOGIC;
begin
  DETECT <= output_logic_0_y;
  Net <= CE;
  Net2 <= RESET;
  S0 <= dff_0_q;
  S1 <= q1_0_1;
  S2 <= dff_2_q;
  clk_0_1 <= CLK;
  sw_0_1 <= SWITCH;
  tl_in_0_1 <= BTN;
debounce_0: component seq_detector_debounce_0_0
     port map (
      clk => clk_0_1,
      tl_in => tl_in_0_1,
      tl_out => Net1
    );
dff_0: component seq_detector_dff_0_0
     port map (
      ce => Net,
      clk => Net1,
      d => next_state_logic_0_d0,
      q => dff_0_q,
      reset => Net2
    );
dff_1: component seq_detector_dff_0_1
     port map (
      ce => Net,
      clk => Net1,
      d => next_state_logic_0_d1,
      q => q1_0_1,
      reset => Net2
    );
dff_2: component seq_detector_dff_0_2
     port map (
      ce => Net,
      clk => Net1,
      d => next_state_logic_0_d2,
      q => dff_2_q,
      reset => Net2
    );
next_state_logic_0: component seq_detector_next_state_logic_0_0
     port map (
      d0 => next_state_logic_0_d0,
      d1 => next_state_logic_0_d1,
      d2 => next_state_logic_0_d2,
      q0 => dff_0_q,
      q1 => q1_0_1,
      q2 => dff_2_q,
      sw => sw_0_1
    );
output_logic_0: component seq_detector_output_logic_0_0
     port map (
      q0 => dff_0_q,
      q1 => q1_0_1,
      q2 => dff_2_q,
      sw => sw_0_1,
      y => output_logic_0_y
    );
end STRUCTURE;
