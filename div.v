`timescale 1ns / 1ps
`default_nettype none

module div
#(
    parameter fin = 100000000, // default to 100Mhz
    parameter fout = 50
)
(
    input wire clk,
    output reg slow
);

    `define width ((fin / fout) - 1)

    reg [$clog2(`width) : 0] counter;

    initial begin
        slow = 0;
        counter = 0;
        $display("[DIV] intializing width to %d", `width);
    end

    always @(posedge clk)
        // verilator lint_off WIDTHEXPAND
        if (counter == `width) begin
            counter <= 0;
            slow <= ~slow;
        end
        else
            counter <= counter + 1;
endmodule