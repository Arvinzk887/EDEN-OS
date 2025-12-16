#pragma once
void pic_remap(void);
void pic_send_eoi(int irq);
void pic_set_masks(unsigned char master_mask, unsigned char slave_mask);
