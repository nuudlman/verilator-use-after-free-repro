#include "pulsegen.h"


int main(int argc, char **argv)
{
    const auto ctx = std::make_unique<VerilatedContext>();
    ctx->commandArgs(argc, argv);
    ctx->traceEverOn(true);

    const auto trace = std::make_unique<VerilatedVcdC>();
    const auto top = std::make_unique<pulsegen>();

    top->trace(trace.get(), 2);
    trace->open("pulse.vcd");

    #define TICK() do { ctx->timeInc(1); top->eval(); trace->dump(ctx->time()); } while (0)
    #define CLOCK() do { top->clk = !top->clk; } while (0)
    #define CYCLE() do { CLOCK(); TICK(); CLOCK(); TICK(); } while (0)

    top->clk = 0;
    top->rst = 1;
    top->start = 1;

    fmt::print("resetting pulse for 10 cycles\n");

    for (int i = 0; i < 10; ++i)
        CYCLE();

    fmt::print("starting pulse @ 32hz\n");
    top->rst = 0;

    int i = 0;
    while (i < (1 << 30)) {
        CYCLE();
        i++;
    }
        
    fmt::print("ending pulse @ 32hz\n");
    trace->close();
    top->final();
    return 0;
}