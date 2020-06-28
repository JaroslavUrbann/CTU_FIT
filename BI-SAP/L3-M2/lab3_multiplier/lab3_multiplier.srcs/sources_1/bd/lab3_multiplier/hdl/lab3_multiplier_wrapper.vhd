--Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
----------------------------------------------------------------------------------
--Tool Version: Vivado v.2018.2 (lin64) Build 2258646 Thu Jun 14 20:02:38 MDT 2018
--Date        : Sun Apr  5 13:29:09 2020
--Host        : JaroslavUrban running 64-bit Linux Mint 19.1 Tessa
--Command     : generate_target lab3_multiplier_wrapper.bd
--Design      : lab3_multiplier_wrapper
--Purpose     : IP block netlist
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity lab3_multiplier_wrapper is
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
end lab3_multiplier_wrapper;

architecture STRUCTURE of lab3_multiplier_wrapper is
  component lab3_multiplier is
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
  end component lab3_multiplier;
begin
lab3_multiplier_i: component lab3_multiplier
     port map (
      a0_0 => a0_0,
      a1_0 => a1_0,
      b0_0 => b0_0,
      b1_0 => b1_0,
      y0_0 => y0_0,
      y1_0 => y1_0,
      y2_0 => y2_0,
      y3_0 => y3_0
    );
end STRUCTURE;
