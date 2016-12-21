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
static uint16_t sReg  = 0; // Status Register
static uint16_t stPr  = 0; // Stack Pointer

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

        case 0x00: // BRK impl
        break;

        case 0x01: // ORA x, ind
        break;

        case 0x05: // ORA zpg
        break;

        case 0x06: // ASL zpg
        break;

        case 0x08: // PHP impl
        break;

        case 0x09: // ORA #
        break;

        case 0x0A: // ASL a 
        break;

        case 0x0D: // ORA abs
        break;

        case 0x0E: // ASL abs
        break;

        case 0x10: // BPL rel
        break;
        
        case 0x11: // ORA ind, Y
        break;

        case 0x15: // ORA zpg, x
        break;

        case 0x16: // ASL zpg, x
        break;

        case 0x18: // CLC impl
        break;

        case 0x19: // ORA abs, y
        break;
        
        case 0x1D: // ORA abs, x
        break;

        case 0x1E: // ASL abs, x
        break;

        case 0x20: // JSR abs
        break;

        case 0x21: // AND x, ind
        break;

        case 0x24: // BIT zpg
        break;

        case 0x25: // AND zpg
        break;

        case 0x26: // ROL zpg
        break;

        case 0x28: // PLP impl
        break;

        case 0x29: // AND #
        break;

        case 0x2A: // ROL A
        break;

        case 0x2C: /// BIT abs
        break;
    
        case 0x2D: // AND abs
        break;

        case 0x2E: // ROL abs
        break;

        case 0x30: // BMI rel
        break;

        case 0x31: // AND ind, y
        break;

        case 0x35: // AND zpg, x
        break;
        
        case 0x36: // ROL zpg, x
        break;

        case 0x38: // SEC impl
        break;

        case 0x39: // AND abs, y
        break;

        case 0x3D: // AND abs, x
        break;

        case 0x3E: // ROL abs, x
        break;

        /** 
         * ADC - Add Memory to Accumulator with Carry
         * A + M + C -> A, C
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
         */

        default:
            printf("Instruction not recognized\n");
    }
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
