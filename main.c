#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define NUM_UNUSED_BYTES 8
#define K16 16384
#define K8  8192

static uint16_t count = 0; // Program Counter
static uint16_t acc   = 0; // Accumulator
static uint16_t xReg  = 0; // X Register
static uint16_t yReg  = 0; // Y Register
static uint16_t stPr  = 0; // Stack Pointer
static uint8_t sReg  = 0; // Status Register

static void execute( uint16_t instruction );
static uint16_t *parseRomFile( char *filename );


int main(int argc, char *argv[]){
    if ( argc != 2 ){
        printf( "usage: %s filename\n", argv[0] );
        return EXIT_FAILURE;
    }

    uint16_t* rom = parseRomFile( argv[1] );
    int i = 0;

    /**
    while(1){
        execute( rom[count ++] );
    }
    **/


    free(rom);
    return EXIT_SUCCESS;
}

static void execute(uint16_t instruction){
    // Top 8 bits of instruction store instruction type
    switch((uint8_t)((instruction >> 8) & 0xFF)){

        /** 
         * ADC - Add Memory to Accumulator with Carry
         * A + M + C -> A, C
         *
         * N Z C I D V
         * + + + - - +
         **/

        // Immediate
        case 0x69:
        break;

        // Zeropage
        case 0x65:
        break;

        // Zeropage, X
        case 0x75
        break;

        // Absolute
        case 0x6D:
        break;


        // Absolute, X
        case 0x7D:
        break;

        // Absolute, Y
        case 0x79:
        break;

        // (Indirect, X)
        case 0x61:
        break;

        // (Indirect), Y
        case 0x71:
        break;

        /**
         * AND - AND Memory with Accumulator
         * A AND M -> A
         *
         * N Z C I D V
         * + + - - - -
         */

        // Immediate
        case 0x29:
        break;

        // Zeropage
        case 0x25:
        break;

        // Zeropage, X
        case 0x35:
        break;

        // Absolute
        case 0x2D:
        break;

        // Absolute, X
        case 0x3D:
        break;

        // Absolute, Y
        case 0x39:
        break;

        // (Indirect, X)
        case 0x21:
        break;

        // (Indirect), Y
        case 0x31:
        break;

        /**
         * ASL - Shift Left One Bit (memory or Accumulator)
         * C <- [76543210] <- 0
         *
         * N Z C I D V
         * + + + - - -
         */

        // Accumulator
        case 0x0A:
        break;

        // Zeropage
        case 0x06:
        break;

        // Zeropage, X
        case 0x16:
        break;

        // Absolute
        case 0x0E:
        break;

        // Absolute, X
        case 0x1E:
        break;

        /**
         * BCC - Branch on Carry Clear
         * branch on c = 0
         *
         * N Z C I D V
         * - - - - - -
         */
        
        // Relative
        case 0x90:
            if ( !(sReg & SR_CARRY_MASK) ) {
                count += (int8_t)(instruction & 0xFF);
            }
        break;

        /**
         * BCS - Branch on Carry Set 
         * branch on c = 1
         *
         * N Z C I D V
         * - - - - - -
         */
        
        // Relative
        case 0x90:
            if ( sReg & SR_CARRY_MASK ) {
                count += (int8_t)(instruction & 0xFF);
            }
        break;

        /**
         * BEQ - Branch on Result Zero 
         * branch on z = 1
         *
         * N Z C I D V
         * - - - - - -
         */
        
        // Relative
        case 0x90:
            if ( sReg & SR_ZERO_MASK ) {
                count += (int8_t)(instruction & 0xFF);
            }
        break;

        /**
         * BIT - Test Bits in Memory with Accumulator
         * A AND M, M7 -> N, M6 -> V
         *
         * N  Z C I D V
         * M7 + - - - M6
         */

        // Zeropage
        case 0x24:
        break;

        // Absolute
        case 0x2C:
        break;


        /**
         * BMI - Branch on Result Minus
         * branch on N = 1
         *
         * N Z C I D V
         * - - - - - -
         */

        //Relative
        case 0x30:
        break;

        /**
         * BNE - Branch on Result not Zero
         * Branch on Z = 0
         *
         * N M Z C I D V
         * - - - - - - -
         */

        // Relative
        case 0x30:
        break;

        /**
         * BPL - Branch on Result Plus
         * Branch on N = 0
         *
         * N M Z C I D V
         * - - - - - - -
         */

        // Relative
        case 0x10:
        break;

        /**
         * BRK - Force Break
         * I <- 1, (SP + 1) <- PC + 2, (SP + 1) <- SR
         *
         * N Z C I D V
         * - - - 1 - -
         */

        // Implied
        case 0x00:
        break;



        default:
            printf("Instruction not recognized\n");
    }
}

static void adc(uint8_t value){

}

static void and(uint8_t value){
    

}

static uint16_t* parseRomFile( char *filename ){
    int c;
    FILE *file;
    file = fopen( filename, "r" );

    if ( file ) {

        /** Parse Header Info **/
        if ( 'N' != getc(file) | 'E' != getc(file) | 'S' != getc(file) | 0x1A != getc(file)){
            printf("Warning: Improper file format. Exiting!\n");
            exit(EXIT_FAILURE);
        }


        uint8_t num16kPages = (uint8_t)getc(file); 
        uint8_t num8kPages  = (uint8_t)getc(file);
        uint8_t flags6      = (uint8_t)getc(file);
        uint8_t flags7      = (uint8_t)getc(file);

        int iter; 
        for ( iter = 0; iter < NUM_UNUSED_BYTES ; iter ++){
            if( 0x00 != getc(file) ){
                printf("Error: Improper file format. Exiting!\n");
                exit(EXIT_FAILURE);
            }
        }

        
        int numBits = num16kPages * K16 + num8kPages * K8;

        uint16_t *rom = malloc( numBits );

        /** Store Instructions in Memory **/
        for(int i = 0; i < numBits/sizeof(uint16_t); i++){
            rom[i] = (getc(file) << 8) | getc(file);
        }

        return rom;

    }else{
        printf("Error: File not found. Exiting!\n");
        exit(EXIT_FAILURE);
    }
    
}
