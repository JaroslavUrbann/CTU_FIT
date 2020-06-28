--Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
----------------------------------------------------------------------------------
--Tool Version: Vivado v.2018.2 (lin64) Build 2258646 Thu Jun 14 20:02:38 MDT 2018
--Date        : Sun Apr  5 13:53:29 2020
--Host        : JaroslavUrban running 64-bit Linux Mint 19.1 Tessa
--Command     : generate_target lab3_wrapper.bd
--Design      : lab3_wrapper
--Purpose     : IP block netlist
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity lab3_wrapper is
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
end lab3_wrapper;

architecture STRUCTURE of lab3_wrapper is
  component lab3 is
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
  end component lab3;
begin
lab3_i: component lab3
     port map (
      a_0 => a_0,
      b_0 => b_0,
      c_0 => c_0,
      d_0 => d_0,
      f_a_0 => f_a_0,
      f_b_0 => f_b_0,
      f_c_0 => f_c_0,
      f_d_0 => f_d_0,
      f_e_0 => f_e_0,
      f_f_0 => f_f_0,
      f_g_0 => f_g_0
    );
end STRUCTURE;
