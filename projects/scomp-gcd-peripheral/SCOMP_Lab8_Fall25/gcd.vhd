LIBRARY IEEE;

LIBRARY LPM;
 
USE IEEE.STD_LOGIC_1164.ALL;

USE IEEE.NUMERIC_STD.ALL;

USE LPM.LPM_COMPONENTS.ALL;
 
ENTITY gcd IS

    PORT ( 

        CLOCK   : IN    STD_LOGIC;

        RESETN  : IN    STD_LOGIC;
 
        IO_ADDR : IN    STD_LOGIC_VECTOR(10 DOWNTO 0);

        IO_DATA : INOUT STD_LOGIC_VECTOR(15 DOWNTO 0);

        IO_WRITE: IN    STD_LOGIC;

        IO_READ : IN    STD_LOGIC

    );
	 

END gcd;

ARCHITECTURE arch OF gcd IS

    SIGNAL MODE      : STD_LOGIC := '0';                             -- 0 = GCD, 1 = LCM

    SIGNAL INPUT_1   : STD_LOGIC_VECTOR(7 DOWNTO 0) := (OTHERS => '0');

    SIGNAL INPUT_2   : STD_LOGIC_VECTOR(7 DOWNTO 0) := (OTHERS => '0');
	 
	 SIGNAL START		: STD_LOGIC := '0'; -- setting this triggers gcd conversion

    SIGNAL OUTPUT    : STD_LOGIC_VECTOR(15 DOWNTO 0) := (OTHERS => '0');
	 
	 
	 SIGNAL IO_EN     : STD_LOGIC; -- tri-state functionality
	 
	 SIGNAL READ_DATA : STD_LOGIC_VECTOR(15 DOWNTO 0) := (OTHERS => '0');

	 TYPE state_type is (IDLE, LOAD, COMPARE, A_GT_B, A_LT_B, DONE); 
	 
	 -- A_GT_B : a > b
	 -- A_LT_B : b > a
	 
	 SIGNAL state		: state_type;
	 
	 -- registers for holding intermediate values during calculation
 
    SIGNAL a_reg   	: UNSIGNED(7 DOWNTO 0) := (OTHERS => '0'); 

    SIGNAL b_reg   	: UNSIGNED(7 DOWNTO 0) := (OTHERS => '0');
	 
	 SIGNAL gcd			: UNSIGNED(7 DOWNTO 0) := (OTHERS => '0');
	  
    SIGNAL in_progress : STD_LOGIC; -- to avoid being interrupted during GCD calculation

 begin
		io_bus: lpm_bustri
		generic map (
			lpm_width => 16
		)
		port map (
			data     => READ_DATA, -- this is what the peripheral is driving  when enable is active
			enabledt => IO_EN,
			tridata  => IO_DATA -- this is the input to our peripheral, when disabled, peripheral is listening in high z
		);
		
		
		
		IO_EN <= --signal controlling when to drive tridata with IO_DATA

         '1' WHEN (IO_READ = '1') AND

                 ( IO_ADDR = "00010010000" OR        -- 0x90 mode

                   IO_ADDR = "00010010001" OR        -- 0x91 input1

                   IO_ADDR = "00010010010" OR        -- 0x92 input2

                   IO_ADDR = "00010010100" )         -- 0x94 output

         ELSE '0';
			
			
			
 WRITE_PROC : PROCESS(CLOCK, RESETN, IO_WRITE)

 BEGIN
							
        IF RESETN = '0' THEN

            MODE      <= '0';

            INPUT_1   <= (OTHERS => '0');

            INPUT_2   <= (OTHERS => '0');
				
 
        ELSIF rising_edge(CLOCK) THEN
 
            IF (IO_WRITE = '1') THEN

                CASE IO_ADDR IS

                    WHEN "00010010000" =>          -- 0x90 : Mode register (LSB only)

                        MODE <= IO_DATA(0);
 
                    WHEN "00010010001" =>          -- 0x91 : Input1 (8-bit)

                        INPUT_1 <= IO_DATA(7 DOWNTO 0);
 
                    WHEN "00010010010" =>          -- 0x92 : Input2 (8-bit)

                        INPUT_2 <= IO_DATA(7 DOWNTO 0);
 
                    WHEN "00010010011" =>          -- 0x93 : Start register (LSB only)
								if in_progress = '0' then
									START <= IO_DATA(0);
								end if;
 
                    WHEN OTHERS =>

                        NULL;

                END CASE;

            END IF;

         END IF;

   END PROCESS WRITE_PROC;
	 
	 
	 
	WITH IO_ADDR SELECT

	   READ_DATA <=

			(15 DOWNTO 1 => '0') & MODE              WHEN "00010010000", -- READ_DATA = MODE when IO_ADDR == 0x90

			(15 DOWNTO 8 => '0') & INPUT_1           WHEN "00010010001", -- READ_DATA = INPUT_1 when IO_ADDR == 0x91

			(15 DOWNTO 8 => '0') & INPUT_2           WHEN "00010010010", -- READ_DATA = INPUT_2 when IO_ADDR == 0x92

			OUTPUT                                   WHEN "00010010100", -- READ_DATA = OUTPUT when IO_ADDR == 0x94

			(OTHERS => 'X')                          WHEN OTHERS;
				
				
			
	-- ============================================================================
	--  GCD/LCM FINITE STATE MACHINE
	--
	--  This FSM calculates either the GCD or LCM of two unsigned 8-bit integers stored
	--  in INPUT_1 and INPUT_2, and stores the output on OUTPUT.
	--
	--  FSM States:
	--    IDLE:       Transitions to LOAD when START = '1'. IN_PROGRESS remains '0'.
	--    LOAD:       Loads INPUT_1 into a_reg and INPUT_2 into b_reg.
	--                  Sets IN_PROGRESS = '1' and transitions to COMPARE.
	--    COMPARE:    Determines the next action:
	--                    - If a_reg = 0 -> GCD is b_reg -> DONE.
	--                    - If b_reg = 0 or a_reg = b_reg -> DONE.
	--                    - If a_reg > b_reg -> go to A_GT_B.
	--                    - Else → go to A_LT_B.
	--    A_GT_B:     Performs a_reg := a_reg - b_reg, then returns to COMPARE.
	--    A_LT_B:     Performs b_reg := b_reg - a_reg, then returns to COMPARE.
	--                  (Together, these states implement the subtraction-based
	--                   Euclidean algorithm.)
	--    DONE:       Produces output:
	--                    MODE = '0' → OUTPUT = GCD
	--                    MODE = '1' → OUTPUT = LCM = (INPUT_1 * INPUT_2) / GCD
	--                  Sets IN_PROGRESS = '0' and returns to IDLE.
	--
	--  NOTES:
	--    GCD is stored in a_reg upon completion.
	--    LCM computation avoids division-by-zero by checking a_reg = 0.
	-- ============================================================================

				
				
	PROCESS(CLOCK, RESETN)
	
	BEGIN
	
		IF (RESETN = '0') THEN 
		-- reset all variables used in FSM 
			STATE <= IDLE;
			a_reg <= (OTHERS => '0');
         b_reg <= (OTHERS => '0');
         OUTPUT <= (OTHERS => '0');
			IN_PROGRESS <= '0';
			
		ELSIF RISING_EDGE(CLOCK) THEN
		
			CASE STATE IS
				
				WHEN IDLE =>
				
					IF (START = '1') THEN STATE <= LOAD; -- begin calculation when start is high
					
					ELSE STATE <= IDLE;
					
					END IF;
					
				WHEN LOAD =>
				
					-- assign values to a_reg and b_reg from INPUT_1, and INPUT_2, respectively
					a_reg <= unsigned(INPUT_1);
					b_reg <= unsigned(INPUT_2);
				
					STATE <= COMPARE; -- begin comparing a and b
					IN_PROGRESS <= '1';
					
				WHEN COMPARE =>
				
					IF (a_reg = 0) THEN 
					
						a_reg <= b_reg;
						STATE <= DONE;
						
					ELSIF (b_reg = 0 OR a_reg = b_reg) THEN
					
						STATE <= DONE;
					
					ELSIF (a_reg > b_reg) THEN 
					
						STATE <= A_GT_B;
					
					ELSE 
					
						STATE <= A_LT_B;
					
					END IF;
					
				WHEN A_GT_B => 
				
					-- subtract B from A
					a_reg <= a_reg - b_reg;
					STATE <= COMPARE;
				
				WHEN A_LT_B => 
				
					-- subtract A from B
					b_reg <= b_reg - a_reg;
					STATE <= COMPARE;
				
				WHEN DONE => 
				
				-- set OUTPUT to GCD if MODE = 0
				IF (MODE = '0') THEN
				
					OUTPUT <= std_logic_vector(resize(a_reg, 16));
				
				-- calculate LCM and set OUTPUT to LCM if MODE = 1
				ELSE
					IF (a_reg = 0) THEN OUTPUT <= (OTHERS => '0');
					ELSE OUTPUT <= std_logic_vector( unsigned(INPUT_1)*unsigned(INPUT_2) / resize(a_reg, 16) ); -- LCM = (a*b)/gcd(a,b), gcd(a,b) != 0
					END IF;
					
				END IF;
				
				IN_PROGRESS <= '0';
				STATE <= IDLE;
				
			END CASE;
			
		END IF;
		
	END PROCESS;

END arch;
