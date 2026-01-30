.syntax unified
.cpu cortex-m3
.thumb
.extern _estack
.extern main
.weak Reset_Handler
.weak HardFault_Handler
.section .isr_vector, "a", %progbits
.type g_pfnVectors, %object

g_pfnVectors:
    .word _estack              
    .word Reset_Handler        
    .word 0                   
    .word HardFault_Handler    
    .word 0                   
    .word 0                   
    .word 0                    
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0                    
    .word 0                    
    .word 0
    .word 0                    
    .word 0                   

.section .text.Reset_Handler
.type Reset_Handler, %function

Reset_Handler:
    bl main
    b .
.section .text.HardFault_Handler
.type HardFault_Handler, %function
.global HardFault_Handler

HardFault_Handler:
    tst lr, #4
    ite eq
    mrseq r0, msp
    mrsne r0, psp
    b hardfault_c
