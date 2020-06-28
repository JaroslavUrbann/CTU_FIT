--Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
----------------------------------------------------------------------------------
--Tool Version: Vivado v.2018.2 (lin64) Build 2258646 Thu Jun 14 20:02:38 MDT 2018
--Date        : Sun Apr  5 13:53:28 2020
--Host        : JaroslavUrban running 64-bit Linux Mint 19.1 Tessa
--Command     : generate_target lab3.bd
--Design      : lab3
--Purpose     : IP block netlist
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity lab3 is
  port (
    a_0 : in STD_LOGIC;
    b_0 : in STD_LOGIC;
    c_0 : in STD_LOGIC;
    d_0 : in STD_LOGIC;
    f_a_0 : out STD_LOGIC;
    f_b_0 : out STD_LOGIC;
    f_c_0 : out STD_LOGIC;
    f_d_0 : out STD_LOGIC;
    f_e_0 : out STD_LOGIC;
    f_f_0 : out STD_LOGIC;
    f_g_0 : out STD_LOGIC
  );
  attribute CORE_GENERATION_INFO : string;
  attribute CORE_GENERATION_INFO of lab3 : entity is "lab3,IP_Integrator,{x_ipVendor=xilinx.com,x_ipLibrary=BlockDiagram,x_ipName=lab3,x_ipVersion=1.00.a,x_ipLanguage=VHDL,numBlks=1,numReposBlks=1,numNonXlnxBlks=0,numHierBlks=0,maxHierDepth=0,numSysgenBlks=0,numHlsBlks=0,numHdlrefBlks=1,numPkgbdBlks=0,bdsource=USER,synth_mode=OOC_per_IP}";
  attribute HW_HANDOFF : string;
  attribute HW_HANDOFF of lab3 : entity is "lab3.hwdef";
end lab3;

architecture STRUCTURE of lab3 is
  component lab3_decoder_7seg_0_0 is
  port (
    a : in STD_LOGIC;
    b : in STD_LOGIC;
    c : in STD_LOGIC;
    d : in STD_LOGIC;
    f_a : out STD_LOGIC;
    f_b : out STD_LOGIC;
    f_c : out STD_LOGIC;
    f_d : out STD_LOGIC;
    f_e : out STD_LOGIC;
    f_f : out STD_LOGIC;
    f_g : out STD_LOGIC
  );
  end component lab3_decoder_7seg_0_0;
  signal a_0_1 : STD_LOGIC;
  signal b_0_1 : STD_LOGIC;
  signal c_0_1 : STD_LOGIC;
  signal d_0_1 : STD_LOGIC;
  signal decoder_7seg_0_f_a : STD_LOGIC;
  signal decoder_7seg_0_f_b : STD_LOGIC;
  signal decoder_7seg_0_f_c : STD_LOGIC;
  signal decoder_7seg_0_f_d : STD_LOGIC;
  signal decoder_7seg_0_f_e : STD_LOGIC;
  signal decoder_7seg_0_f_f : STD_LOGIC;
  signal decoder_7seg_0_f_g : STD_LOGIC;
begin
  a_0_1 <= a_0;
  b_0_1 <= b_0;
  c_0_1 <= c_0;
  d_0_1 <= d_0;
  f_a_0 <= decoder_7seg_0_f_a;
  f_b_0 <= decoder_7seg_0_f_b;
  f_c_0 <= decoder_7seg_0_f_c;
  f_d_0 <= decoder_7seg_0_f_d;
  f_e_0 <= decoder_7seg_0_f_e;
  f_f_0 <= decoder_7seg_0_f_f;
  f_g_0 <= decoder_7seg_0_f_g;
decoder_7seg_0: component lab3_decoder_7seg_0_0
     port map (
      a => a_0_1,
      b => b_0_1,
      c => c_0_1,
      d => d_0_1,
      f_a => decoder_7seg_0_f_a,
      f_b => decoder_7seg_0_f_b,
      f_c => decoder_7seg_0_f_c,
      f_d => decoder_7seg_0_f_d,
      f_e => decoder_7seg_0_f_e,
      f_f => decoder_7seg_0_f_f,
      f_g => decoder_7seg_0_f_g
    );
end STRUCTURE;
