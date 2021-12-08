#include "PanelDriver.hpp"

void PanelDriver::init() {}
void PanelDriver::draw() {}
void PanelDriver::update(PanelBitmap bitmap, uint32_t colour) {}
void PanelDriver::update_patterned(PanelBitmap bitmap, int offset) {}
void PanelDriver::update_nomirror(PanelBitmap bitmap, uint32_t colour) {}
void PanelDriver::overdub(PanelBitmap bitmap, uint32_t colour) {}
void PanelDriver::set_pattern(unsigned char pattern[16][16]) {}