
reg [1:0] 	int_en = 2'b11;
reg		dual_bank = 1'b0;
reg		redu_en=1'b0;
reg [31:0]	rdata;
reg [31:0]	data;
reg [31:0]	addr=32'h8000_6000; 
integer		i;

`define REF_CLK 20.84

//PWM0 register
reg     int_en = 1'b1;
reg     int_clr = 1'b1;
reg [31:0] pwm_h_cnt = 32'd50;
reg [31:0] pwm_l_cnt = 32'd100;
reg     pwm_timer = 1'b0;  //o:pwm 1:timer
reg     cnt_clr ;
reg     cnt_en = 1'b1;
reg     rep;     //o :repeat 1: single
reg     output_en;
reg [7:0] pre_div = 8'h1;

  wire pwm_genclk; //= topsys.chip.core_domain0.u0_pwm_top_wrap.pwm_pad_o;
  real pwm_exp_p = `REF_CLK *( pwm_l_cnt+32'd1); 
  integer clk_t, clk_t1, clk_t2;
  integer exp_t;


initial begin

   // PWM0 function
    $display("@%t, %m MSG: ****** test name: init_test_CPU_BFM", $time);
    wait (`TB_INIT_DONE);
    $display("@%d, %m MSG: TB_INIT_DONEROM", $time);
   
    `ifdef PWM0_PORT_0
    force pwm_genclk = topsys.chip.pad_ring.GPIO[0];
    set_port_pwm0_0();
    $display("====== GPIO[0] ======");
    `elsif PWM0_PORT_2
    force pwm_genclk = topsys.chip.pad_ring.GPIO[2];
    set_port_pwm0_2();
    $display("====== GPIO[2] ======");
    `elsif PWM0_PORT_4
    force pwm_genclk = topsys.chip.pad_ring.GPIO[4];
    set_port_pwm0_4();
    $display("====== GPIO[4] ======");
    `elsif PWM0_PORT_6
    force pwm_genclk = topsys.chip.pad_ring.GPIO[6];
    set_port_pwm0_6();
    $display("====== GPIO[6] ======");
    `elsif PWM0_PORT_8
    force pwm_genclk = topsys.chip.pad_ring.GPIO[8];
    set_port_pwm0_8();
    $display("====== GPIO[8] ======");
    `elsif PWM0_PORT_12
    force pwm_genclk = topsys.chip.pad_ring.GPIO[12];
    set_port_pwm0_12();
    $display("====== GPIO[12] ======");
    `elsif PWM0_PORT_16
    force pwm_genclk = topsys.chip.pad_ring.GPIO[16];
    set_port_pwm0_16();
    $display("====== GPIO[16] ======");
    `elsif PWM0_PORT_20
    force pwm_genclk = topsys.chip.pad_ring.GPIO[20];
    set_port_pwm0_20();
    $display("====== GPIO[20] ======");
    `elsif PWM0_PORT_24
    force pwm_genclk = topsys.chip.pad_ring.GPIO[24];
    set_port_pwm0_24();
    $display("====== GPIO[24] ======");
    `elsif PWM0_PORT_28
    force pwm_genclk = topsys.chip.pad_ring.GPIO[28];
    set_port_pwm0_28();
    `elsif PWM0_PORT_32
    force pwm_genclk = topsys.chip.pad_ring.GPIO[32];
    set_port_pwm0_32();
    $display("====== GPIO[32] ======");
    `elsif PWM0_PORT_34
    force pwm_genclk = topsys.chip.pad_ring.GPIO[34];
    set_port_pwm0_34();
    $display("====== GPIO[34] ======");
    `elsif PWM0_PORT_36
    force pwm_genclk = topsys.chip.pad_ring.GPIO[36];
    set_port_pwm0_36();
    $display("====== GPIO[36] ======");
    `elsif PWM0_PORT_38
    force pwm_genclk = topsys.chip.pad_ring.GPIO[38];
    set_port_pwm0_38();
    $display("====== GPIO[38] ======");
    `elsif PWM0_PORT_40
    force pwm_genclk = topsys.chip.pad_ring.GPIO[40];
    set_port_pwm0_40();
    $display("====== GPIO[40] ======");
    `elsif PWM0_PORT_42
    force pwm_genclk = topsys.chip.pad_ring.GPIO[42];
    set_port_pwm0_42();
    $display("====== GPIO[42] ======");
    `elsif PWM0_PORT_44
    force pwm_genclk = topsys.chip.pad_ring.GPIO[44];
    set_port_pwm0_44();
    $display("====== GPIO[44] ======");
    `else
    force pwm_genclk = topsys.chip.pad_ring.GPIO[46];
    set_port_pwm0_46();
    $display("====== GPIO[46] ======");
    `endif

    repeat(4) @(posedge `CHIP_CLK48M);
    
    `CHIP_CPU_BFM.ahbwr( `PWM0_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h0000_0000 | int_en);
    `CHIP_CPU_BFM.ahbwr( `PWM0_ADDR_BASE+32'h04, 3'd0, 4'd2, pwm_h_cnt);
    `CHIP_CPU_BFM.ahbwr( `PWM0_ADDR_BASE+32'h08, 3'd0, 4'd2, pwm_l_cnt);
    `CHIP_CPU_BFM.ahbwr( `PWM0_ADDR_BASE+32'h10, 3'd0, 4'd2, pre_div);

    `CHIP_CPU_BFM.ahbwr( `PWM0_ADDR_BASE+32'h0c, 3'd0, 4'd2, 32'h0000_0000 | cnt_en << 10);
//    `CHIP_CPU_BFM.ahbrd( `I2S_ADDR_BASE+32'h00, 3'd0, 4'd2, rdata, 1, 32'h17_0109, 1'b0);

   // wait(topsys.chip.core_domain0.u0_pwm_top_wrap.pwm_int);
     wait(`PWM0_INTR);
    //wait(topsys.chip.core_domain0.watchdog_top_wrap.watchdog_top.watchdog_int);

    repeat(100)@(posedge `CHIP_CLK48M);    

    `CHIP_CPU_BFM.ahbwr( `PWM0_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h0000_0000 | int_clr << 16);
    //wait(~topsys.chip.core_domain0.watchdog_top_wrap.watchdog_top.watchdog_int);

    repeat(100)@(posedge `CHIP_CLK48M);    
     wait(~`PWM0_INTR);
    //wait(~topsys.chip.core_domain0.u0_pwm_top_wrap.pwm_int);
    
   fork
   begin:pwm_clk_period
    @(posedge pwm_genclk) clk_t1=$realtime*1000;
    @(posedge pwm_genclk) clk_t2=$realtime*1000;
    
    clk_t=clk_t2-clk_t1;//ps
    $display("@%0d, MSG: clk_t1=%0d clk_t2=%0d", $time,clk_t1,clk_t2);  
    exp_t=pwm_exp_p*1000;//ps
    disable pwm_timeout;
    if (exp_t==clk_t)begin
          $display("======= pwm PASS=========");
          $display("@%0d, MSG: exp_t=%0d ps, clk_t=%0d ps", $time,exp_t,clk_t);  
    end
    else begin 
          $display("=======pwm FAIL=========");
          $display("@%0d, ERROR_MSG: exp_t=%0d ps, clk_t=%0d ps", $time,exp_t,clk_t);  
    end 
   end
   begin: pwm_timeout
    #(pwm_exp_p*2000);
    $display("=======pwm TIME OUT FAIL=========");
    $display("@%0d, ERROR_MSG(TIME_OUT): exp_t=%0d ps, clk_t=%0d ps", $time,exp_t,clk_t);
     disable pwm_clk_period;
   end
   join_any

    repeat(10000)@(posedge `CHIP_CLK48M);    
    $finish;
end

