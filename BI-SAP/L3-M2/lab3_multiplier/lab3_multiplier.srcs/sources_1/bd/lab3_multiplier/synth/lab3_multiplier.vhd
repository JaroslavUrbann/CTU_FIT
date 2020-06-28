--Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
----------------------------------------------------------------------------------
--Tool Version: Vivado v.2018.2 (lin64) Build 2258646 Thu Jun 14 20:02:38 MDT 2018
--Date        : Sun Apr  5 13:29:09 2020
--Host        : JaroslavUrban running 64-bit Linux Mint 19.1 Tessa
--Command     : generate_target lab3_multiplier.bd
--Design      : lab3_multiplier
--Purpose     : IP block netlist
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity lab3_multiplier is
  port (
    a0_0 : in STD_LOGIC;
    a1_0 : in STD_LOGIC;
    b0_0 : in STD_LOGIC;
    b1_0 : in STD_LOGIC;
    y0_0 : out STD_LOGIC;
    y1_0 : out STD_LOGIC;
    y2_0 : out STD_LOGIC;
    y3_0 : out STD_LOGIC
  );
  attribute CORE_GENERATION_INFO : string;
  attribute CORE_GENERATION_INFO of lab3_multiplier : entity is "lab3_multiplier,IP_Integrator,{x_ipVendor=xilinx.com,x_ipLibrary=BlockDiagram,x_ipName=lab3_multiplier,x_ipVersion=1.00.a,x_ipLanguage=VHDL,numBlks=1,numReposBlks=1,numNonXlnxBlks=0,numHierBlks=0,maxHierDepth=0,numSysgenBlks=0,numHlsBlks=0,numHdlrefBlks=1,numPkgbdBlks=0,bdsource=USER,synth_mode=OOC_per_IP}";
  attribute HW_HANDOFF : string;
  attribute HW_HANDOFF of lab3_multiplier : entity is "lab3_multiplier.hwdef";
end lab3_multiplier;

architecture STRUCTURE of lab3_multiplier is
  component lab3_multiplier_multiplier_0_0 is
  port (
    a0 : in STD_LOGIC;
    a1 : in STD_LOGIC;
    b0 : in STD_LOGIC;
    b1 : in STD_LOGIC;
    y0 : out STD_LOGIC;
    y1 : out STD_LOGIC;
    y2 : out STD_LOGIC;
    y3 : out STD_LOGIC
  );
  end component lab3_multiplier_multiplier_0_0;
  signal a0_0_1 : STD_LOGIC;
  signal a1_0_1 : STD_LOGIC;
  signal b0_0_1 : STD_LOGIC;
  signal b1_0_1 : STD_LOGIC;
  signal multiplier_0_y0 : STD_LOGIC;
  signal multiplier_0_y1 : STD_LOGIC;
  signal multiplier_0_y2 : STD_LOGIC;
  signal multiplier_0_y3 : STD_LOGIC;
begin
  a0_0_1 <= a0_0;
  a1_0_1 <= a1_0;
  b0_0_1 <= b0_0;
  b1_0_1 <= b1_0;
  y0_0 <= multiplier_0_y0;
  y1_0 <= multiplier_0_y1;
  y2_0 <= multiplier_0_y2;
  y3_0 <= multiplier_0_y3;
multiplier_0: component lab3_multiplier_multiplier_0_0
     port map (
      a0 => a0_0_1,
      a1 => a1_0_1,
      b0 => b0_0_1,
      b1 => b1_0_1,
      y0 => multiplier_0_y0,
      y1 => multiplier_0_y1,
      y2 => multiplier_0_y2,
      y3 => multiplier_0_y3
    );
end STRUCTURE;
