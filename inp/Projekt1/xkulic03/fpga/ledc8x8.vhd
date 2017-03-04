library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
	port (-- Sem doplnte popis rozhrani obvodu.
		ROW : out std_logic_vector (0 to 7);
		LED : out std_logic_vector (0 to 7);
		RESET : in std_logic;
		SMCLK : in std_logic
	);
end ledc8x8;

architecture main of ledc8x8 is

	-- Sem doplnte definice vnitrnich signalu.
	signal stlpce : std_logic_vector(7 downto 0);
	signal riadky : std_logic_vector(7 downto 0);
	signal pocitadlo_riadky : std_logic_vector(7 downto 0);
	signal f_inicialky : std_logic_vector(15 downto 0);
	signal f_riadky: std_logic;

begin
	-- Sem doplnte popis funkce obvodu (zakladni konstrukce VHDL jako napr.
	-- prirazeni signalu, multiplexory, dekodery, procesy...).
	-- DODRZUJTE ZASADY PSANI SYNTETIZOVATELNEHO VHDL UVEDENE NA WEBU:
	-- http://merlin.fit.vutbr.cz/FITkit/docs/navody/synth_templates.html

	-- Nezapomente take doplnit mapovani signalu rozhrani na piny FPGA
	-- v souboru ledc8x8.ucf.
 

	-- Delic frekvencie SMCLK/256

	delic_f : process(SMCLK, RESET) -- odvodenie od SMCLK
	begin
		if RESET = '1' then
			pocitadlo_riadky <= "00000000";
		elsif SMCLK'EVENT and SMCLK = '1' then
			pocitadlo_riadky <= pocitadlo_riadky + 1;
		end if;
	end process delic_f;
	f_riadky <= '1' when pocitadlo_riadky = X"FF" else '0';
 
	-- Prepinanie riadkov a blikanie inicialok
	prepinac_riadkov : process(RESET, f_riadky, SMCLK)
	begin
		if RESET = '1' then --asynchrony reset
			f_inicialky <= (15 downto 0 => '0', others => '0');
			riadky <= "10000000";
		elsif SMCLK'EVENT and SMCLK = '1' and f_riadky= '1' then
			-- Delic frekvencie SMCLK/256/2^15 = potrebne pre blikanie inicialok vo frekvencii ~1Hz
			f_inicialky <= f_inicialky + 1; 
			riadky <= riadky(0) & riadky(7 downto 1);
		end if;
	end process prepinac_riadkov;
	
	svietenie : process(riadky)
	begin
		if f_inicialky(15) = '1' then

			case riadky is
				when "10000000" => stlpce <= "11111101";
				when "01000000" => stlpce <= "11111101";
				when "00100000" => stlpce <= "11111101";
				when "00010000" => stlpce <= "11111101";
				when "00001000" => stlpce <= "11111101";
				when "00000100" => stlpce <= "11011101";
				when "00000010" => stlpce <= "11011101";
				when "00000001" => stlpce <= "11100011";
				when others => stlpce <= "11111111";
			end case;
		else
			case riadky is
				when "10000000" => stlpce <= "11011101";
				when "01000000" => stlpce <= "11011011";
				when "00100000" => stlpce <= "11010111";
				when "00010000" => stlpce <= "11001111";
				when "00001000" => stlpce <= "11001111";
				when "00000100" => stlpce <= "11010111";
				when "00000010" => stlpce <= "11011011";
				when "00000001" => stlpce <= "11011101";
				when others => stlpce <= "11111111";
			end case;
		end if;
	end process svietenie;
 
	ROW <= riadky;
	LED <= stlpce;

end main;
