`timescale 1ns / 1ps
`default_nettype none

module pulsegen
(
    input wire clk, 
    input wire rst,
    input wire start,
    output reg pulse    
);

    wire    h32;

    div #(.fout(32))    d32 (clk, h32);
    
    always @* begin
        if (!start || rst)
            pulse = 1'b0;
        else
            pulse = h32;
    end

endmodule