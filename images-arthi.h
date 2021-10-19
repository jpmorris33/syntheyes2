//
//	Xerian animation data
//

//
// Animation sequences
//
// These are indicies into the sprite list, e.g. 0 is fully open, 1 is blank (closed), 8 is the first frame of the startled animation etc.
// If you insert or remove any existing frames, you'll need to bump the numbers along!
// Negative numbers indicate a delay in wait cycles (60ms) rather than a frame number.
// See the comments in the sprite data below for the ID of each frame
//


// NOTE: Animations should always start with the default expression (e.g. 0) as they go back to the first frame in the list when the sequence finishes!

signed char arthi_closeeye[] = {0,-50,0}; // Blink is now done procedurally instead of using a complex animation, so just display one frame and wait

signed char arthi_rolleye[] = {0, 0}; // Arthi doesn't do this yet

signed char arthi_startled[] = {0, 0}; // Arthi doesn't do this yet

signed char arthi_annoyed[] = {0, 5, 6, -30, 6, 5, 0, -10};

signed char arthi_blushing[] = {0,0,-30,0}; // Arthi doesn't quite do this yet

signed char arthi_fault[] = {0,7,-4,1,-4,7,-4,1,-4,7,-4,1,-4,7,-4,1,-4,7,-4,1,-4,7,-4,1,-4,7,-4,1,-4,7,-4,1,-4,7,-4,1,-4,7,-4,1,-4,7,-4,1,-4,7,-4,1,-4};

signed char arthi_owo[] = {0, 2, 3, 4, -30, 4, 3, 2, 0, -10};

signed char arthi_happy[] = {0,8,-30,0};

// Overlay animation sequence - must have the same number of steps as the eye animation

signed char arthi_ovl_blushing[] = {0, 1, -30, 0};

signed char arthi_ovl_fault[] = {0,2,-4,0,-4,2,-4,0,-4,2,-4,0,-4,2,-4,0,-4,2,-4,0,-4,2,-4,0,-4,2,-4,0,-4,2,-4,0,-4,2,-4,0,-4,2,-4,0,-4,2,-4,0,-4,2,-4,0,-4};



//
//  Sprite data
//
// These are 16x16 monochrome sprites, 0 is dark, 1 is lit
// The first byte is the top-left corner, second byte is the top-right corner.
// The third byte is the left 8 pixels of the second row, fourth is the right 8 pixels and so on.
//
// If customising the sprites, you may find it easier to express these in binary,
// e.g.  B00010000, B00111000
//
// You could also add extra animation frames and logic, e.g. to roll the eyes
// Or to make them animate a pixel at a time instead of two pixels
//
// Note that these are currently stored in dynamic memory so there's a 2KB limit,
// As of this writing we're using 650 bytes, and each sprite takes 32 bytes.
// Putting the sprites into program memory will give you about 30KB to play with
// but you'd have to modify the software to read them from that address space.
//

// Right eye (facing left)
unsigned char arthi_eye[][32] = {
    // Basic open eye (0)
    {
      0b00000000, 0b00000000,
      0b00000000, 0b00000000,
      0b00000000, 0b00000000,
      0b00000000, 0b00000000,
      0b00000011, 0b11000000,
      0b00001111, 0b11111000,
      0b00001111, 0b11111100,
      0b00101111, 0b11111110,

      0b01101111, 0b11111111,
      0b01101111, 0b11111111,
      0b11101111, 0b11111111,
      0b11101111, 0b11111111,
      0b01111111, 0b11111110,
      0b00000111, 0b11100000,
      0b00000000, 0b00000000,
      0b00000000, 0b00000000,
    },
    // closed (blank) (1)
    {
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
    },
    // OwO eye 1 (2)
    {
      0b00000000, 0b00000000,
      0b00000000, 0b00000000,
      0b00000000, 0b00000000,
      0b00000011, 0b11000000,
      0b00000111, 0b11111000,
      0b00000111, 0b11111100,
      0b00110111, 0b11111110,
      0b01110111, 0b11111110,
      0b01110111, 0b11111110,
      0b01110111, 0b11111110,
      0b01111111, 0b11111110,
      0b00111111, 0b11111100,
      0b00001111, 0b11110000,
      0b00000111, 0b11100000,
      0b00000000, 0b00000000,
      0b00000000, 0b00000000,
    },
    // OwO eye 2 (3)
    {
      0b00000000, 0b00000000,
      0b00000000, 0b00000000,
      0b00000011, 0b11000000,
      0b00000011, 0b11111000,
      0b00010011, 0b11111100,
      0b00110011, 0b11111100,
      0b01110011, 0b11111110,
      0b01110011, 0b11111110,
      
      0b01110011, 0b11111110,
      0b01111111, 0b11111110,
      0b01111111, 0b11111110,
      0b00111111, 0b11111100,
      0b00011111, 0b11110000,
      0b00000111, 0b11100000,
      0b00000000, 0b00000000,
      0b00000000, 0b00000000,
    },
    // OwO eye 3 (4)
    {
      0b00000000,0b00000000,
      0b00000001,0b11000000,
      0b00001001,0b11110000,
      0b00011001,0b11111000,
      0b00111001,0b11111100,
      0b00111001,0b11111100,
      0b01111001,0b11111110,
      0b01111101,0b11111110,
      0b01111111,0b11111110,
      0b01111111,0b11111110,
      0b00111111,0b11111100,
      0b00111111,0b11111100,
      0b00011111,0b11111000,
      0b00001111,0b11110000,
      0b00000011,0b11000000,
      0b00000000,0b00000000,
    },
    // annoyed 1 (5)
    {
      0b00000000, 0b00000000,
      0b00000000, 0b00000000,
      0b00000000, 0b00000000,
      0b00000000, 0b00000000,
      0b00000000, 0b00000000,
      0b00000000, 0b00000000,
      0b00000111, 0b11111100,
      0b00101111, 0b11111111,
      0b01101111, 0b11111111,
      0b01101111, 0b11111111,
      0b11101111, 0b11111111,
      0b01111111, 0b11111110,
      0b00011111, 0b11111000,
      0b00000111, 0b11100000,
      0b00000000, 0b00000000,
      0b00000000, 0b00000000,
    },
    // annoyed 2 (6)
    {
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b01110111,0b11111111,
      0b11110111,0b11111111,
      0b11110111,0b11111110,
      0b01111111,0b11111100,
      0b00111111,0b11111000,
      0b00011111,0b11110000,
      0b00000111,0b11000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
    },
    // fault (7)
    {
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00011000,0b00011000,
      0b00001100,0b00110000,
      0b00000110,0b01100000,
      0b00000011,0b11000000,
      0b00000001,0b10000000,
      0b00000011,0b11000000,
      0b00000110,0b01100000,
      0b00001100,0b00110000,
      0b00011000,0b00011000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
    },
    // happy (8)
    {
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00001111,0b10000000,
      0b00011111,0b11000000,
      0b00111000,0b01100000,
      0b01110000,0b00010000,
      0b01100000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
    }

  };

unsigned char arthi_overlay[][32] = {
    // Nothing (0)
    {
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
    },
// Blush (1)
    {
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b01100110,0b01100000,
      0b11001100,0b11000000
    },
// Fault overlay (2)
    {
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00011000,0b00011000,
      0b00001100,0b00110000,
      0b00000110,0b01100000,
      0b00000011,0b11000000,
      0b00000001,0b10000000,
      0b00000011,0b11000000,
      0b00000110,0b01100000,
      0b00001100,0b00110000,
      0b00011000,0b00011000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
     }
};
