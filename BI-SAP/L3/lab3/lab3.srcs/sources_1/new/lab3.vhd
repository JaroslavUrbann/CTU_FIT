--POTREBNE KNIHOVNY PRO PRACI
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;




-- POPIS CERNE SKRINKY S NAZVEM DEKODER, ZNAME POUZE VSTUPY A VYSTUPY
entity decoder_7seg is
	port(
		a   : in  STD_LOGIC;
		b   : in  STD_LOGIC;
		c   : in  STD_LOGIC;
		d   : in  STD_LOGIC;
		f_a : out STD_LOGIC;
		f_b : out STD_LOGIC;
		f_c : out STD_LOGIC;
		f_d : out STD_LOGIC;
		f_e : out STD_LOGIC;
		f_f : out STD_LOGIC;
		f_g : out STD_LOGIC
	);
end decoder_7seg;


-- POPIS VNITRNOSTI CERNE SKRINKY, KTERA JE DEFINOVANA VYSE
architecture Behavioral of decoder_7seg is
begin

-- DOPLNTE PREDPRIPRAVENE FUNKCE PRO BUZENI DEKODERU
-- VE FORMATU JAKO JE NA PRIKLADU NIZE, FUNKCE JSOU JIZ ZNEGOVANY, NEBOT DISPLEJ JE AKTIVNI V LOG. 0 
-- f_g <= (not(((not a) and b) or (b and (not c)) or ((not c) and d) or ((not b) and c and (not d))));
	
	f_a <= (( (a and (not b) and (not c) and (not d)) or ((not a) and (not b) and c and (not d)) or (a and (not b) and c and d) or (a and b and (not c) and d) )); --spravne
    f_b <= (( (b and c and (not a)) or (a and b and d) or ((not a) and (not b) and c and d) or (a and (not b) and c and (not d) ) )); --spravne
    f_c <= (( (b and c and d) or ((not a) and (not b) and c and d) or ((not a) and b and (not c) and (not d)) )); --spravne
    f_d <= (( (a and (not b) and (not c) and (not d)) or ((not a) and (not b) and c and (not d)) or (a and b and c) or ((not a) and b and (not c) and d) )); --spravne
    f_e <= (( (a and (not d)) or (a and (not b) and (not c)) or ((not b) and c and (not d)) ));
    f_f <= (( (a and (not c) and (not d)) or (b and (not c) and (not d)) or (a and b and (not d)) or (a and (not b) and c and d) )); --spravne
    f_g <= (( ((not b) and (not c) and (not d)) or (a and b and c and (not d)) or ((not a) and (not b) and c and d) )); --spravne

end Behavioral;


--PROTO ABYSTE MOHLI VYPLNIT TENTO SOUBOR JE NUTNE ZNAT BUDICI FUNKCE PRO DISPLEJ