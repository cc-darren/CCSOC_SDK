
reg [1:0] 	int_en = 2'b11;
reg		dual_bank = 1'b0;
reg		redu_en=1'b0;
reg [31:0]	rdata;
reg [31:0]	data;
reg [31:0]	addr=32'h8000_6000; 
integer		i;

`define REF_CLK 20.84

//PWM1 register
reg     int_en = 1'b1;
reg     int_clr = 1'b1;
reg [31:0] pwm_h_cnt = 32'd40;
reg [31:0] pwm_l_cnt = 32'd80;
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

   // PWM1 function
    $display("@%t, %m MSG: ****** test name: init_test_CPU_BFM", $time);
    wait (`TB_INIT_DONE);
    $display("@%d, %m MSG: TB_INIT_DONEROM", $time);

   `ifdef PWM1_PORT_1
    force pwm_genclk = topsys.chip.pad_ring.GPIO[1];
    set_port_pwm1_1();
    $display("====== GPIO[1] ======");
   `elsif PWM1_PORT_3
    force pwm_genclk = topsys.chip.pad_ring.GPIO[3];
    set_port_pwm1_3();
    $display("====== GPIO[3] ======");
   `elsif PWM1_PORT_5
    force pwm_genclk = topsys.chip.pad_ring.GPIO[5];
    set_port_pwm1_5();
    $display("====== GPIO[5] ======");
   `elsif PWM1_PORT_7
    force pwm_genclk = topsys.chip.pad_ring.GPIO[7];
    set_port_pwm1_7();
    $display("====== GPIO[7] ======");
   `elsif PWM1_PORT_9
    force pwm_genclk = topsys.chip.pad_ring.GPIO[9];
    set_port_pwm1_9();
    $display("====== GPIO[9] ======");
   `elsif PWM1_PORT_11
    force pwm_genclk = topsys.chip.pad_ring.GPIO[11];
    set_port_pwm1_11();
    $display("====== GPIO[11] ======");
   `elsif PWM1_PORT_13
    force pwm_genclk = topsys.chip.pad_ring.GPIO[13];
    set_port_pwm1_13();
    $display("====== GPIO[13] ======");
   `elsif PWM1_PORT_17
    force pwm_genclk = topsys.chip.pad_ring.GPIO[17];
    set_port_pwm1_17();
    $display("====== GPIO[17] ======");
   `elsif PWM1_PORT_21
    force pwm_genclk = topsys.chip.pad_ring.GPIO[21];
    set_port_pwm1_21();
    $display("====== GPIO[21] ======");
   `elsif PWM1_PORT_25
    force pwm_genclk = topsys.chip.pad_ring.GPIO[25];
    set_port_pwm1_25();
    $display("====== GPIO[25] ======");
   `elsif PWM1_PORT_29
    force pwm_genclk = topsys.chip.pad_ring.GPIO[29];
    set_port_pwm1_29();
    $display("====== GPIO[29] ======");
   `elsif PWM1_PORT_31
    force pwm_genclk = topsys.chip.pad_ring.GPIO[31];
    set_port_pwm1_31();
    $display("====== GPIO[31] ======");
   `elsif PWM1_PORT_33
    force pwm_genclk = topsys.chip.pad_ring.GPIO[33];
    set_port_pwm1_33();
    $display("====== GPIO[33] ======");
   `elsif PWM1_PORT_35
    force pwm_genclk = topsys.chip.pad_ring.GPIO[35];
    set_port_pwm1_35();
    $display("====== GPIO[35] ======");
   `elsif PWM1_PORT_37
    force pwm_genclk = topsys.chip.pad_ring.GPIO[37];
    set_port_pwm1_37();
    $display("====== GPIO[37] ======");
   `elsif PWM1_PORT_39
    force pwm_genclk = topsys.chip.pad_ring.GPIO[39];
    set_port_pwm1_39();
    $display("====== GPIO[39] ======");
   `elsif PWM1_PORT_41
    force pwm_genclk = topsys.chip.pad_ring.GPIO[41];
    set_port_pwm1_41();
    $display("====== GPIO[41] ======");
   `elsif PWM1_PORT_43
    force pwm_genclk = topsys.chip.pad_ring.GPIO[43];
    set_port_pwm1_43();
    $display("====== GPIO[43] ======");
   `elsif PWM1_PORT_45
    force pwm_genclk = topsys.chip.pad_ring.GPIO[45];
    set_port_pwm1_45();
    $display("====== GPIO[45] ======");
   `else
    force pwm_genclk = topsys.chip.pad_ring.GPIO[47];
    set_port_pwm1_47();
    $display("====== GPIO[47] ======");
   `endif

    repeat(4) @(posedge `CHIP_CLK48M);
    
    `CHIP_CPU_BFM.ahbwr( `PWM1_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h0000_0000 | int_en);
    `CHIP_CPU_BFM.ahbwr( `PWM1_ADDR_BASE+32'h04, 3'd0, 4'd2, pwm_h_cnt);
    `CHIP_CPU_BFM.ahbwr( `PWM1_ADDR_BASE+32'h08, 3'd0, 4'd2, pwm_l_cnt);
    `CHIP_CPU_BFM.ahbwr( `PWM1_ADDR_BASE+32'h10, 3'd0, 4'd2, pre_div);

    `CHIP_CPU_BFM.ahbwr( `PWM1_ADDR_BASE+32'h0c, 3'd0, 4'd2, 32'h0000_0000 | cnt_en << 10);
//    `CHIP_CPU_BFM.ahbrd( `I2S_ADDR_BASE+32'h00, 3'd0, 4'd2, rdata, 1, 32'h17_0109, 1'b0);
    wait(`PWM1_INTR);
   // wait(topsys.chip.core_domain0.u1_pwm_top_wrap.pwm_int);
    //wait(topsys.chip.core_domain0.watchdog_top_wrap.watchdog_top.watchdog_int);

    repeat(100)@(posedge `CHIP_CLK48M);    

    `CHIP_CPU_BFM.ahbwr( `PWM1_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h0000_0000 | int_clr << 16);
    //wait(~topsys.chip.core_domain0.watchdog_top_wrap.watchdog_top.watchdog_int);

    repeat(100)@(posedge `CHIP_CLK48M);    
    wait(~`PWM1_INTR);
   // wait(~topsys.chip.core_domain0.u1_pwm_top_wrap.pwm_int);
    
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

