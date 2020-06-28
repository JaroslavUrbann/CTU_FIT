----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 04/05/2020 01:09:40 PM
-- Design Name: 
-- Module Name: multiplier - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity multiplier is
    Port ( a0 : in std_logic;
            a1 : in std_logic;
            b0 : in std_logic;
            b1 : in std_logic;
            y0, y1, y2, y3 : out std_logic);
end multiplier;
  
architecture Behavioral of multiplier is
begin
    y0 <= (a0 and b0);
    y1 <= ((a0 and b1 and (not b0)) or (a0 and (not a1) and b1) or ((not a0) and a1 and b0) or (a1 and b0 and (not b1)));
    y2 <= ((a0 and a1 and (not b0) and b1) or ((not a0) and a1 and b1));
    y3 <= ((a0 and a1 and b0 and b1));
end Behavioral;
