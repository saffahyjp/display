-- 性能测试：
-- 6.016 s
-- 9.032 s
-- 4.032 s
-- 17.016 s
-- 7.032 s

-- 3
-- 5
-- 2.5
-- 6.5
-- 2.5

// #define DISABLE_FWD

#define slv(x) std_logic_vector(x downto 0)
#define addr_type slv(17)
#define data_type slv(15)
#define bool std_logic

#define ZERO (others => '0')
#define ONE (others => '1')
#define HIGH_Z (others => 'Z')
#define ONE_16b "0000000000000001"
#define ONE_18b "111111111111111111"

#define BF00 "001011111100000000"
#define BF01 "001011111100000001"

#define SEG_0 (not "1000000")
#define SEG_1 (not "1111001")
#define SEG_2 (not "0100100")
#define SEG_3 (not "0110000")
#define SEG_4 (not "0011001")
#define SEG_5 (not "0010010")
#define SEG_6 (not "0000010")
#define SEG_7 (not "1111000")
#define SEG_8 (not "0000000")
#define SEG_9 (not "0010000")

#define ALUOP_I1   "0000"
#define ALUOP_I2   "0001"
#define ALUOP_ADD  "0010"
#define ALUOP_SUB  "0011"
#define ALUOP_EQ   "0100"
#define ALUOP_NEQ  "0101"
#define ALUOP_LESS "0110"
#define ALUOP_AND  "1000"
#define ALUOP_OR   "1001"
#define ALUOP_SHL  "1100"
#define ALUOP_SHR  "1101"

#define ALUSRC_REG '0'
#define ALUSRC_OTH '1'

#define rid_type slv(3)
#define RID_NULL "0000"
#define RID_RA   "0001"
#define RID_SP   "0010"
#define RID_IH   "0011"
#define RID_T    "0100"
#define RID_R0   "1000"
#define RID_R1   "1001"
#define RID_R2   "1010"
#define RID_R3   "1011"
#define RID_R4   "1100"
#define RID_R5   "1101"
#define RID_R6   "1110"
#define RID_R7   "1111"

#define JUMP_NO  "00"
#define JUMP_ALU "01"
#define JUMP_RA  "10"
#define JUMP_RY  "11"
#define BR_NO  "00"
#define BR_EQ  "10"
#define BR_NEQ "11"

-- change log
-- 20151110 MI saffah
-- 20151113 BL saffah
-- TODO : 串口管理器
-- 20151117 第一阶段 saffah
-- 20151117 EXE saffah
-- 20151118 WB saffah
-- 20151118 Fwd saffah
-- 20151118 ID saffah
-- 20151118 JH saffah
-- 20151118 IF saffah
-- 20151118 MEM saffah

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity CPU is
	Port(
		clk : in bool; -- B9
		click : in bool; -- U9
		rst : in bool; -- U10
		seg1 : out slv(6); -- 0->6 C9 D9 E9 F9 G9 A10 B10
		seg2 : out slv(6); -- 0->6 C7 D7 E7 F7 A8 E8 F8
		sw : in data_type; -- 0->15 T7 R7 U8 V4 V3 V2 U1 R4 R1 N2 N1 M1 K7 K2 J7 J6
		l : out data_type; -- 0->15 D10 E10 A11 B11 C11 D11 E11 F11 A12 E12 F12 A13 B13 D13 E13 A14
		ram1addr : out addr_type; -- 0->17 C17 C18 D16 D17 E15 E16 F14 F15 F17 F18 G13 G14 G15 G16 H14 H15 H16 H17
		ram1data : inout data_type; -- 0->15 J12 J13 J14 J15 J16 J17 K12 K13 K14 K15 L15 L16 L17 L18 M13 M14
		ram1oe : out bool; -- M16
		ram1we : out bool; -- M18
		ram1en : out bool; -- M15
		ram2addr : out addr_type; -- 0->17 N14 N15 N18 P16 P17 P18 R15 R16 R18 T17 T18 U18 V15 V13 V12 V9 U16 U15
		ram2data : inout data_type; -- 0->15 U13 T16 T15 T14 T12 R13 R14 R12 R11 R10 P13 P12 P11 P10 N12 N11
		ram2oe : out bool; -- R9
		ram2we : out bool; -- M9
		ram2en : out bool; -- N10
		data_ready : in bool; -- A16
		rdn : out bool; -- C14
		tbre : in bool; -- D14
		tsre : in bool; -- N9
		wrn : out bool -- U5
	);
end CPU;

architecture Behavioral of CPU is

	signal clk_out : std_logic:='0';
	signal clk_out_x : std_logic:='0';
	signal clk_out_y : std_logic:='0';
	signal cnt : std_logic_vector(25 downto 0):="00000000000000000000000000";

	shared variable Boot_State : bool := '1';
	shared variable BL_State : bool := '1';

-- component CPU233 
-- port (CLKIN_IN,RST_IN : in std_logic;
-- CLKFX_OUT, CLKIN_IBUFG_OUT, LOCKED_OUT : out std_logic);
-- end component ;

begin

-- process(clk)
-- begin
	-- if (Boot_State = '1' and BL_State = '1') then
		-- if (clk'event and clk='1') then
			-- clk_out <= not clk_out;
		-- end if;
	-- else
		-- if (clk'event and clk='1') then
			-- cnt <= cnt + '1';
			-- if (cnt = "00000000000000000000000000") then
				-- clk_out <= '0';
			-- end if;
			-- if (cnt > "00000000000000010000000000") then
				-- cnt <= "00000000000000000000000000";
				-- clk_out <= '1';
			-- end if;
		-- end if;
	-- end if;
-- end process;

-- process(clk)
-- begin
	-- if (clk'event and clk='1') then
		-- cnt <= cnt + '1';
		-- if (cnt = "00000000000000000000000000") then
			-- clk_out <= '0';
		-- end if;
		-- if (Boot_State = '1' and BL_State = '1') then
			-- if (cnt > sw(15 downto 10) & "0" & sw(9) & "0" & sw(8) & "0" & sw(7) & "0" & sw(6) & "0" & sw(5) & "0" & sw(4) & "0" & sw(3) & "0" & sw(2) & "0" & sw(1) & "0" & sw(0)) then
				-- cnt <= "00000000000000000000000000";
				-- clk_out <= '1';
			-- end if;
		-- else
			-- if (cnt > sw(15 downto 10) & "0" & sw(9) & "0" & sw(8) & "0" & sw(7) & "0" & sw(6) & "0" & sw(5) & "0" & sw(4) & "0" & sw(3) & "0" & sw(2) & "0" & sw(1) & "0" & sw(0)) then
				-- cnt <= "00000000000000000000000000";
				-- clk_out <= '1';
			-- end if;
		-- end if;
	-- end if;
-- end process;

process(clk, rst)

	variable CLK_clk_ed : bool := '0';
	variable UART_read : bool := '0';

-- Memory Interface

-- 注意！只有MI_State = "000"才能修改各个输入端口

-- 输入端口：
	
	variable MI_Write_Addr : addr_type := ONE;
	variable MI_Write_Data : data_type := HIGH_Z;
	-- 如果不写入，则
	-- MI_Write_Addr = "11 1111 1111 1111 1111"
	-- MI_Write_Data = "ZZZZ ZZZZ ZZZZ ZZZZ"
	variable MI_Read_Addr_1 : addr_type := ONE;
	variable MI_Read_Data_1 : data_type := ZERO;
	variable MI_Read_Addr_2 : addr_type := ONE;
	variable MI_Read_Data_2 : data_type := ZERO;
	-- 务必把取IR放到端口2
	-- 务必把取IR放到端口2
	-- 务必把取IR放到端口2
	
	variable New_MI_Write_Addr : addr_type := ONE;
	variable New_MI_Write_Data : data_type := HIGH_Z;
	variable New_MI_Read_Addr_1 : addr_type := ONE;
	variable New_MI_Read_Data_1 : data_type := ZERO;
	variable New_MI_Read_Addr_2 : addr_type := ONE;
	variable New_MI_Read_Data_2 : data_type := ZERO;

-- 输出端口：
	
	variable New_Ram1Addr : addr_type := ONE;
	variable New_Ram1Data : data_type := ZERO;
	variable New_Ram2Addr : addr_type := ONE;
	variable New_Ram2Data : data_type := ZERO;
	variable New_Ram1WE : bool := '1';
	variable New_Ram1OE : bool := '1';
	variable New_Ram1EN : bool := '1';
	variable New_Ram2WE : bool := '1';
	variable New_Ram2OE : bool := '0';
	variable New_Ram2EN : bool := '0';
	variable New_wrn : bool := '1';
	variable New_rdn : bool := '1';

-- 内部变量：

	variable MI_State : slv(2) := ZERO;
	
	variable New_MI_State : slv(2) := ZERO;
	
-- Boot Loader

-- Boot_State为0表示正在boot，为1表示CPU正常工作
-- BL_State为0表示正在接收总字数，为1表示正在接收真正的内存字
-- BL_Recv_State为0表示正在接收高8位，为1表示低8位，2表示接收完毕存储
-- BL_Addr_Tot, BL_Addr_Cur表示总地址数和当前地址

	variable BL_Recv_State : slv(2) := ZERO;
	variable BL_Addr_Tot : addr_type := ZERO;
	variable BL_Addr_Cur : addr_type := ZERO;
	variable BL_Data : data_type := ZERO;

	variable New_Boot_State : bool := '1';
	variable New_BL_State : bool := '1';
	variable New_BL_Recv_State : slv(2) := ZERO;
	variable New_BL_Addr_Tot : addr_type := ZERO;
	variable New_BL_Addr_Cur : addr_type := ZERO;
	variable New_BL_Data : data_type := ZERO;

-- 调用MI实现对串口的访问

-- 协议格式：
-- 先接收一个字表示总字数，然后依次接收每个字
-- 每个字按照先高8位，后低8位的顺序接收

-- IF

-- 输入端口：
	-- IF_Jump_Addr : addr_type【WAIT】
	-- IF_Jump_Type : bool【WAIT】
	-- IF_Pause : bool【WAIT】
	-- MI_Read_Data_2 : data_type【WAIT】

-- 输出端口：
	variable ID_New_PC : addr_type := ZERO;
	variable ID_IR : data_type := ZERO;
	variable ID_Jump_Tar : addr_type := ZERO;
	-- variable MI_Read_Addr_2 : addr_type := ZERO;
	
	variable New_ID_New_PC : addr_type := ZERO;
	variable New_ID_IR : data_type := ZERO;
	variable New_ID_Jump_Tar : addr_type := ZERO;
	-- variable New_MI_Read_Addr_2 : addr_type := ZERO;

-- 内部变量：
	variable PC : addr_type := ONE;
	variable New_PC : addr_type := ONE;

-- ID

-- 输入端口：
	-- ID_New_PC : addr_type
	-- ID_IR : data_type
	-- ID_IR_Clear : bool
	-- ID_Wb_Rid : rid_type【WAIT】
	-- ID_Wb_Data : data_type【WAIT】
	-- Fwd_Rid_1 : rid_type【WAIT】
	-- Fwd_Data_1 : data_type【WAIT】
	-- Fwd_Data_1_av : bool【WAIT】
	-- Fwd_Rid_2 : rid_type【WAIT】
	-- Fwd_Data_2 : data_type【WAIT】

-- 输出端口：
	variable IF_Pause : bool := '0';
	variable EXE_New_PC : addr_type := ZERO;
	variable EXE_ALU_Op : slv(3) := ZERO;
	variable EXE_ALU_Src_1 : bool := '0';
	variable EXE_ALU_Src_2 : bool := '0';
	variable EXE_Reg_Data_1 : data_type := ZERO;
	variable EXE_Reg_Data_2 : data_type := ZERO;
	variable EXE_Imm : data_type := ZERO;
	variable EXE_Jump_Type : slv(1) := ZERO;
	variable EXE_Sw_Data : data_type := ZERO;
	variable EXE_Mem_Read : bool := '0';
	variable EXE_Mem_Write : bool := '0';
	variable EXE_Reg_Write : bool := '0';
	variable EXE_Wb_Rid : rid_type := ZERO;
	
	variable New_IF_Pause : bool := '0';
	variable New_EXE_New_PC : addr_type := ZERO;
	variable New_EXE_ALU_Op : slv(3) := ZERO;
	variable New_EXE_ALU_Src_1 : bool := '0';
	variable New_EXE_ALU_Src_2 : bool := '0';
	variable New_EXE_Reg_Data_1 : data_type := ZERO;
	variable New_EXE_Reg_Data_2 : data_type := ZERO;
	variable New_EXE_Imm : data_type := ZERO;
	variable New_EXE_Jump_Type : slv(1) := ZERO;
	variable New_EXE_Sw_Data : data_type := ZERO;
	variable New_EXE_Mem_Read : bool := '0';
	variable New_EXE_Mem_Write : bool := '0';
	variable New_EXE_Reg_Write : bool := '0';
	variable New_EXE_Wb_Rid : rid_type := ZERO;

-- 内部变量：
	-- 各种寄存器
	variable RA : data_type := ZERO;
	variable SP : data_type := ZERO;
	variable IH : data_type := ZERO;
	variable T  : data_type := ZERO;
	variable R0 : data_type := ZERO;
	variable R1 : data_type := ZERO;
	variable R2 : data_type := ZERO;
	variable R3 : data_type := ZERO;
	variable R4 : data_type := ZERO;
	variable R5 : data_type := ZERO;
	variable R6 : data_type := ZERO;
	variable R7 : data_type := ZERO;

-- EXE

-- 输入端口：
	-- EXE_New_PC : addr_type
	-- EXE_ALU_Op : slv(3)
	-- EXE_ALU_Src_1 : bool
	-- EXE_ALU_Src_2 : bool
	-- EXE_Reg_Data_1 : data_type
	-- EXE_Reg_Data_2 : data_type
	-- EXE_Imm : data_type
	-- EXE_Jump_Type : slv(1)
	-- EXE_Sw_Data : data_type
	-- EXE_Mem_Read : bool
	-- EXE_Mem_Write : bool
	-- EXE_Reg_Write : bool
	-- EXE_Wb_Rid : rid_type

-- 输出端口：
	variable MEM_ALU_Res : data_type := ZERO;
	variable MEM_Sw_Data : data_type := ZERO;
	variable MEM_Mem_Read : bool := '0';
	variable MEM_Mem_Write : bool := '0';
	variable MEM_Reg_Write : bool := '0';
	variable MEM_Wb_Rid : rid_type := ZERO;
	
	variable New_MEM_ALU_Res : data_type := ZERO;
	variable New_MEM_Sw_Data : data_type := ZERO;
	variable New_MEM_Mem_Read : bool := '0';
	variable New_MEM_Mem_Write : bool := '0';
	variable New_MEM_Reg_Write : bool := '0';
	variable New_MEM_Wb_Rid : rid_type := ZERO;

-- MEM

-- 输入端口：
	-- MEM_ALU_Res : data_type
	-- MEM_Sw_Data : data_type
	-- MEM_Mem_Read : bool
	-- MEM_Mem_Write : bool
	-- MEM_Reg_Write : bool
	-- MEM_Wb_Rid : rid_type
	-- MI_Read_Data_1 : data_type【WAIT】

-- 输出端口：
	-- variable MI_Read_Addr_1 : addr_type
	-- variable MI_Write_Addr : addr_type
	-- variable MI_Write_Data : data_type
	variable WB_Lw_Data : data_type := ZERO;
	variable WB_ALU_Res : data_type := ZERO;
	variable WB_Mem_Read : bool := '0';
	variable WB_Reg_Write : bool := '0';
	variable WB_Wb_Rid : rid_type := ZERO;
	
	variable New_WB_Lw_Data : data_type := ZERO;
	variable New_WB_ALU_Res : data_type := ZERO;
	variable New_WB_Mem_Read : bool := '0';
	variable New_WB_Reg_Write : bool := '0';
	variable New_WB_Wb_Rid : rid_type := ZERO;

-- WB

-- 输入端口：
	-- WB_Lw_Data : data_type
	-- WB_ALU_Res : data_type
	-- WB_Mem_Read : bool
	-- WB_Reg_Write : bool
	-- WB_Wb_Rid : rid_type

-- 输出端口
	variable ID_Wb_Rid : rid_type := ZERO;
	variable ID_Wb_Data : data_type := ZERO;
	
	variable New_ID_Wb_Rid : rid_type := ZERO;
	variable New_ID_Wb_Data : data_type := ZERO;

-- JH

-- 输入端口：
	-- EXE_New_PC : addr_type【WAIT】
	-- EXE_Imm : data_type【WAIT】
	-- EXE_Jump_Type : slv(1)【WAIT】
	-- EXE_Reg_Data_2 : data_type【WAIT】
	-- MEM_ALU_Res : data_type【WAIT】
	-- EXE_Jump_Type : slv(1)

-- 输出端口：
	variable IF_Jump_Addr : addr_type := ZERO;
	variable IF_Jump_Type : bool := '0';
	variable ID_IR_Clear : bool := '0';
	
	variable New_IF_Jump_Addr : addr_type := ZERO;
	variable New_IF_Jump_Type : bool := '0';
	variable New_ID_IR_Clear : bool := '0';

-- Fwd2

-- 输入端口：
	-- MEM_Mem_Read : bool
	-- MEM_Reg_Write : bool
	-- MEM_ALU_Res : data_type
	-- WB_Lw_Data : data_type【WAIT】
	-- MEM_Wb_Rid : rid_type

-- 输出端口：
	variable Fwd2_Rid : rid_type := ZERO;
	variable Fwd2_Data : data_type := ZERO;

-- Fwd

-- 输入端口：
	-- EXE_Mem_Read : bool
	-- EXE_Reg_Write : bool
	-- MEM_ALU_Res : data_type【WAIT】
	-- EXE_Wb_Rid : rid_type
	-- Fwd2_Rid : rid_type【WAIT】
	-- Fwd2_Data : data_type【WAIT】

-- 输出端口：
	variable Fwd_Rid_1 : rid_type := ZERO;
	variable Fwd_Data_1 : data_type := ZERO;
	variable Fwd_Data_1_av : bool := '1';
	variable Fwd_Rid_2 : rid_type := ZERO;
	variable Fwd_Data_2 : data_type := ZERO;



-- 所有的临时变量：
	variable T_ALU_Input_1 : data_type := ZERO;
	variable T_ALU_Input_2 : data_type := ZERO;
	variable T_ID_Rid_1 : rid_type := RID_NULL;
	variable T_ID_Rid_2 : rid_type := RID_NULL;
	variable T_RARead : bool := '0';
	variable T_RAWrite : bool := '0';
	variable T_Br_Type : slv(1) := ZERO;
	variable L_Br_Type : slv(1) := ZERO;
	variable Br_OPC : addr_type := ZERO;
	variable New_Br_OPC : addr_type := ZERO;
	variable uartcnt : bool := '0';
	variable New_uartcnt : bool := '0';
-- RyData

----
	
	variable Test_L : data_type := ZERO;

begin
	
	if(rst = '0') then
		
		CLK_clk_ed := '0';
		UART_read := '0';
		
		-- Memory Interface
		
		MI_Write_Addr := ONE;
		MI_Write_Data := HIGH_Z;
		MI_Read_Addr_1 := ONE;
		MI_Read_Data_1 := ZERO;
		MI_Read_Addr_2 := ONE;
		MI_Read_Data_2 := ZERO;
		New_MI_Write_Addr := ONE;
		New_MI_Write_Data := HIGH_Z;
		New_MI_Read_Addr_1 := ONE;
		New_MI_Read_Data_1 := ZERO;
		New_MI_Read_Addr_2 := ONE;
		New_MI_Read_Data_2 := ZERO;
		
		New_Ram1Addr := ZERO;
		New_Ram1Data := ZERO;
		New_Ram2Addr := ZERO;
		New_Ram2Data := ZERO;
		New_Ram1WE := '1';
		New_Ram1OE := '1';
		New_Ram1EN := '1';
		New_Ram2WE := '1';
		New_Ram2OE := '0';
		New_Ram2EN := '0';
		New_wrn := '1';
		New_rdn := '1';
		
		MI_State := ZERO;
		New_MI_State := ZERO;
		
		-- Boot Loader
		
		Boot_State := '1';
		BL_State := '1';
		BL_Recv_State := ZERO;
		BL_Addr_Tot := ZERO;
		BL_Addr_Cur := ZERO;
		BL_Data := ZERO;
		New_Boot_State := '1';
		New_BL_State := '1';
		New_BL_Recv_State := ZERO;
		New_BL_Addr_Tot := ZERO;
		New_BL_Addr_Cur := ZERO;
		New_BL_Data := ZERO;
		
		ID_New_PC := ONE;
		ID_IR := ZERO;
		ID_Jump_Tar := ZERO;
		New_ID_New_PC := ONE;
		New_ID_IR := ZERO;
		New_ID_Jump_Tar := ZERO;
		
		PC := ONE;
		New_PC := ONE;
		
		IF_Pause := '0';
		EXE_New_PC := ONE;
		EXE_ALU_Op := ZERO;
		EXE_ALU_Src_1 := '0';
		EXE_ALU_Src_2 := '0';
		EXE_Reg_Data_1 := ZERO;
		EXE_Reg_Data_2 := ZERO;
		EXE_Imm := ZERO;
		EXE_Jump_Type := ZERO;
		EXE_Sw_Data := ZERO;
		EXE_Mem_Read := '0';
		EXE_Mem_Write := '0';
		EXE_Reg_Write := '0';
		EXE_Wb_Rid := ZERO;
		New_IF_Pause := '0';
		New_EXE_New_PC := ONE;
		New_EXE_ALU_Op := ZERO;
		New_EXE_ALU_Src_1 := '0';
		New_EXE_ALU_Src_2 := '0';
		New_EXE_Reg_Data_1 := ZERO;
		New_EXE_Reg_Data_2 := ZERO;
		New_EXE_Imm := ZERO;
		New_EXE_Jump_Type := ZERO;
		New_EXE_Sw_Data := ZERO;
		New_EXE_Mem_Read := '0';
		New_EXE_Mem_Write := '0';
		New_EXE_Reg_Write := '0';
		New_EXE_Wb_Rid := ZERO;
		
		RA := ZERO;
		SP := ZERO;
		IH := ZERO;
		T  := ZERO;
		R0 := ZERO;
		R1 := ZERO;
		R2 := ZERO;
		R3 := ZERO;
		R4 := ZERO;
		R5 := ZERO;
		R6 := ZERO;
		R7 := ZERO;
		
		MEM_ALU_Res := ZERO;
		MEM_Sw_Data := ZERO;
		MEM_Mem_Read := '0';
		MEM_Mem_Write := '0';
		MEM_Reg_Write := '0';
		MEM_Wb_Rid := ZERO;
		New_MEM_ALU_Res := ZERO;
		New_MEM_Sw_Data := ZERO;
		New_MEM_Mem_Read := '0';
		New_MEM_Mem_Write := '0';
		New_MEM_Reg_Write := '0';
		New_MEM_Wb_Rid := ZERO;
		
		WB_Lw_Data := ZERO;
		WB_ALU_Res := ZERO;
		WB_Mem_Read := '0';
		WB_Reg_Write := '0';
		WB_Wb_Rid := ZERO;
		New_WB_Lw_Data := ZERO;
		New_WB_ALU_Res := ZERO;
		New_WB_Mem_Read := '0';
		New_WB_Reg_Write := '0';
		New_WB_Wb_Rid := ZERO;
		
		ID_Wb_Rid := ZERO;
		ID_Wb_Data := ZERO;
		New_ID_Wb_Rid := ZERO;
		New_ID_Wb_Data := ZERO;
		
		IF_Jump_Addr := ZERO;
		IF_Jump_Type := '0';
		ID_IR_Clear := '0';
		New_IF_Jump_Addr := ZERO;
		New_IF_Jump_Type := '0';
		New_ID_IR_Clear := '0';
		
		Fwd2_Rid := ZERO;
		Fwd2_Data := ZERO;
		
		Fwd_Rid_1 := ZERO;
		Fwd_Data_1 := ZERO;
		Fwd_Data_1_av := '1';
		Fwd_Rid_2 := ZERO;
		Fwd_Data_2 := ZERO;
		
		T_ALU_Input_1 := ZERO;
		T_ALU_Input_2 := ZERO;
		T_ID_Rid_1 := RID_NULL;
		T_ID_Rid_2 := RID_NULL;
		T_RARead := '0';
		T_RAWrite := '0';
		T_Br_Type := ZERO;
		L_Br_Type := ZERO;
		Br_OPC := ZERO;
		New_Br_OPC := ZERO;
		
		Test_L := ZERO;
		
	else
		if(clk'event and clk = '1') then
			-- CLK_cnt := CLK_cnt + '1';
			-- if(CLK_cnt(21 downto 0) > sw & "000000") then
				-- CLK_cnt := ZERO;
				if(MI_State = "000") then
					New_MI_Read_Data_2 := Ram2Data;
					if(UART_read = '1') then
						New_rdn := '1';
						New_MI_Read_Data_1 := Ram1Data;
						UART_read := '0';
					end if;
					-- MI干完活以后清空write线 20151113 saffah
					New_MI_Write_Addr := ONE;
					New_MI_Write_Data := HIGH_Z;
					New_MI_Read_Addr_1 := ONE;
					New_MI_Read_Addr_2 := ONE;
				
					MI_Write_Addr  := New_MI_Write_Addr ;
					MI_Write_Data  := New_MI_Write_Data ;
					MI_Read_Addr_1 := New_MI_Read_Addr_1;
					MI_Read_Data_1 := New_MI_Read_Data_1;
					MI_Read_Addr_2 := New_MI_Read_Addr_2;
					MI_Read_Data_2 := New_MI_Read_Data_2;
				
					MI_State := New_MI_State;
					
					New_MI_Read_Data_1 := ZERO;
					New_MI_Read_Data_2 := ZERO;
					-- 现在才能进行正常的事情
					-- 也就是CPU开始干活了
					-- 然而CPU并不能开始干活
					-- 因为Boot Loader要干活了
					-- 然而Boot Loader并没有啥可以做的
					if(Boot_State = '1' and BL_State = '1' and CLK_clk_ed = '1') then
						-- 现在CPU可以开始干活了
						-- 但是只能做第二阶段的事情
						
						-- 总执行时序
							-- 【done】IF和MEM只进行读写MI
							-- 【done】MI工作
							-- 确定New_WB_Lw_Data
							-- EXE工作
							-- WB工作
							-- Fwd2工作
							-- Fwd工作
							-- ID工作
							-- JH工作
							-- IF和MEM把剩余工作做完
						-- OK
						
						New_WB_Lw_Data := MI_Read_Data_1;
						
						-- 总执行时序
							-- 【done】IF和MEM只进行读写MI
							-- 【done】MI工作
							-- 【done】确定New_WB_Lw_Data
							-- EXE工作
							-- WB工作
							-- Fwd2工作
							-- Fwd工作
							-- ID工作
							-- JH工作
							-- IF和MEM把剩余工作做完
						-- OK
						
						-- Begin component EXE
						-- 输入端口
							-- EXE_New_PC : addr_type
							-- EXE_ALU_Op : slv(3)
							-- EXE_ALU_Src_1 : bool
							-- EXE_ALU_Src_2 : bool
							-- EXE_Reg_Data_1 : data_type
							-- EXE_Reg_Data_2 : data_type
							-- EXE_Imm : data_type
							-- EXE_Jump_Type : slv(1)
							-- EXE_Sw_Data : data_type
							-- EXE_Mem_Read : bool
							-- EXE_Mem_Write : bool
							-- EXE_Reg_Write : bool
							-- EXE_Wb_Rid : rid_type
						-- 输出端口
							-- MEM_ALU_Res : data_type
							-- MEM_Sw_Data : data_type
							-- MEM_Mem_Read : bool
							-- MEM_Mem_Write : bool
							-- MEM_Reg_Write : bool
							-- MEM_Wb_Rid : rid_type
						
						-- 5个信号直接后传
						New_MEM_Sw_Data   := EXE_Sw_Data  ;
						New_MEM_Mem_Read  := EXE_Mem_Read ;
						New_MEM_Mem_Write := EXE_Mem_Write;
						New_MEM_Reg_Write := EXE_Reg_Write;
						New_MEM_Wb_Rid    := EXE_Wb_Rid   ;
						
						-- 计算ALUInput
						if(EXE_ALU_Src_1 = ALUSRC_REG) then
							T_ALU_Input_1 := EXE_Reg_Data_1;
						else
							T_ALU_Input_1 := EXE_New_PC(15 downto 0);
						end if;
						if(EXE_ALU_Src_2 = ALUSRC_REG) then
							T_ALU_Input_2 := EXE_Reg_Data_2;
						else
							T_ALU_Input_2 := EXE_Imm;
						end if;
						
						-- 计算ALURes
						case EXE_ALU_Op is
							when ALUOP_I1 =>
								New_MEM_ALU_Res := T_ALU_Input_1;
							when ALUOP_I2 =>
								New_MEM_ALU_Res := T_ALU_Input_2;
							when ALUOP_ADD =>
								New_MEM_ALU_Res := T_ALU_Input_1 + T_ALU_Input_2;
							when ALUOP_SUB =>
								New_MEM_ALU_Res := T_ALU_Input_1 - T_ALU_Input_2;
							when ALUOP_EQ =>
								if(T_ALU_Input_1 = T_ALU_Input_2) then
									New_MEM_ALU_Res := ONE_16b;
								else
									New_MEM_ALU_Res := ZERO;
								end if;
							when ALUOP_NEQ =>
								if(T_ALU_Input_1 = T_ALU_Input_2) then
									New_MEM_ALU_Res := ZERO;
								else
									New_MEM_ALU_Res := ONE_16b;
								end if;
							when ALUOP_LESS =>
								-- SIGNED!!! SIGNED!!! SIGNED!!!
								if(T_ALU_Input_1(15) = T_ALU_Input_2(15)) then
									if(T_ALU_Input_1 < T_ALU_Input_2) then
										New_MEM_ALU_Res := ONE_16b;
									else
										New_MEM_ALU_Res := ZERO;
									end if;
								else
									if(T_ALU_Input_1(15) = '1') then
										New_MEM_ALU_Res := ONE_16b;
									else
										New_MEM_ALU_Res := ZERO;
									end if;
								end if;
							when ALUOP_AND =>
								New_MEM_ALU_Res := T_ALU_Input_1 and T_ALU_Input_2;
							when ALUOP_OR =>
								New_MEM_ALU_Res := T_ALU_Input_1 or T_ALU_Input_2;
							when ALUOP_SHL =>
								case T_ALU_Input_2(2 downto 0) is
									when "001" =>
										New_MEM_ALU_Res := T_ALU_Input_1(14 downto 0) & "0"       ;
									when "010" =>
										New_MEM_ALU_Res := T_ALU_Input_1(13 downto 0) & "00"      ;
									when "011" =>
										New_MEM_ALU_Res := T_ALU_Input_1(12 downto 0) & "000"     ;
									when "100" =>
										New_MEM_ALU_Res := T_ALU_Input_1(11 downto 0) & "0000"    ;
									when "101" =>
										New_MEM_ALU_Res := T_ALU_Input_1(10 downto 0) & "00000"   ;
									when "110" =>
										New_MEM_ALU_Res := T_ALU_Input_1( 9 downto 0) & "000000"  ;
									when "111" =>
										New_MEM_ALU_Res := T_ALU_Input_1( 8 downto 0) & "0000000" ;
									when "000" =>
										New_MEM_ALU_Res := T_ALU_Input_1( 7 downto 0) & "00000000";
									when others =>
								end case;
							when ALUOP_SHR =>
								if(T_ALU_Input_1(15) = '0') then
									case T_ALU_Input_2(2 downto 0) is
										when "001" =>
											New_MEM_ALU_Res := "0"        & T_ALU_Input_1(15 downto 1);
										when "010" =>
											New_MEM_ALU_Res := "00"       & T_ALU_Input_1(15 downto 2);
										when "011" =>
											New_MEM_ALU_Res := "000"      & T_ALU_Input_1(15 downto 3);
										when "100" =>
											New_MEM_ALU_Res := "0000"     & T_ALU_Input_1(15 downto 4);
										when "101" =>
											New_MEM_ALU_Res := "00000"    & T_ALU_Input_1(15 downto 5);
										when "110" =>
											New_MEM_ALU_Res := "000000"   & T_ALU_Input_1(15 downto 6);
										when "111" =>
											New_MEM_ALU_Res := "0000000"  & T_ALU_Input_1(15 downto 7);
										when "000" =>
											New_MEM_ALU_Res := "00000000" & T_ALU_Input_1(15 downto 8);
										when others =>
									end case;
								else
									case T_ALU_Input_2(2 downto 0) is
										when "001" =>
											New_MEM_ALU_Res := "1"        & T_ALU_Input_1(15 downto 1);
										when "010" =>
											New_MEM_ALU_Res := "11"       & T_ALU_Input_1(15 downto 2);
										when "011" =>
											New_MEM_ALU_Res := "111"      & T_ALU_Input_1(15 downto 3);
										when "100" =>
											New_MEM_ALU_Res := "1111"     & T_ALU_Input_1(15 downto 4);
										when "101" =>
											New_MEM_ALU_Res := "11111"    & T_ALU_Input_1(15 downto 5);
										when "110" =>
											New_MEM_ALU_Res := "111111"   & T_ALU_Input_1(15 downto 6);
										when "111" =>
											New_MEM_ALU_Res := "1111111"  & T_ALU_Input_1(15 downto 7);
										when "000" =>
											New_MEM_ALU_Res := "11111111" & T_ALU_Input_1(15 downto 8);
										when others =>
									end case;
								end if;
							when others =>
								New_MEM_ALU_Res := ZERO;
						end case;
						
						-- End component EXE
						
						-- 总执行时序
							-- 【done】IF和MEM只进行读写MI
							-- 【done】MI工作
							-- 【done】确定New_WB_Lw_Data
							-- 【done】EXE工作
							-- WB工作
							-- Fwd2工作
							-- Fwd工作
							-- ID工作
							-- JH工作
							-- IF和MEM把剩余工作做完
						-- OK
						
						-- Begin component WB
						-- 输入端口
							-- WB_Lw_Data : data_type
							-- WB_ALU_Res : data_type
							-- WB_Mem_Read : bool
							-- WB_Reg_Write : bool
							-- WB_Wb_Rid : rid_type
						-- 输出端口
							-- ID_Wb_Rid : rid_type
							-- ID_Wb_Data : data_type
						
						if(WB_Mem_Read = '1') then
							New_ID_Wb_Data := WB_Lw_Data;
						else
							New_ID_Wb_Data := WB_ALU_Res;
						end if;
						
						if(WB_Reg_Write = '1') then
							New_ID_Wb_Rid := WB_Wb_Rid;
						else
							New_ID_Wb_Rid := RID_NULL;
						end if;
						-- End component WB
						
						-- 总执行时序
							-- 【done】IF和MEM只进行读写MI
							-- 【done】MI工作
							-- 【done】确定New_WB_Lw_Data
							-- 【done】EXE工作
							-- 【done】WB工作
							-- Fwd2工作
							-- Fwd工作
							-- ID工作
							-- JH工作
							-- IF和MEM把剩余工作做完
						-- OK
						
						-- Begin component Fwd2
						-- 输入端口
							-- MEM_Mem_Read : bool
							-- MEM_Reg_Write : bool
							-- MEM_ALU_Res : data_type
							-- WB_Lw_Data : data_type【WAIT】
							-- MEM_Wb_Rid : rid_type
						-- 输出端口
							-- Fwd2_Rid : rid_type
							-- Fwd2_Data : data_type
						-- 注意Fwd2不参与时序逻辑，即Fwd2整体是一个临时变量
						-- 所以可以对非New变量赋值
						
						-- 所有标有【WAIT】的输入端口，除了MI端口外
						-- 其余的在引用时必须加New_
	#ifdef DISABLE_FWD
						Fwd2_Rid := RID_NULL;
						Fwd2_Data := ZERO;
	#else
						if(MEM_Reg_Write = '1') then
							Fwd2_Rid := MEM_Wb_Rid;
							if(MEM_Mem_Read = '1') then
								Fwd2_Data := New_WB_Lw_Data;
							else
								Fwd2_Data := MEM_ALU_Res;
							end if;
						else
							Fwd2_Rid := RID_NULL;
							Fwd2_Data := ZERO;
						end if;
	#endif
						-- End component Fwd2
						
						-- 总执行时序
							-- 【done】IF和MEM只进行读写MI
							-- 【done】MI工作
							-- 【done】确定New_WB_Lw_Data
							-- 【done】EXE工作
							-- 【done】WB工作
							-- 【done】Fwd2工作
							-- Fwd工作
							-- ID工作
							-- JH工作
							-- IF和MEM把剩余工作做完
						-- OK
						
						-- Begin component Fwd
						-- 输入端口
							-- EXE_Mem_Read : bool
							-- EXE_Reg_Write : bool
							-- MEM_ALU_Res : data_type【WAIT】
							-- EXE_Wb_Rid : rid_type
							-- Fwd2_Rid : rid_type【WAIT】
							-- Fwd2_Data : data_type【WAIT】
						-- 输出端口
							-- Fwd_Rid_1 : rid_type
							-- Fwd_Data_1 : data_type
							-- Fwd_Data_1_av : bool
							-- Fwd_Rid_2 : rid_type
							-- Fwd_Data_2 : data_type
						-- 由于Fwd2是临时变量，所以
						-- 引用Fwd2_Rid与Fwd2_Data不加New
						-- 同理Fwd本身是临时的，所有输出端也不用New
						
	#ifdef DISABLE_FWD
						Fwd_Rid_1 := RID_NULL;
						Fwd_Data_1 := ZERO;
						Fwd_Data_1_av := '1';
						Fwd_Rid_2 := RID_NULL;
						Fwd_Data_2 := ZERO;
	#else
						if(EXE_Reg_Write = '1') then
							Fwd_Rid_1 := EXE_Wb_Rid;
							if(EXE_Mem_Read = '1') then
								Fwd_Data_1 := ZERO;
								Fwd_Data_1_av := '0';
							else
								Fwd_Data_1 := New_MEM_ALU_Res;
								Fwd_Data_1_av := '1';
							end if;
						else
							Fwd_Rid_1 := RID_NULL;
							Fwd_Data_1 := ZERO;
							Fwd_Data_1_av := '1';
						end if;
						-- 有两个Rid相同的Fwd，则选取后一条指令Fwd
						if(Fwd2_Rid = Fwd_Rid_1) then
							Fwd_Rid_2 := RID_NULL;
							Fwd_Data_2 := ZERO;
						else
							Fwd_Rid_2 := Fwd2_Rid;
							Fwd_Data_2 := Fwd2_Data;
						end if;
	#endif
						
						-- End component Fwd
						
						-- 总执行时序
							-- 【done】IF和MEM只进行读写MI
							-- 【done】MI工作
							-- 【done】确定New_WB_Lw_Data
							-- 【done】EXE工作
							-- 【done】WB工作
							-- 【done】Fwd2工作
							-- 【done】Fwd工作
							-- ID工作
							-- JH工作
							-- IF和MEM把剩余工作做完
						-- OK
						
						-- Begin component ID
						-- 输入端口
							-- ID_New_PC : addr_type
							-- ID_IR : data_type
							-- ID_IR_Clear : bool
							-- ID_Wb_Rid : rid_type【WAIT】
							-- ID_Wb_Data : data_type【WAIT】
							-- Fwd_Rid_1 : rid_type【WAIT】
							-- Fwd_Data_1 : data_type【WAIT】
							-- Fwd_Data_1_av : bool【WAIT】
							-- Fwd_Rid_2 : rid_type【WAIT】
							-- Fwd_Data_2 : data_type【WAIT】
						-- 输出端口
							-- IF_Pause : bool
							-- EXE_New_PC : addr_type
							-- EXE_ALU_Op : slv
							-- EXE_ALU_Src_1 : bool
							-- EXE_ALU_Src_2 : bool
							-- EXE_Reg_Data_1 : data_type
							-- EXE_Reg_Data_2 : data_type
							-- EXE_Imm : data_type
							-- EXE_Jump_Type : slv(1)
							-- EXE_Sw_Data : data_type
							-- EXE_Mem_Read : bool
							-- EXE_Mem_Write : bool
							-- EXE_Reg_Write : bool
							-- EXE_Wb_Rid : rid_type
						-- 内部状态变量
							-- RA, SP, IH, T, R0-R7
						
						-- 无论如何，先写寄存器
						case New_ID_Wb_Rid is
							when RID_NULL =>
							when RID_RA   => RA := New_ID_Wb_Data;
							when RID_SP   => SP := New_ID_Wb_Data;
							when RID_IH   => IH := New_ID_Wb_Data;
							when RID_T    => T  := New_ID_Wb_Data;
							when RID_R0   => R0 := New_ID_Wb_Data;
							-- DEBUG
							Test_L := R0(7 downto 0) & Br_OPC(7 downto 0);
							when RID_R1   => R1 := New_ID_Wb_Data;
							when RID_R2   => R2 := New_ID_Wb_Data;
							when RID_R3   => R3 := New_ID_Wb_Data;
							when RID_R4   => R4 := New_ID_Wb_Data;
							when RID_R5   => R5 := New_ID_Wb_Data;
							when RID_R6   => R6 := New_ID_Wb_Data;
							when RID_R7   => R7 := New_ID_Wb_Data;
							when others   =>
						end case;
						if(ID_IR_Clear = '1') then
							-- 强制被清为NOP
							New_EXE_New_PC := ZERO;
							New_EXE_ALU_Op := ALUOP_I1;
							New_EXE_ALU_Src_1 := ALUSRC_REG;
							New_EXE_ALU_Src_2 := ALUSRC_REG;
							New_EXE_Reg_Data_1 := ZERO;
							New_EXE_Reg_Data_2 := ZERO;
							New_EXE_Imm := ZERO;
							New_EXE_Jump_Type := JUMP_NO;
							New_EXE_Sw_Data := ZERO;
							New_EXE_Mem_Read := '0';
							New_EXE_Mem_Write := '0';
							New_EXE_Reg_Write := '0';
							New_EXE_Wb_Rid := RID_NULL;
						else
							-- 没有被IRClear，进行指令译码
							case ID_IR(15 downto 11) is
								when "00001" =>
									-- NOP 000 000 00000
									New_EXE_ALU_Op    := ALUOP_I1;
									New_EXE_ALU_Src_1 := ALUSRC_REG;
									New_EXE_ALU_Src_2 := ALUSRC_REG;
									New_EXE_Imm       := ZERO;
									New_EXE_Jump_Type := JUMP_NO;
									New_EXE_Mem_Read  := '0';
									New_EXE_Mem_Write := '0';
									New_EXE_Reg_Write := '0';
									New_EXE_Wb_Rid    := RID_NULL;
									T_ID_Rid_1        := RID_NULL;
									T_ID_Rid_2        := RID_NULL;
									T_RARead          := '0';
									T_RAWrite         := '0';
									T_Br_Type         := BR_NO;
								when "00010" =>
									-- B imm11
									New_EXE_ALU_Op    := ALUOP_ADD;
									New_EXE_ALU_Src_1 := ALUSRC_OTH;
									New_EXE_ALU_Src_2 := ALUSRC_OTH;
									if(ID_IR(10) = '0') then
										New_EXE_Imm := "00000" & ID_IR(10 downto 0);
									else
										New_EXE_Imm := "11111" & ID_IR(10 downto 0);
									end if;
									New_EXE_Jump_Type := JUMP_ALU;
									New_EXE_Mem_Read  := '0';
									New_EXE_Mem_Write := '0';
									New_EXE_Reg_Write := '0';
									New_EXE_Wb_Rid    := RID_NULL;
									T_ID_Rid_1        := RID_NULL;
									T_ID_Rid_2        := RID_NULL;
									T_RARead          := '0';
									T_RAWrite         := '0';
									T_Br_Type         := BR_NO;
								when "00100" =>
									-- BEQZ rx imm8
									New_EXE_ALU_Op    := ALUOP_ADD;
									New_EXE_ALU_Src_1 := ALUSRC_OTH;
									New_EXE_ALU_Src_2 := ALUSRC_OTH;
									if(ID_IR(7) = '0') then
										New_EXE_Imm := "00000000" & ID_IR(7 downto 0);
									else
										New_EXE_Imm := "11111111" & ID_IR(7 downto 0);
									end if;
									New_EXE_Jump_Type := JUMP_ALU;
									New_EXE_Mem_Read  := '0';
									New_EXE_Mem_Write := '0';
									New_EXE_Reg_Write := '0';
									New_EXE_Wb_Rid    := RID_NULL;
									T_ID_Rid_1        := RID_NULL;
									T_ID_Rid_2        := "1" & ID_IR(10 downto 8);
									T_RARead          := '0';
									T_RAWrite         := '0';
									T_Br_Type         := BR_EQ;
								when "00101" =>
									-- BNEZ rx imm8
									New_EXE_ALU_Op    := ALUOP_ADD;
									New_EXE_ALU_Src_1 := ALUSRC_OTH;
									New_EXE_ALU_Src_2 := ALUSRC_OTH;
									if(ID_IR(7) = '0') then
										New_EXE_Imm := "00000000" & ID_IR(7 downto 0);
									else
										New_EXE_Imm := "11111111" & ID_IR(7 downto 0);
									end if;
									New_EXE_Jump_Type := JUMP_ALU;
									New_EXE_Mem_Read  := '0';
									New_EXE_Mem_Write := '0';
									New_EXE_Reg_Write := '0';
									New_EXE_Wb_Rid    := RID_NULL;
									T_ID_Rid_1        := RID_NULL;
									T_ID_Rid_2        := "1" & ID_IR(10 downto 8);
									T_RARead          := '0';
									T_RAWrite         := '0';
									T_Br_Type         := BR_NEQ;
								when "00110" =>
									case ID_IR(1 downto 0) is
										when "00" =>
											-- SLL rx ry imm3 00
											New_EXE_ALU_Op    := ALUOP_SHL;
											New_EXE_ALU_Src_1 := ALUSRC_REG;
											New_EXE_ALU_Src_2 := ALUSRC_OTH;
											New_EXE_Imm       := "0000000000000" & ID_IR(4 downto 2);
											New_EXE_Jump_Type := JUMP_NO;
											New_EXE_Mem_Read  := '0';
											New_EXE_Mem_Write := '0';
											New_EXE_Reg_Write := '1';
											New_EXE_Wb_Rid    := "1" & ID_IR(10 downto 8);
											T_ID_Rid_1        := "1" & ID_IR(7 downto 5);
											T_ID_Rid_2        := RID_NULL;
											T_RARead          := '0';
											T_RAWrite         := '0';
											T_Br_Type         := BR_NO;
										when "11" =>
											-- SRA rx ry imm3 11
											New_EXE_ALU_Op    := ALUOP_SHR;
											New_EXE_ALU_Src_1 := ALUSRC_REG;
											New_EXE_ALU_Src_2 := ALUSRC_OTH;
											New_EXE_Imm       := "0000000000000" & ID_IR(4 downto 2);
											New_EXE_Jump_Type := JUMP_NO;
											New_EXE_Mem_Read  := '0';
											New_EXE_Mem_Write := '0';
											New_EXE_Reg_Write := '1';
											New_EXE_Wb_Rid    := "1" & ID_IR(10 downto 8);
											T_ID_Rid_1        := "1" & ID_IR(7 downto 5);
											T_ID_Rid_2        := RID_NULL;
											T_RARead          := '0';
											T_RAWrite         := '0';
											T_Br_Type         := BR_NO;
										when others =>
									end case;
								when "01000" =>
									-- ADDIU3 rx ry 0 imm4
									New_EXE_ALU_Op    := ALUOP_ADD;
									New_EXE_ALU_Src_1 := ALUSRC_REG;
									New_EXE_ALU_Src_2 := ALUSRC_OTH;
									if(ID_IR(3) = '0') then
										New_EXE_Imm := "000000000000" & ID_IR(3 downto 0);
									else
										New_EXE_Imm := "111111111111" & ID_IR(3 downto 0);
									end if;
									New_EXE_Jump_Type := JUMP_NO;
									New_EXE_Mem_Read  := '0';
									New_EXE_Mem_Write := '0';
									New_EXE_Reg_Write := '1';
									New_EXE_Wb_Rid    := "1" & ID_IR(7 downto 5);
									T_ID_Rid_1        := "1" & ID_IR(10 downto 8);
									T_ID_Rid_2        := RID_NULL;
									T_RARead          := '0';
									T_RAWrite         := '0';
									T_Br_Type         := BR_NO;
								when "01001" =>
									-- ADDIU rx imm8
									New_EXE_ALU_Op    := ALUOP_ADD;
									New_EXE_ALU_Src_1 := ALUSRC_REG;
									New_EXE_ALU_Src_2 := ALUSRC_OTH;
									if(ID_IR(7) = '0') then
										New_EXE_Imm := "00000000" & ID_IR(7 downto 0);
									else
										New_EXE_Imm := "11111111" & ID_IR(7 downto 0);
									end if;
									New_EXE_Jump_Type := JUMP_NO;
									New_EXE_Mem_Read  := '0';
									New_EXE_Mem_Write := '0';
									New_EXE_Reg_Write := '1';
									New_EXE_Wb_Rid    := "1" & ID_IR(10 downto 8);
									T_ID_Rid_1        := "1" & ID_IR(10 downto 8);
									T_ID_Rid_2        := RID_NULL;
									T_RARead          := '0';
									T_RAWrite         := '0';
									T_Br_Type         := BR_NO;
								when "01010" =>
									-- SLTI rx imm8
									New_EXE_ALU_Op    := ALUOP_LESS;
									New_EXE_ALU_Src_1 := ALUSRC_REG;
									New_EXE_ALU_Src_2 := ALUSRC_OTH;
									if(ID_IR(7) = '0') then
										New_EXE_Imm := "00000000" & ID_IR(7 downto 0);
									else
										New_EXE_Imm := "11111111" & ID_IR(7 downto 0);
									end if;
									New_EXE_Jump_Type := JUMP_NO;
									New_EXE_Mem_Read  := '0';
									New_EXE_Mem_Write := '0';
									New_EXE_Reg_Write := '1';
									New_EXE_Wb_Rid    := RID_T;
									T_ID_Rid_1        := "1" & ID_IR(10 downto 8);
									T_ID_Rid_2        := RID_NULL;
									T_RARead          := '0';
									T_RAWrite         := '0';
									T_Br_Type         := BR_NO;
								when "01100" =>
									case ID_IR(10 downto 8) is
										when "000" =>
											-- BTEQZ 000 imm8
											New_EXE_ALU_Op    := ALUOP_ADD;
											New_EXE_ALU_Src_1 := ALUSRC_OTH;
											New_EXE_ALU_Src_2 := ALUSRC_OTH;
											if(ID_IR(7) = '0') then
												New_EXE_Imm := "00000000" & ID_IR(7 downto 0);
											else
												New_EXE_Imm := "11111111" & ID_IR(7 downto 0);
											end if;
											New_EXE_Jump_Type := JUMP_ALU;
											New_EXE_Mem_Read  := '0';
											New_EXE_Mem_Write := '0';
											New_EXE_Reg_Write := '0';
											New_EXE_Wb_Rid    := RID_NULL;
											T_ID_Rid_1        := RID_NULL;
											T_ID_Rid_2        := RID_T;
											T_RARead          := '0';
											T_RAWrite         := '0';
											T_Br_Type         := BR_EQ;
										when "001" =>
											-- BTNEZ 001 imm8
											New_EXE_ALU_Op    := ALUOP_ADD;
											New_EXE_ALU_Src_1 := ALUSRC_OTH;
											New_EXE_ALU_Src_2 := ALUSRC_OTH;
											if(ID_IR(7) = '0') then
												New_EXE_Imm := "00000000" & ID_IR(7 downto 0);
											else
												New_EXE_Imm := "11111111" & ID_IR(7 downto 0);
											end if;
											New_EXE_Jump_Type := JUMP_ALU;
											New_EXE_Mem_Read  := '0';
											New_EXE_Mem_Write := '0';
											New_EXE_Reg_Write := '0';
											New_EXE_Wb_Rid    := RID_NULL;
											T_ID_Rid_1        := RID_NULL;
											T_ID_Rid_2        := RID_T;
											T_RARead          := '0';
											T_RAWrite         := '0';
											T_Br_Type         := BR_NEQ;
										when "011" =>
											-- ADDSP 011 imm8
											New_EXE_ALU_Op    := ALUOP_ADD;
											New_EXE_ALU_Src_1 := ALUSRC_REG;
											New_EXE_ALU_Src_2 := ALUSRC_OTH;
											if(ID_IR(7) = '0') then
												New_EXE_Imm := "00000000" & ID_IR(7 downto 0);
											else
												New_EXE_Imm := "11111111" & ID_IR(7 downto 0);
											end if;
											New_EXE_Jump_Type := JUMP_NO;
											New_EXE_Mem_Read  := '0';
											New_EXE_Mem_Write := '0';
											New_EXE_Reg_Write := '1';
											New_EXE_Wb_Rid    := RID_SP;
											T_ID_Rid_1        := RID_SP;
											T_ID_Rid_2        := RID_NULL;
											T_RARead          := '0';
											T_RAWrite         := '0';
											T_Br_Type         := BR_NO;
										when "100" =>
											-- MTSP 100 rx 00000
											New_EXE_ALU_Op    := ALUOP_I1;
											New_EXE_ALU_Src_1 := ALUSRC_REG;
											New_EXE_ALU_Src_2 := ALUSRC_REG;
											New_EXE_Imm       := ZERO;
											New_EXE_Jump_Type := JUMP_NO;
											New_EXE_Mem_Read  := '0';
											New_EXE_Mem_Write := '0';
											New_EXE_Reg_Write := '1';
											New_EXE_Wb_Rid    := RID_SP;
											T_ID_Rid_1        := "1" & ID_IR(7 downto 5);
											T_ID_Rid_2        := RID_NULL;
											T_RARead          := '0';
											T_RAWrite         := '0';
											T_Br_Type         := BR_NO;
										when others =>
									end case;
								when "01101" =>
									-- LI rx imm8
									New_EXE_ALU_Op    := ALUOP_I2;
									New_EXE_ALU_Src_1 := ALUSRC_REG;
									New_EXE_ALU_Src_2 := ALUSRC_OTH;
									if(ID_IR(7) = '0') then
										New_EXE_Imm := "00000000" & ID_IR(7 downto 0);
									else
										New_EXE_Imm := "11111111" & ID_IR(7 downto 0);
									end if;
									New_EXE_Jump_Type := JUMP_NO;
									New_EXE_Mem_Read  := '0';
									New_EXE_Mem_Write := '0';
									New_EXE_Reg_Write := '1';
									New_EXE_Wb_Rid    := "1" & ID_IR(10 downto 8);
									T_ID_Rid_1        := RID_NULL;
									T_ID_Rid_2        := RID_NULL;
									T_RARead          := '0';
									T_RAWrite         := '0';
									T_Br_Type         := BR_NO;
								when "01111" =>
									-- MOVE rx ry 00000
									New_EXE_ALU_Op    := ALUOP_I1;
									New_EXE_ALU_Src_1 := ALUSRC_REG;
									New_EXE_ALU_Src_2 := ALUSRC_REG;
									New_EXE_Imm       := ZERO;
									New_EXE_Jump_Type := JUMP_NO;
									New_EXE_Mem_Read  := '0';
									New_EXE_Mem_Write := '0';
									New_EXE_Reg_Write := '1';
									New_EXE_Wb_Rid    := "1" & ID_IR(10 downto 8);
									T_ID_Rid_1        := "1" & ID_IR(7 downto 5);
									T_ID_Rid_2        := RID_NULL;
									T_RARead          := '0';
									T_RAWrite         := '0';
									T_Br_Type         := BR_NO;
								when "10010" =>
									-- LW_SP rx imm8
									New_EXE_ALU_Op    := ALUOP_ADD;
									New_EXE_ALU_Src_1 := ALUSRC_REG;
									New_EXE_ALU_Src_2 := ALUSRC_OTH;
									if(ID_IR(7) = '0') then
										New_EXE_Imm := "00000000" & ID_IR(7 downto 0);
									else
										New_EXE_Imm := "11111111" & ID_IR(7 downto 0);
									end if;
									New_EXE_Jump_Type := JUMP_NO;
									New_EXE_Mem_Read  := '1';
									New_EXE_Mem_Write := '0';
									New_EXE_Reg_Write := '1';
									New_EXE_Wb_Rid    := "1" & ID_IR(10 downto 8);
									T_ID_Rid_1        := RID_SP;
									T_ID_Rid_2        := RID_NULL;
									T_RARead          := '0';
									T_RAWrite         := '0';
									T_Br_Type         := BR_NO;
								when "10011" =>
									-- LW rx ry imm5
									New_EXE_ALU_Op    := ALUOP_ADD;
									New_EXE_ALU_Src_1 := ALUSRC_REG;
									New_EXE_ALU_Src_2 := ALUSRC_OTH;
									if(ID_IR(4) = '0') then
										New_EXE_Imm := "00000000000" & ID_IR(4 downto 0);
									else
										New_EXE_Imm := "11111111111" & ID_IR(4 downto 0);
									end if;
									New_EXE_Jump_Type := JUMP_NO;
									New_EXE_Mem_Read  := '1';
									New_EXE_Mem_Write := '0';
									New_EXE_Reg_Write := '1';
									New_EXE_Wb_Rid    := "1" & ID_IR(7 downto 5);
									T_ID_Rid_1        := "1" & ID_IR(10 downto 8);
									T_ID_Rid_2        := RID_NULL;
									T_RARead          := '0';
									T_RAWrite         := '0';
									T_Br_Type         := BR_NO;
								when "11010" =>
									-- SW_SP rx imm8
									New_EXE_ALU_Op    := ALUOP_ADD;
									New_EXE_ALU_Src_1 := ALUSRC_REG;
									New_EXE_ALU_Src_2 := ALUSRC_OTH;
									if(ID_IR(7) = '0') then
										New_EXE_Imm := "00000000" & ID_IR(7 downto 0);
									else
										New_EXE_Imm := "11111111" & ID_IR(7 downto 0);
									end if;
									New_EXE_Jump_Type := JUMP_NO;
									New_EXE_Mem_Read  := '0';
									New_EXE_Mem_Write := '1';
									New_EXE_Reg_Write := '0';
									New_EXE_Wb_Rid    := RID_NULL;
									T_ID_Rid_1        := RID_SP;
									T_ID_Rid_2        := "1" & ID_IR(10 downto 8);
									T_RARead          := '0';
									T_RAWrite         := '0';
									T_Br_Type         := BR_NO;
								when "11011" =>
									-- SW rx ry imm5
									New_EXE_ALU_Op    := ALUOP_ADD;
									New_EXE_ALU_Src_1 := ALUSRC_REG;
									New_EXE_ALU_Src_2 := ALUSRC_OTH;
									if(ID_IR(4) = '0') then
										New_EXE_Imm := "00000000000" & ID_IR(4 downto 0);
									else
										New_EXE_Imm := "11111111111" & ID_IR(4 downto 0);
									end if;
									New_EXE_Jump_Type := JUMP_NO;
									New_EXE_Mem_Read  := '0';
									New_EXE_Mem_Write := '1';
									New_EXE_Reg_Write := '0';
									New_EXE_Wb_Rid    := RID_NULL;
									T_ID_Rid_1        := "1" & ID_IR(10 downto 8);
									T_ID_Rid_2        := "1" & ID_IR(7 downto 5);
									T_RARead          := '0';
									T_RAWrite         := '0';
									T_Br_Type         := BR_NO;
								when "11100" =>
									case ID_IR(1 downto 0) is
										when "01" =>
											-- ADDU rx ry rz 01
											New_EXE_ALU_Op    := ALUOP_ADD;
											New_EXE_ALU_Src_1 := ALUSRC_REG;
											New_EXE_ALU_Src_2 := ALUSRC_REG;
											New_EXE_Imm       := ZERO;
											New_EXE_Jump_Type := JUMP_NO;
											New_EXE_Mem_Read  := '0';
											New_EXE_Mem_Write := '0';
											New_EXE_Reg_Write := '1';
											New_EXE_Wb_Rid    := "1" & ID_IR(4 downto 2);
											T_ID_Rid_1        := "1" & ID_IR(10 downto 8);
											T_ID_Rid_2        := "1" & ID_IR(7 downto 5);
											T_RARead          := '0';
											T_RAWrite         := '0';
											T_Br_Type         := BR_NO;
										when "11" =>
											-- SUBU rx ry rz 11
											New_EXE_ALU_Op    := ALUOP_SUB;
											New_EXE_ALU_Src_1 := ALUSRC_REG;
											New_EXE_ALU_Src_2 := ALUSRC_REG;
											New_EXE_Imm       := ZERO;
											New_EXE_Jump_Type := JUMP_NO;
											New_EXE_Mem_Read  := '0';
											New_EXE_Mem_Write := '0';
											New_EXE_Reg_Write := '1';
											New_EXE_Wb_Rid    := "1" & ID_IR(4 downto 2);
											T_ID_Rid_1        := "1" & ID_IR(10 downto 8);
											T_ID_Rid_2        := "1" & ID_IR(7 downto 5);
											T_RARead          := '0';
											T_RAWrite         := '0';
											T_Br_Type         := BR_NO;
										when others =>
									end case;
								when "11101" =>
									case ID_IR(4 downto 0) is
										when "00000" =>
											case ID_IR(7 downto 5) is
												when "000" =>
													-- JR rx 000 00000
													-- DEBUG 20151123
													New_EXE_ALU_Op    := ALUOP_I1;
													-- New_EXE_ALU_Op    := ALUOP_I2;
													New_EXE_ALU_Src_1 := ALUSRC_REG;
													New_EXE_ALU_Src_2 := ALUSRC_REG;
													New_EXE_Imm       := ZERO;
													New_EXE_Jump_Type := JUMP_RY;
													-- New_EXE_Jump_Type := JUMP_ALU;
													New_EXE_Mem_Read  := '0';
													New_EXE_Mem_Write := '0';
													New_EXE_Reg_Write := '0';
													New_EXE_Wb_Rid    := RID_NULL;
													T_ID_Rid_1        := RID_NULL;
													T_ID_Rid_2        := "1" & ID_IR(10 downto 8);
													T_RARead          := '0';
													T_RAWrite         := '0';
													T_Br_Type         := BR_NO;
												when "001" =>
													-- JRRA 000 001 00000
													New_EXE_ALU_Op    := ALUOP_I1;
													New_EXE_ALU_Src_1 := ALUSRC_REG;
													New_EXE_ALU_Src_2 := ALUSRC_REG;
													New_EXE_Imm       := ZERO;
													New_EXE_Jump_Type := JUMP_RY;
													New_EXE_Mem_Read  := '0';
													New_EXE_Mem_Write := '0';
													New_EXE_Reg_Write := '0';
													New_EXE_Wb_Rid    := RID_NULL;
													T_ID_Rid_1        := RID_NULL;
													T_ID_Rid_2        := RID_RA;
													T_RARead          := '0';
													T_RAWrite         := '0';
													T_Br_Type         := BR_NO;
												when "010" =>
													-- MFPC rx 010 00000
													New_EXE_ALU_Op    := ALUOP_I1;
													New_EXE_ALU_Src_1 := ALUSRC_OTH;
													New_EXE_ALU_Src_2 := ALUSRC_REG;
													New_EXE_Imm       := ZERO;
													New_EXE_Jump_Type := JUMP_NO;
													New_EXE_Mem_Read  := '0';
													New_EXE_Mem_Write := '0';
													New_EXE_Reg_Write := '1';
													New_EXE_Wb_Rid    := "1" & ID_IR(10 downto 8);
													T_ID_Rid_1        := RID_IH;
													T_ID_Rid_2        := RID_NULL;
													T_RARead          := '0';
													T_RAWrite         := '0';
													T_Br_Type         := BR_NO;
												when "110" =>
													-- JALR rx 110 00000
													New_EXE_ALU_Op    := ALUOP_I1;
													New_EXE_ALU_Src_1 := ALUSRC_OTH;
													New_EXE_ALU_Src_2 := ALUSRC_REG;
													New_EXE_Imm       := ZERO;
													New_EXE_Jump_Type := JUMP_RY;
													New_EXE_Mem_Read  := '0';
													New_EXE_Mem_Write := '0';
													New_EXE_Reg_Write := '1';
													New_EXE_Wb_Rid    := RID_RA;
													T_ID_Rid_1        := RID_NULL;
													T_ID_Rid_2        := "1" & ID_IR(10 downto 8);
													T_RARead          := '0';
													T_RAWrite         := '0';
													T_Br_Type         := BR_NO;
												when others =>
											end case;
										when "01010" =>
											-- CMP rx ry 01010
											New_EXE_ALU_Op    := ALUOP_NEQ;
											New_EXE_ALU_Src_1 := ALUSRC_REG;
											New_EXE_ALU_Src_2 := ALUSRC_REG;
											New_EXE_Imm       := ZERO;
											New_EXE_Jump_Type := JUMP_NO;
											New_EXE_Mem_Read  := '0';
											New_EXE_Mem_Write := '0';
											New_EXE_Reg_Write := '1';
											New_EXE_Wb_Rid    := RID_T;
											T_ID_Rid_1        := "1" & ID_IR(10 downto 8);
											T_ID_Rid_2        := "1" & ID_IR(7 downto 5);
											T_RARead          := '0';
											T_RAWrite         := '0';
											T_Br_Type         := BR_NO;
										when "01100" =>
											-- AND rx ry 01100
											New_EXE_ALU_Op    := ALUOP_AND;
											New_EXE_ALU_Src_1 := ALUSRC_REG;
											New_EXE_ALU_Src_2 := ALUSRC_REG;
											New_EXE_Imm       := ZERO;
											New_EXE_Jump_Type := JUMP_NO;
											New_EXE_Mem_Read  := '0';
											New_EXE_Mem_Write := '0';
											New_EXE_Reg_Write := '1';
											New_EXE_Wb_Rid    := "1" & ID_IR(10 downto 8);
											T_ID_Rid_1        := "1" & ID_IR(10 downto 8);
											T_ID_Rid_2        := "1" & ID_IR(7 downto 5);
											T_RARead          := '0';
											T_RAWrite         := '0';
											T_Br_Type         := BR_NO;
										when "01101" =>
											-- OR rx ry 01101
											New_EXE_ALU_Op    := ALUOP_OR;
											New_EXE_ALU_Src_1 := ALUSRC_REG;
											New_EXE_ALU_Src_2 := ALUSRC_REG;
											New_EXE_Imm       := ZERO;
											New_EXE_Jump_Type := JUMP_NO;
											New_EXE_Mem_Read  := '0';
											New_EXE_Mem_Write := '0';
											New_EXE_Reg_Write := '1';
											New_EXE_Wb_Rid    := "1" & ID_IR(10 downto 8);
											T_ID_Rid_1        := "1" & ID_IR(10 downto 8);
											T_ID_Rid_2        := "1" & ID_IR(7 downto 5);
											T_RARead          := '0';
											T_RAWrite         := '0';
											T_Br_Type         := BR_NO;
										when others =>
									end case;
								when "11110" =>
									case ID_IR(4 downto 0) is
										when "00000" =>
											-- MFIH rx 000 00000
											New_EXE_ALU_Op    := ALUOP_I1;
											New_EXE_ALU_Src_1 := ALUSRC_REG;
											New_EXE_ALU_Src_2 := ALUSRC_REG;
											New_EXE_Imm       := ZERO;
											New_EXE_Jump_Type := JUMP_NO;
											New_EXE_Mem_Read  := '0';
											New_EXE_Mem_Write := '0';
											New_EXE_Reg_Write := '1';
											New_EXE_Wb_Rid    := "1" & ID_IR(10 downto 8);
											T_ID_Rid_1        := RID_IH;
											T_ID_Rid_2        := RID_NULL;
											T_RARead          := '0';
											T_RAWrite         := '0';
											T_Br_Type         := BR_NO;
										when "00001" =>
											-- MTIH rx 000 00001
											New_EXE_ALU_Op    := ALUOP_I1;
											New_EXE_ALU_Src_1 := ALUSRC_REG;
											New_EXE_ALU_Src_2 := ALUSRC_REG;
											New_EXE_Imm       := ZERO;
											New_EXE_Jump_Type := JUMP_NO;
											New_EXE_Mem_Read  := '0';
											New_EXE_Mem_Write := '0';
											New_EXE_Reg_Write := '1';
											New_EXE_Wb_Rid    := RID_IH;
											T_ID_Rid_1        := "1" & ID_IR(10 downto 8);
											T_ID_Rid_2        := RID_NULL;
											T_RARead          := '0';
											T_RAWrite         := '0';
											T_Br_Type         := BR_NO;
										when others =>
									end case;
								when others =>
									New_EXE_ALU_Op    := ALUOP_I1;
									New_EXE_ALU_Src_1 := ALUSRC_REG;
									New_EXE_ALU_Src_2 := ALUSRC_REG;
									New_EXE_Imm       := ZERO;
									New_EXE_Jump_Type := JUMP_NO;
									New_EXE_Mem_Read  := '0';
									New_EXE_Mem_Write := '0';
									New_EXE_Reg_Write := '0';
									New_EXE_Wb_Rid    := RID_NULL;
									T_ID_Rid_1        := RID_NULL;
									T_ID_Rid_2        := RID_NULL;
									T_RARead          := '0';
									T_RAWrite         := '0';
									T_Br_Type         := BR_NO;
							end case;
							-- RA的访问与修改，以及得出New_EXE_New_PC
							-- Reg得出New_EXE_Reg_Data
							-- 根据New_EXE_Reg_Data_2（也就是New_EXE_Sw_Data）
							--   和T_Br_Type得出真正的New_EXE_Jump_Type
							-- if(T_RAWrite = '1') then
								-- RA := ID_New_PC;
							-- end if;
							-- if(T_RARead = '1') then
								-- New_EXE_New_PC := RA;
							-- else
							New_EXE_New_PC := ID_New_PC;
							-- end if;
							-- 【done】RA的访问与修改，以及得出New_EXE_New_PC
							-- Reg得出New_EXE_Reg_Data
							-- 根据New_EXE_Reg_Data_2（也就是New_EXE_Sw_Data）
							--   和T_Br_Type得出真正的New_EXE_Jump_Type
							New_IF_Pause := '0';
							if(T_ID_Rid_1 = RID_NULL) then
								New_EXE_Reg_Data_1 := ZERO;
							else
								if(T_ID_Rid_1 = Fwd_Rid_1) then
									if(Fwd_Data_1_av = '1') then
										New_EXE_Reg_Data_1 := Fwd_Data_1;
									else
										New_IF_Pause := '1';
									end if;
								else
									if(T_ID_Rid_1 = Fwd_Rid_2) then
										New_EXE_Reg_Data_1 := Fwd_Data_2;
									else
										case T_ID_Rid_1 is
											when RID_NULL => New_EXE_Reg_Data_1 := ZERO;
											when RID_RA   => New_EXE_Reg_Data_1 := RA;
											when RID_SP   => New_EXE_Reg_Data_1 := SP;
											when RID_IH   => New_EXE_Reg_Data_1 := IH;
											when RID_T    => New_EXE_Reg_Data_1 := T ;
											when RID_R0   => New_EXE_Reg_Data_1 := R0;
											when RID_R1   => New_EXE_Reg_Data_1 := R1;
											when RID_R2   => New_EXE_Reg_Data_1 := R2;
											when RID_R3   => New_EXE_Reg_Data_1 := R3;
											when RID_R4   => New_EXE_Reg_Data_1 := R4;
											when RID_R5   => New_EXE_Reg_Data_1 := R5;
											when RID_R6   => New_EXE_Reg_Data_1 := R6;
											when RID_R7   => New_EXE_Reg_Data_1 := R7;
											when others   => New_EXE_Reg_Data_1 := ZERO;
										end case;
									end if;
								end if;
							end if;
							if(T_ID_Rid_2 = RID_NULL) then
								New_EXE_Reg_Data_2 := ZERO;
							else
								if(T_ID_Rid_2 = Fwd_Rid_1) then
									if(Fwd_Data_1_av = '1') then
										New_EXE_Reg_Data_2 := Fwd_Data_1;
									else
										New_IF_Pause := '1';
									end if;
								else
									if(T_ID_Rid_2 = Fwd_Rid_2) then
										New_EXE_Reg_Data_2 := Fwd_Data_2;
									else
										case T_ID_Rid_2 is
											when RID_NULL => New_EXE_Reg_Data_2 := ZERO;
											when RID_RA   => New_EXE_Reg_Data_2 := RA;
											when RID_SP   => New_EXE_Reg_Data_2 := SP;
											when RID_IH   => New_EXE_Reg_Data_2 := IH;
											when RID_T    => New_EXE_Reg_Data_2 := T ;
											when RID_R0   => New_EXE_Reg_Data_2 := R0;
											when RID_R1   => New_EXE_Reg_Data_2 := R1;
											when RID_R2   => New_EXE_Reg_Data_2 := R2;
											when RID_R3   => New_EXE_Reg_Data_2 := R3;
											when RID_R4   => New_EXE_Reg_Data_2 := R4;
											when RID_R5   => New_EXE_Reg_Data_2 := R5;
											when RID_R6   => New_EXE_Reg_Data_2 := R6;
											when RID_R7   => New_EXE_Reg_Data_2 := R7;
											when others   => New_EXE_Reg_Data_2 := ZERO;
										end case;
									end if;
								end if;
							end if;
							-- 【done】RA的访问与修改，以及得出New_EXE_New_PC
							-- 【done】Reg得出New_EXE_Reg_Data
							-- 根据New_EXE_Reg_Data_2（也就是New_EXE_Sw_Data）
							--   和T_Br_Type得出真正的New_EXE_Jump_Type
							New_EXE_Sw_Data := New_EXE_Reg_Data_2;
							-- if(T_Br_Type = BR_EQ) then
								-- if(New_EXE_Sw_Data /= "0000000000000000") then
									-- New_EXE_Jump_Type := JUMP_NO;
								-- end if;
							-- end if;
							-- if(T_Br_Type = BR_NEQ) then
								-- if(New_EXE_Sw_Data = "0000000000000000") then
									-- New_EXE_Jump_Type := JUMP_NO;
								-- end if;
							-- end if;
							-- 【done】RA的访问与修改，以及得出New_EXE_New_PC
							-- 【done】Reg得出New_EXE_Reg_Data
							-- 【done】根据New_EXE_Reg_Data_2（也就是New_EXE_Sw_Data）
							--   和T_Br_Type得出真正的New_EXE_Jump_Type
							-- 最后一件事……发送Pause信号
							if(New_IF_Pause = '1') then
								-- 毁掉当前成果
								New_EXE_New_PC := ZERO;
								New_EXE_ALU_Op := ALUOP_I1;
								New_EXE_ALU_Src_1 := ALUSRC_REG;
								New_EXE_ALU_Src_2 := ALUSRC_REG;
								New_EXE_Reg_Data_1 := ZERO;
								New_EXE_Reg_Data_2 := ZERO;
								New_EXE_Imm := ZERO;
								New_EXE_Jump_Type := JUMP_NO;
								New_EXE_Sw_Data := ZERO;
								New_EXE_Mem_Read := '0';
								New_EXE_Mem_Write := '0';
								New_EXE_Reg_Write := '0';
								New_EXE_Wb_Rid := RID_NULL;
								-- 维持当前输入
								New_ID_New_PC   := ID_New_PC;
								New_ID_IR       := ID_IR;
							end if;
						end if;
						
						-- End component ID
						
						-- 总执行时序
							-- 【done】IF和MEM只进行读写MI
							-- 【done】MI工作
							-- 【done】确定New_WB_Lw_Data
							-- 【done】EXE工作
							-- 【done】WB工作
							-- 【done】Fwd2工作
							-- 【done】Fwd工作
							-- 【done】ID工作
							-- JH工作
							-- IF和MEM把剩余工作做完
						-- OK
						
						-- Begin component JH
						-- 输入端口
							-- EXE_New_PC : addr_type【WAIT】
							-- EXE_Imm : data_type【WAIT】
							-- EXE_Jump_Type : slv(1)【WAIT】
							-- EXE_Reg_Data_2 : data_type【WAIT】
							-- MEM_ALU_Res : data_type【WAIT】
							-- EXE_Jump_Type : slv(1)
						-- 输出端口
							-- IF_Jump_Addr : addr_type
							-- IF_Jump_Type : bool
							-- ID_IR_Clear : bool
						
						New_IF_Jump_Type := '0';
						New_ID_IR_Clear := '0';
						-- 处理三段跳
						if(New_EXE_Jump_Type = JUMP_ALU) then
							New_IF_Jump_Addr := ID_Jump_Tar;
							New_IF_Jump_Type := '1';
							New_ID_IR_Clear := '0';
							New_Br_OPC := ID_New_PC + '1';
							Test_L := R0(7 downto 0) & New_Br_OPC(7 downto 0);
						end if;
						if(EXE_Jump_Type = JUMP_ALU) then
							if(L_Br_Type = BR_EQ) then
								if(EXE_Sw_Data /= "0000000000000000") then
									New_IF_Jump_Addr := Br_OPC;
									New_IF_Jump_Type := '1';
									New_ID_IR_Clear := '1';
								end if;
							end if;
							if(L_Br_Type = BR_NEQ) then
								if(EXE_Sw_Data = "0000000000000000") then
									New_IF_Jump_Addr := Br_OPC;
									New_IF_Jump_Type := '1';
									New_ID_IR_Clear := '1';
								end if;
							end if;
						end if;
						-- 处理二段跳
						-- if(New_EXE_Jump_Type = JUMP_RY) then
							-- New_IF_Jump_Addr := "00" & New_EXE_Reg_Data_2;
							-- New_IF_Jump_Type := '1';
							-- New_ID_IR_Clear := '0';
						-- end if;
						-- if(New_EXE_Jump_Type = JUMP_RA) then
							-- New_IF_Jump_Addr := New_EXE_New_PC;
							-- New_IF_Jump_Type := '1';
							-- New_ID_IR_Clear := '0';
						-- end if;
						if(EXE_Jump_Type = JUMP_RY) then
							New_IF_Jump_Addr := "00" & EXE_Reg_Data_2;
							New_IF_Jump_Type := '1';
							New_ID_IR_Clear := '1';
						end if;
						if(EXE_Jump_Type = JUMP_RA) then
							New_IF_Jump_Addr := EXE_New_PC;
							New_IF_Jump_Type := '1';
							New_ID_IR_Clear := '1';
						end if;
						
						-- End component JH
						
						-- 总执行时序
							-- 【done】IF和MEM只进行读写MI
							-- 【done】MI工作
							-- 【done】确定New_WB_Lw_Data
							-- 【done】EXE工作
							-- 【done】WB工作
							-- 【done】Fwd2工作
							-- 【done】Fwd工作
							-- 【done】ID工作
							-- 【done】JH工作
							-- IF和MEM把剩余工作做完
						-- OK
						
						-- Begin component IF
						
						-- IF如果被Pause了就不要动New_ID_New_PC和New_ID_IR了！
						-- 输入端口
							-- IF_Jump_Addr : addr_type【WAIT】
							-- IF_Jump_Type : bool【WAIT】
							-- IF_Pause : bool【WAIT】
							-- MI_Read_Data_2 : data_type【WAIT】
						-- 输出端口
							-- ID_New_PC : addr_type
							-- ID_IR : data_type
							-- MI_Read_Addr_2 : addr_type
						-- 内部状态变量
							-- PC : addr_type
						if(New_IF_Pause = '1') then
							-- 被暂停，什么也不做
							New_PC := PC;
						else
							New_ID_New_PC := PC + '1';
							New_ID_IR := MI_Read_Data_2;
							case New_ID_IR(15 downto 11) is
								when "00010" =>
									-- B imm11
									if(New_ID_IR(10) = '0') then
										New_ID_Jump_Tar := "00" & (New_ID_New_PC(15 downto 0) + ("00000" & New_ID_IR(10 downto 0)));
									else
										New_ID_Jump_Tar := "00" & (New_ID_New_PC(15 downto 0) + ("11111" & New_ID_IR(10 downto 0)));
									end if;
								when "01100" =>
									-- BTEQZ/BTNEZ xxx imm8
									if(New_ID_IR(7) = '0') then
										New_ID_Jump_Tar := "00" & (New_ID_New_PC(15 downto 0) + ("00000000" & New_ID_IR(7 downto 0)));
									else
										New_ID_Jump_Tar := "00" & (New_ID_New_PC(15 downto 0) + ("11111111" & New_ID_IR(7 downto 0)));
									end if;
								when "00100" =>
									-- BEQZ xxx imm8
									if(New_ID_IR(7) = '0') then
										New_ID_Jump_Tar := "00" & (New_ID_New_PC(15 downto 0) + ("00000000" & New_ID_IR(7 downto 0)));
									else
										New_ID_Jump_Tar := "00" & (New_ID_New_PC(15 downto 0) + ("11111111" & New_ID_IR(7 downto 0)));
									end if;
								when "00101" =>
									-- BEQZ xxx imm8
									if(New_ID_IR(7) = '0') then
										New_ID_Jump_Tar := "00" & (New_ID_New_PC(15 downto 0) + ("00000000" & New_ID_IR(7 downto 0)));
									else
										New_ID_Jump_Tar := "00" & (New_ID_New_PC(15 downto 0) + ("11111111" & New_ID_IR(7 downto 0)));
									end if;
								when others =>
									New_ID_Jump_Tar := ZERO;
							end case;
							if(New_IF_Jump_Type = '1') then
								New_PC := New_IF_Jump_Addr;
							else
								New_PC := New_ID_New_PC;
							end if;
						end if;
						
						-- End component IF
						
						-- Begin component MEM
						-- 输入端口
							-- MEM_ALU_Res : data_type
							-- MEM_Sw_Data : data_type
							-- MEM_Mem_Read : bool
							-- MEM_Mem_Write : bool
							-- MEM_Reg_Write : bool
							-- MEM_Wb_Rid : reg_type
							-- MI_Read_Data_1 : data_type【WAIT】
						-- 输出端口
							-- MI_Read_Addr_1 : addr_type
							-- MI_Write_Addr : addr_type
							-- MI_Write_Data : data_type
							-- WB_Lw_Data : data_type
							-- WB_ALU_Res : data_type
							-- WB_Mem_Read : bool
							-- WB_Reg_Write : bool
							-- WB_Wb_Rid : reg_type
						New_WB_Lw_Data := MI_Read_Data_1;
						New_WB_ALU_Res := MEM_ALU_Res;
						New_WB_Mem_Read := MEM_Mem_Read;
						New_WB_Reg_Write := MEM_Reg_Write;
						New_WB_Wb_Rid := MEM_Wb_Rid;
						-- End component MEM
						
						-- 总执行时序
							-- 【done】IF和MEM只进行读写MI
							-- 【done】MI工作
							-- 【done】确定New_WB_Lw_Data
							-- 【done】EXE工作
							-- 【done】WB工作
							-- 【done】Fwd2工作
							-- 【done】Fwd工作
							-- 【done】ID工作
							-- 【done】JH工作
							-- 【done】IF和MEM把剩余工作做完
						-- OK
						
						-- 所有的事情都做完啦！
						-- 最后一件事情：
						-- 把所有的New都Apply掉
						
						-- Apply的顺序就无所谓了，每个component来吧
						
						L_Br_Type := T_Br_Type;
						Br_OPC := New_Br_OPC;
						uartcnt := New_uartcnt;
						
						-- IF
						ID_New_PC := New_ID_New_PC;
						ID_IR     := New_ID_IR;
						ID_Jump_Tar := New_ID_Jump_Tar;
						PC        := New_PC;
						PC(17 downto 16) := "00";
						
						-- DEBUG
						-- if(PC = BL_Addr_Tot) then
							-- PC := PC - '1';
						-- end if;
						
						-- ID
						IF_Pause       := New_IF_Pause;
						EXE_New_PC     := New_EXE_New_PC;
						EXE_ALU_Op     := New_EXE_ALU_Op;
						EXE_ALU_Src_1  := New_EXE_ALU_Src_1;
						EXE_ALU_Src_2  := New_EXE_ALU_Src_2;
						EXE_Reg_Data_1 := New_EXE_Reg_Data_1;
						EXE_Reg_Data_2 := New_EXE_Reg_Data_2;
						EXE_Imm        := New_EXE_Imm;
						EXE_Jump_Type  := New_EXE_Jump_Type;
						EXE_Sw_Data    := New_EXE_Sw_Data;
						EXE_Mem_Read   := New_EXE_Mem_Read;
						EXE_Mem_Write  := New_EXE_Mem_Write;
						EXE_Reg_Write  := New_EXE_Reg_Write;
						EXE_Wb_Rid     := New_EXE_Wb_Rid;
						
						-- EXE
						MEM_ALU_Res   := New_MEM_ALU_Res;
						MEM_Sw_Data   := New_MEM_Sw_Data;
						MEM_Mem_Read  := New_MEM_Mem_Read;
						MEM_Mem_Write := New_MEM_Mem_Write;
						MEM_Reg_Write := New_MEM_Reg_Write;
						MEM_Wb_Rid    := New_MEM_Wb_Rid;
						
						-- MEM
						WB_Lw_Data   := New_WB_Lw_Data;
						WB_ALU_Res   := New_WB_ALU_Res;
						WB_Mem_Read  := New_WB_Mem_Read;
						WB_Reg_Write := New_WB_Reg_Write;
						WB_Wb_Rid    := New_WB_Wb_Rid;
						
						-- WB
						ID_Wb_Rid  := New_ID_Wb_Rid;
						ID_Wb_Data := New_ID_Wb_Data;
						
						-- JH
						IF_Jump_Addr := New_IF_Jump_Addr;
						IF_Jump_Type := New_IF_Jump_Type;
						ID_IR_Clear  := New_ID_IR_Clear;
						
						-- 注意Fwd模块不参与时序（即整体临时）所以不用Apply
					-- if(BL_State = '1' and CLK_clk_ed = '1') then
						-- 现在CPU可以开始干活了
						-- 但是只能做第一阶段的事情
						-- 也就是IF和MEM调用MI
						---- Begin component IF_Pre
						New_MI_Read_Addr_2 := PC;
						---- End component IF_Pre
						---- Begin component MEM_Pre
						if(MEM_Mem_Read = '1') then
							New_MI_Read_Addr_1 := "00" & MEM_ALU_Res;
						else
							New_MI_Read_Addr_1 := ONE;
						end if;
						if(MEM_Mem_Write = '1') then
							New_MI_Write_Addr := "00" & MEM_ALU_Res;
							New_MI_Write_Data := MEM_Sw_Data;
						else
							New_MI_Write_Addr := ONE;
							New_MI_Write_Data := HIGH_Z;
						end if;
						---- End component MEM_Pre
					-- end if;
					end if;
					if(BL_State = '1' and click = '0') then
						CLK_clk_ed := '1';
					end if;
				end if;
				
				if(MI_State = "000") then
					-- 现在才能进行正常的事情
					-- 也就是CPU开始干活了
					-- 然而CPU并不能开始干活
					-- 因为Boot Loader要干活了
					if(Boot_State = '1') then
						if(BL_State = '0') then
							case BL_Recv_State is
								when "000" =>
									New_MI_Read_Addr_1 := BL_Addr_Cur;
									New_BL_Recv_State := "001";
								when "001" =>
									Test_L := Test_L xor MI_Read_Data_1;
									New_BL_Addr_Cur := BL_Addr_Cur + '1';
									if(New_BL_Addr_Cur = BL_Addr_Tot) then
										New_BL_State := '1';
									end if;
									New_BL_Recv_State := "000";
								when others =>
							end case;
							Boot_State    := New_Boot_State   ;
							BL_State      := New_BL_State     ;
							BL_Recv_State := New_BL_Recv_State;
							BL_Addr_Tot   := New_BL_Addr_Tot  ;
							BL_Addr_Cur   := New_BL_Addr_Cur  ;
							BL_Data       := New_BL_Data      ;
						end if;
					end if;
					if(Boot_State = '0') then
						---- Begin component BL
						-- 无论如何，先收一个字
						case BL_Recv_State is
							when "000" =>
								if(uartcnt = '1') then
									New_uartcnt := '0';
								else
									if(data_ready = '1') then
										New_uartcnt := '1';
										New_MI_Read_Addr_1 := BF00;
										New_BL_Recv_State := "001";
									end if;
								end if;
							when "001" =>
								New_BL_Data(15 downto 14) := MI_Read_Data_1(1 downto 0);
								New_BL_Recv_State := "010";
							when "010" =>
								if(uartcnt = '1') then
									New_uartcnt := '0';
								else
									if(data_ready = '1') then
										New_uartcnt := '1';
										New_MI_Read_Addr_1 := BF00;
										New_BL_Recv_State := "011";
									end if;
								end if;
							when "011" =>
								New_BL_Data(13 downto 7) := MI_Read_Data_1(6 downto 0);
								New_BL_Recv_State := "100";
							when "100" =>
								if(uartcnt = '1') then
									New_uartcnt := '0';
								else
									if(data_ready = '1') then
										New_uartcnt := '1';
										New_MI_Read_Addr_1 := BF00;
										New_BL_Recv_State := "101";
									end if;
								end if;
							when "101" =>
								New_BL_Data(6 downto 0) := MI_Read_Data_1(6 downto 0);
								New_BL_Recv_State := "110";
								if(BL_State = '0') then
									-- 正在接收总字数
									New_BL_Addr_Tot := "00" & New_BL_Data;
									New_BL_Addr_Cur := ZERO;
									New_BL_State := '1';
									Test_L := ZERO;
								else
									-- 正在接收内存字
									New_MI_Write_Addr := BL_Addr_Cur;
									New_MI_Write_Data := New_BL_Data;
									New_BL_Addr_Cur := BL_Addr_Cur + '1';
									-- Test_L := Test_L xor New_BL_Data;
								end if;
							when "110" =>
								New_BL_Recv_State := "000";
								if(BL_Addr_Cur = BL_Addr_Tot) then
									New_Boot_State := '1';
									New_BL_State := '0';
									New_BL_Addr_Cur := ZERO;
								end if;
							when others =>
						end case;
						-- if(BL_State = '0') then
							-- New_BL_State := '1';
						-- else
							-- New_Boot_State := '1';
							-- New_BL_State := '0';
						-- end if;
						-- New_BL_Recv_State := BL_Recv_State + '1';
						-- if(New_BL_Recv_State = "11") then
							-- New_BL_Recv_State := "00";
						-- end if;
						Boot_State    := New_Boot_State   ;
						BL_State      := New_BL_State     ;
						BL_Recv_State := New_BL_Recv_State;
						BL_Addr_Tot   := New_BL_Addr_Tot  ;
						BL_Addr_Cur   := New_BL_Addr_Cur  ;
						BL_Data       := New_BL_Data      ;
						---- End component BL
					end if;
				end if;
				---- Begin component MI
				-- Apply NEW Before MI 20151113 wys
				MI_Write_Addr  := New_MI_Write_Addr ;
				MI_Write_Data  := New_MI_Write_Data ;
				MI_Read_Addr_1 := New_MI_Read_Addr_1;
				MI_Read_Data_1 := New_MI_Read_Data_1;
				MI_Read_Addr_2 := New_MI_Read_Addr_2;
				MI_Read_Data_2 := New_MI_Read_Data_2;
			
				MI_State := New_MI_State;
				-- OE = 0 after write!!!
				-- OE = 0 after write!!!
				-- OE = 0 after write!!!
				if(MI_Read_Addr_1 /= ONE_18b) then
					if(MI_Read_Addr_1 = BF00) then
						-- Read UART and Read 2
						case MI_State is
							when "000" =>
								New_rdn := '1';
								New_Ram1Data := HIGH_Z;
							when "001" =>
								New_Ram2Data := HIGH_Z;
								New_Ram2Addr := MI_Read_Addr_2;
								New_rdn := '0';
								UART_read := '1';
							when others =>
						end case;
						New_MI_State := MI_State + '1';
						if(New_MI_State = "010") then
							New_MI_State := "000";
						end if;
					else
						if(MI_Read_Addr_1 = BF01) then
							-- Read SB and Read 2
							case MI_State is
								when "000" =>
									New_Ram2Data := HIGH_Z;
									New_Ram2Addr := MI_Read_Addr_2;
									if(uartcnt = '1') then
										New_uartcnt := '0';
										New_MI_Read_Data_1(1) := '0';
										New_MI_Read_Data_1(0) := '0';
									else
										New_uartcnt := '1';
										New_MI_Read_Data_1(1) := data_ready;
										New_MI_Read_Data_1(0) := tsre and tbre;
									end if;
								when others =>
							end case;
							New_MI_State := MI_State + '1';
							if(New_MI_State = "001") then
								New_MI_State := "000";
							end if;
						else
							-- Read 1 and Read 2
							case MI_State is
								when "000" =>
									New_Ram2Data := HIGH_Z;
									New_Ram2Addr := MI_Read_Addr_1;
								when "001" =>
									New_MI_Read_Data_1 := Ram2Data;
									New_Ram2Data := HIGH_Z;
									New_Ram2Addr := MI_Read_Addr_2;
								when others =>
							end case;
							New_MI_State := MI_State + '1';
							if(New_MI_State = "010") then
								New_MI_State := "000";
							end if;
						end if;
					end if;
				else
					if(MI_Write_Addr /= ONE_18b) then
						if(MI_Write_Addr = BF00) then
							-- Write UART and Read 2
							case MI_State is
								when "000" =>
									New_Ram1Data := MI_Write_Data;
								when "001" =>
									New_wrn := '0';
								when "010" =>
									New_wrn := '1';
									New_Ram2Addr := MI_Read_Addr_2;
									New_Ram2Data := HIGH_Z;
								when others =>
							end case;
							New_MI_State := MI_State + '1';
							if(New_MI_State = "011") then
								New_MI_State := "000";
							end if;
						else
							-- Write 1 and Read 2
							case MI_State is
								when "000" =>
									New_Ram2OE := '1';
									New_Ram2Addr := MI_Write_Addr;
									New_Ram2Data := MI_Write_Data;
								when "001" =>
									New_Ram2WE := '0';
								when "010" =>
									New_Ram2WE := '1';
									New_Ram2OE := '0';
									New_Ram2Addr := MI_Read_Addr_2;
									New_Ram2Data := HIGH_Z;
								when others =>
							end case;
							New_MI_State := MI_State + '1';
							if(New_MI_State = "011") then
								New_MI_State := "000";
							end if;
						end if;
					else
						-- Read 2 only
						case MI_State is
							when "000" =>
								New_Ram2Data := HIGH_Z;
								New_Ram2Addr := MI_Read_Addr_2;
							when others =>
						end case;
						New_MI_State := MI_State + '1';
						if(New_MI_State = "001") then
							New_MI_State := "000";
						end if;
					end if;
				end if;
				
				MI_Write_Addr  := New_MI_Write_Addr ;
				MI_Write_Data  := New_MI_Write_Data ;
				MI_Read_Addr_1 := New_MI_Read_Addr_1;
				MI_Read_Data_1 := New_MI_Read_Data_1;
				MI_Read_Addr_2 := New_MI_Read_Addr_2;
				MI_Read_Data_2 := New_MI_Read_Data_2;
			
				MI_State := New_MI_State;
				---- End component MI
			-- end if;
		end if;
	end if;
	
	Ram1Addr <= New_Ram1Addr;
	Ram1Data <= New_Ram1Data;
	Ram2Addr <= New_Ram2Addr;
	Ram2Data <= New_Ram2Data;
	Ram1WE <= New_Ram1WE;
	Ram1OE <= New_Ram1OE;
	Ram1EN <= New_Ram1EN;
	Ram2WE <= New_Ram2WE;
	Ram2OE <= New_Ram2OE;
	Ram2EN <= New_Ram2EN;
	wrn <= New_wrn;
	rdn <= New_rdn;
	
	l <= Test_L;
	-- case Boot_State is
		-- when '0' => seg1 <= SEG_0;
		-- when '1' => seg1 <= SEG_1;
		-- when "010" => seg1 <= SEG_2;
		-- when "011" => seg1 <= SEG_3;
		-- when "100" => seg1 <= SEG_4;
		-- when "101" => seg1 <= SEG_5;
		-- when others =>
	-- end case;
	-- case test_L is
		-- when "0000000000000000" => seg2 <= SEG_0;
		-- when "0000000000000001" => seg2 <= SEG_1;
		-- when "0000000000000010" => seg2 <= SEG_2;
		-- when "0000000000000011" => seg2 <= SEG_3;
		-- when "0000000000000100" => seg2 <= SEG_4;
		-- when "0000000000000101" => seg2 <= SEG_5;
		-- when "0000000000000110" => seg2 <= SEG_6;
		-- when "0000000000000111" => seg2 <= SEG_7;
		-- when "0000000000001000" => seg2 <= SEG_8;
		-- when "0000000000001001" => seg2 <= SEG_9;
		-- when others             => seg2 <= "0000001";
	-- end case;
	-- case RA is
		-- when "0000000000000000" => seg1 <= SEG_0;
		-- when "0000000000000001" => seg1 <= SEG_1;
		-- when "0000000000000010" => seg1 <= SEG_2;
		-- when "0000000000000011" => seg1 <= SEG_3;
		-- when "0000000000000100" => seg1 <= SEG_4;
		-- when "0000000000000101" => seg1 <= SEG_5;
		-- when "0000000000000110" => seg1 <= SEG_6;
		-- when "0000000000000111" => seg1 <= SEG_7;
		-- when "0000000000001000" => seg1 <= SEG_8;
		-- when "0000000000001001" => seg1 <= SEG_9;
		-- when others               => seg1 <= "0000001";
	-- end case;
	case PC(15 downto 0) is
when "0000000000000000" =>
seg1 <= SEG_0; seg2 <= SEG_0;
when "0000000000000001" =>
seg1 <= SEG_0; seg2 <= SEG_1;
when "0000000000000010" =>
seg1 <= SEG_0; seg2 <= SEG_2;
when "0000000000000011" =>
seg1 <= SEG_0; seg2 <= SEG_3;
when "0000000000000100" =>
seg1 <= SEG_0; seg2 <= SEG_4;
when "0000000000000101" =>
seg1 <= SEG_0; seg2 <= SEG_5;
when "0000000000000110" =>
seg1 <= SEG_0; seg2 <= SEG_6;
when "0000000000000111" =>
seg1 <= SEG_0; seg2 <= SEG_7;
when "0000000000001000" =>
seg1 <= SEG_0; seg2 <= SEG_8;
when "0000000000001001" =>
seg1 <= SEG_0; seg2 <= SEG_9;
when "0000000000001010" =>
seg1 <= SEG_1; seg2 <= SEG_0;
when "0000000000001011" =>
seg1 <= SEG_1; seg2 <= SEG_1;
when "0000000000001100" =>
seg1 <= SEG_1; seg2 <= SEG_2;
when "0000000000001101" =>
seg1 <= SEG_1; seg2 <= SEG_3;
when "0000000000001110" =>
seg1 <= SEG_1; seg2 <= SEG_4;
when "0000000000001111" =>
seg1 <= SEG_1; seg2 <= SEG_5;
when "0000000000010000" =>
seg1 <= SEG_1; seg2 <= SEG_6;
when "0000000000010001" =>
seg1 <= SEG_1; seg2 <= SEG_7;
when "0000000000010010" =>
seg1 <= SEG_1; seg2 <= SEG_8;
when "0000000000010011" =>
seg1 <= SEG_1; seg2 <= SEG_9;
when "0000000000010100" =>
seg1 <= SEG_2; seg2 <= SEG_0;
when "0000000000010101" =>
seg1 <= SEG_2; seg2 <= SEG_1;
when "0000000000010110" =>
seg1 <= SEG_2; seg2 <= SEG_2;
when "0000000000010111" =>
seg1 <= SEG_2; seg2 <= SEG_3;
when "0000000000011000" =>
seg1 <= SEG_2; seg2 <= SEG_4;
when "0000000000011001" =>
seg1 <= SEG_2; seg2 <= SEG_5;
when "0000000000011010" =>
seg1 <= SEG_2; seg2 <= SEG_6;
when "0000000000011011" =>
seg1 <= SEG_2; seg2 <= SEG_7;
when "0000000000011100" =>
seg1 <= SEG_2; seg2 <= SEG_8;
when "0000000000011101" =>
seg1 <= SEG_2; seg2 <= SEG_9;
when "0000000000011110" =>
seg1 <= SEG_3; seg2 <= SEG_0;
when "0000000000011111" =>
seg1 <= SEG_3; seg2 <= SEG_1;
when "0000000000100000" =>
seg1 <= SEG_3; seg2 <= SEG_2;
when "0000000000100001" =>
seg1 <= SEG_3; seg2 <= SEG_3;
when "0000000000100010" =>
seg1 <= SEG_3; seg2 <= SEG_4;
when "0000000000100011" =>
seg1 <= SEG_3; seg2 <= SEG_5;
when "0000000000100100" =>
seg1 <= SEG_3; seg2 <= SEG_6;
when "0000000000100101" =>
seg1 <= SEG_3; seg2 <= SEG_7;
when "0000000000100110" =>
seg1 <= SEG_3; seg2 <= SEG_8;
when "0000000000100111" =>
seg1 <= SEG_3; seg2 <= SEG_9;
when "0000000000101000" =>
seg1 <= SEG_4; seg2 <= SEG_0;
when "0000000000101001" =>
seg1 <= SEG_4; seg2 <= SEG_1;
when "0000000000101010" =>
seg1 <= SEG_4; seg2 <= SEG_2;
when "0000000000101011" =>
seg1 <= SEG_4; seg2 <= SEG_3;
when "0000000000101100" =>
seg1 <= SEG_4; seg2 <= SEG_4;
when "0000000000101101" =>
seg1 <= SEG_4; seg2 <= SEG_5;
when "0000000000101110" =>
seg1 <= SEG_4; seg2 <= SEG_6;
when "0000000000101111" =>
seg1 <= SEG_4; seg2 <= SEG_7;
when "0000000000110000" =>
seg1 <= SEG_4; seg2 <= SEG_8;
when "0000000000110001" =>
seg1 <= SEG_4; seg2 <= SEG_9;
when "0000000000110010" =>
seg1 <= SEG_5; seg2 <= SEG_0;
when "0000000000110011" =>
seg1 <= SEG_5; seg2 <= SEG_1;
when "0000000000110100" =>
seg1 <= SEG_5; seg2 <= SEG_2;
when "0000000000110101" =>
seg1 <= SEG_5; seg2 <= SEG_3;
when "0000000000110110" =>
seg1 <= SEG_5; seg2 <= SEG_4;
when "0000000000110111" =>
seg1 <= SEG_5; seg2 <= SEG_5;
when "0000000000111000" =>
seg1 <= SEG_5; seg2 <= SEG_6;
when "0000000000111001" =>
seg1 <= SEG_5; seg2 <= SEG_7;
when "0000000000111010" =>
seg1 <= SEG_5; seg2 <= SEG_8;
when "0000000000111011" =>
seg1 <= SEG_5; seg2 <= SEG_9;
when "0000000000111100" =>
seg1 <= SEG_6; seg2 <= SEG_0;
when "0000000000111101" =>
seg1 <= SEG_6; seg2 <= SEG_1;
when "0000000000111110" =>
seg1 <= SEG_6; seg2 <= SEG_2;
when "0000000000111111" =>
seg1 <= SEG_6; seg2 <= SEG_3;
when "0000000001000000" =>
seg1 <= SEG_6; seg2 <= SEG_4;
when "0000000001000001" =>
seg1 <= SEG_6; seg2 <= SEG_5;
when "0000000001000010" =>
seg1 <= SEG_6; seg2 <= SEG_6;
when "0000000001000011" =>
seg1 <= SEG_6; seg2 <= SEG_7;
when "0000000001000100" =>
seg1 <= SEG_6; seg2 <= SEG_8;
when "0000000001000101" =>
seg1 <= SEG_6; seg2 <= SEG_9;
when "0000000001000110" =>
seg1 <= SEG_7; seg2 <= SEG_0;
when "0000000001000111" =>
seg1 <= SEG_7; seg2 <= SEG_1;
when "0000000001001000" =>
seg1 <= SEG_7; seg2 <= SEG_2;
when "0000000001001001" =>
seg1 <= SEG_7; seg2 <= SEG_3;
when "0000000001001010" =>
seg1 <= SEG_7; seg2 <= SEG_4;
when "0000000001001011" =>
seg1 <= SEG_7; seg2 <= SEG_5;
when "0000000001001100" =>
seg1 <= SEG_7; seg2 <= SEG_6;
when "0000000001001101" =>
seg1 <= SEG_7; seg2 <= SEG_7;
when "0000000001001110" =>
seg1 <= SEG_7; seg2 <= SEG_8;
when "0000000001001111" =>
seg1 <= SEG_7; seg2 <= SEG_9;
when "0000000001010000" =>
seg1 <= SEG_8; seg2 <= SEG_0;
when "0000000001010001" =>
seg1 <= SEG_8; seg2 <= SEG_1;
when "0000000001010010" =>
seg1 <= SEG_8; seg2 <= SEG_2;
when "0000000001010011" =>
seg1 <= SEG_8; seg2 <= SEG_3;
when "0000000001010100" =>
seg1 <= SEG_8; seg2 <= SEG_4;
when "0000000001010101" =>
seg1 <= SEG_8; seg2 <= SEG_5;
when "0000000001010110" =>
seg1 <= SEG_8; seg2 <= SEG_6;
when "0000000001010111" =>
seg1 <= SEG_8; seg2 <= SEG_7;
when "0000000001011000" =>
seg1 <= SEG_8; seg2 <= SEG_8;
when "0000000001011001" =>
seg1 <= SEG_8; seg2 <= SEG_9;
when "0000000001011010" =>
seg1 <= SEG_9; seg2 <= SEG_0;
when "0000000001011011" =>
seg1 <= SEG_9; seg2 <= SEG_1;
when "0000000001011100" =>
seg1 <= SEG_9; seg2 <= SEG_2;
when "0000000001011101" =>
seg1 <= SEG_9; seg2 <= SEG_3;
when "0000000001011110" =>
seg1 <= SEG_9; seg2 <= SEG_4;
when "0000000001011111" =>
seg1 <= SEG_9; seg2 <= SEG_5;
when "0000000001100000" =>
seg1 <= SEG_9; seg2 <= SEG_6;
when "0000000001100001" =>
seg1 <= SEG_9; seg2 <= SEG_7;
when "0000000001100010" =>
seg1 <= SEG_9; seg2 <= SEG_8;
when "0000000001100011" =>
seg1 <= SEG_9; seg2 <= SEG_9;



		when others =>
			seg1 <= "0000001"; seg2 <= "0000001";
	end case;
	-- case BL_Recv_State is
		-- when "000" => seg2 <= SEG_0;
		-- when "001" => seg2 <= SEG_1;
		-- when "010" => seg2 <= SEG_2;
		-- when "011" => seg2 <= SEG_3;
		-- when "100" => seg2 <= SEG_4;
		-- when "101" => seg2 <= SEG_5;
		-- when others =>
	-- end case;
	
end process;

end Behavioral;
