--Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
----------------------------------------------------------------------------------
--Tool Version: Vivado v.2018.2 (lin64) Build 2258646 Thu Jun 14 20:02:38 MDT 2018
--Date        : Fri Apr 10 12:24:48 2020
--Host        : JaroslavUrban running 64-bit Linux Mint 19.1 Tessa
--Command     : generate_target seq_detector_wrapper.bd
--Design      : seq_detector_wrapper
--Purpose     : IP block netlist
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity seq_detector_wrapper is
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
end seq_detector_wrapper;

architecture STRUCTURE of seq_detector_wrapper is
  component seq_detector is
  port (
    SWITCH : in STD_LOGIC;
    RESET : in STD_LOGIC;
    DETECT : out STD_LOGIC;
    CE : in STD_LOGIC;
    S0 : out STD_LOGIC;
    S1 : out STD_LOGIC;
    S2 : out STD_LOGIC;
    CLK : in STD_LOGIC;
    BTN : in STD_LOGIC
  );
  end component seq_detector;
begin
seq_detector_i: component seq_detector
     port map (
      BTN => BTN,
      CE => CE,
      CLK => CLK,
      DETECT => DETECT,
      RESET => RESET,
      S0 => S0,
      S1 => S1,
      S2 => S2,
      SWITCH => SWITCH
    );
end STRUCTURE;
