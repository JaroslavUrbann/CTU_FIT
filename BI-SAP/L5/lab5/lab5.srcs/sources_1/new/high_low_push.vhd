library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity high_low_push is
    Port ( 
         CLK        : in  STD_LOGIC;
         RST        : in  STD_LOGIC;
         BTN        : in  STD_LOGIC;
         VALUE      : out  STD_LOGIC;
         BTN_EVENT  : out  STD_LOGIC
     );
end high_low_push;

architecture Behavioral of high_low_push is

-----------------------------------------------------------
-- Kdo si chce hrát s èasováním, zde má možnost------------
-----------------------------------------------------------
    constant clk_freq       :   integer   := 100000000;
    constant counter_TOP    :   integer   := clk_freq/2;
-----------------------------------------------------------
-----------------------------------------------------------
-----------------------------------------------------------
    
    type execution_stage is (s0,s1,s2);


    signal  state, next_state           :  execution_stage;
    signal  sig_counter                 :  integer range 0 to counter_TOP;
    signal  sig_rst_counter, sig_count  :  std_logic;
    
begin

    proc_counter    :   process (CLK, RST, sig_count, sig_rst_counter)
    begin
        if (RST='1' or sig_rst_counter='1') then
            sig_counter <=  0;
        elsif (CLK='1' and CLK'event) then
             if (sig_counter < counter_TOP and sig_count='1') then
                sig_counter <= sig_counter + 1;
             end if;
        end if;
    end process;
    
    proc_fsm    :   process (CLK, RST)
    begin
        if (RST='1') then
            state   <=  s0;
        elsif (CLK='1' and CLK'event) then
            state   <=  next_state;
        end if;
    end process;
    
    proc_fsm_trans      :   process (state, BTN)
    begin
        
        case state is
            when    s0  =>
                next_state  <=  s0;
                if (BTN='1') then
                    next_state  <=  s1;
                end if;
            when    s1  =>
                next_state  <= s1;
                if (BTN='0') then
                    next_state  <= s2;
                end if;
            when    s2  =>
                next_state  <=  s0;
            when others =>
                next_state  <= s0;
        end case;
        
    end process;
    
    proc_fsm_output :   process (state, sig_counter)
    begin
    
        sig_count       <=  '0';
        sig_rst_counter <=  '0';
        VALUE           <=  '0';
        BTN_EVENT       <=  '0';
        
        case state is
            when    s0  =>
                sig_rst_counter <=  '1';
            when    s1  =>
                sig_count       <=  '1';
            when    s2  =>
                BTN_EVENT       <=  '1';
                if (sig_counter < counter_TOP) then
                    VALUE       <=  '0';
                else
                    VALUE       <=  '1';
                end if;
            when others =>
        end case;
        
    end process;

end Behavioral;
