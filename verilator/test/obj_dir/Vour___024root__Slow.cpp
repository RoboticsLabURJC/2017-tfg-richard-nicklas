// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vour.h for the primary calling header

#include "verilated.h"

#include "Vour__Syms.h"
#include "Vour___024root.h"

void Vour___024root___ctor_var_reset(Vour___024root* vlSelf);

Vour___024root::Vour___024root(const char* _vcname__)
    : VerilatedModule(_vcname__)
 {
    // Reset structure values
    Vour___024root___ctor_var_reset(this);
}

void Vour___024root::__Vconfigure(Vour__Syms* _vlSymsp, bool first) {
    if (false && first) {}  // Prevent unused
    this->vlSymsp = _vlSymsp;
}

Vour___024root::~Vour___024root() {
}
