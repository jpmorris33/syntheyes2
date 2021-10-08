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

signed char closeeye[] = {0,-50,0}; // Blink is now done procedurally instead of using a complex animation, so just display one frame and wait

signed char rolleye[] = {0,2,2,3,3,4,4,5,5,6,6,7,7,-20,7,7,6,6,5,5,4,4,3,3,2,2,0};

signed char startled[] = {0,8,9,-30,9,8,0};

signed char annoyed[] = {0,10,11,-30,11,10,0};

signed char blushing[] = {0,12,-30,0};

signed char fault[] = {0,13,-4,1,-4,13,-4,1,-4,13,-4,1,-4,13,-4,1,-4,13,-4,1,-4,13,-4,1,-4,13,-4,1,-4,13,-4,1,-4,13,-4,1,-4,13,-4,1,-4,13,-4,1,-4,13,-4,1,-4};

signed char owo[] = {0, 0};	// Xerian doesn't do this

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
unsigned char eye[][32] = {
    // Basic open eye (0)
    {
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00001111,0b10000000,
      0b00011111,0b11000000,
      0b00111111,0b11100000,
      0b01111111,0b11110000,
      0b01111111,0b11110000,
      0b01111111,0b11110000,
      0b01110011,0b11110000,
      0b01100001,0b11110000,
      0b01100001,0b11110000,
      0b01100001,0b11110000,
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
    // roll eye 1 (2)
    {
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00001111,0b10000000,
      0b00011111,0b11000000,
      0b00111111,0b11100000,
      0b01111111,0b11110000,
      0b01111111,0b11110000,
      0b01111111,0b11110000,
      0b01110011,0b11110000,
      0b01100001,0b11110000,
      0b01100001,0b11110000,
      0b01100001,0b11110000,
      0b01110011,0b11110000,
      0b01111111,0b11110000,
    },
    // roll eye 2 (3)
    {
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00001111,0b10000000,
      0b00011111,0b11000000,
      0b00111111,0b11100000,
      0b01111111,0b11110000,
      0b01110011,0b11110000,
      0b01100001,0b11110000,
      0b01100001,0b11110000,
      0b01100001,0b11110000,
      0b01110011,0b11110000,
      0b01111111,0b11110000,
      0b01111111,0b11110000,
      0b01111111,0b11110000,
      0b00111111,0b11100000,
      0b00011111,0b11000000,
    },
    // roll eye 3 (4)
    {
      0b00000111,0b11000000,
      0b00011111,0b11100000,
      0b00111111,0b11110000,
      0b00111100,0b11111000,
      0b00111000,0b01111000,
      0b00111000,0b01111000,
      0b00111000,0b01111000,
      0b00111100,0b11111000,
      0b00111111,0b11111000,
      0b00111111,0b11111000,
      0b00111111,0b11111000,
      0b00111111,0b11111000,
      0b00011111,0b11110000,
      0b00001111,0b11100000,
      0b00000111,0b11000000,
      0b00000000,0b00000000,
    },
    // roll eye 4 (5)
    {
      0b00001111,0b11111000,
      0b00011111,0b00111100,
      0b00011110,0b00011100,
      0b00011110,0b00011100,
      0b00011110,0b00011100,
      0b00011111,0b00111100,
      0b00011111,0b11111100,
      0b00011111,0b11111100,
      0b00011111,0b11111100,
      0b00011111,0b11111100,
      0b00001111,0b11111000,
      0b00000111,0b11110000,
      0b00000011,0b11100000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
    },
    // roll eye 5 (6)
    {
      0b00001111,0b11001110,
      0b00001111,0b10000110,
      0b00001111,0b10000110,
      0b00001111,0b10000110,
      0b00001111,0b11001110,
      0b00001111,0b11111110,
      0b00001111,0b11111110,
      0b00001111,0b11111110,
      0b00001111,0b11111110,
      0b00000111,0b11111100,
      0b00000011,0b11111000,
      0b00000001,0b11110000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
    },
    // roll eye 6 (7)
    {
      0b00000111,0b11000011,
      0b00000111,0b11000011,
      0b00000111,0b11000011,
      0b00000111,0b11100111,
      0b00000111,0b11111111,
      0b00000111,0b11111111,
      0b00000111,0b11111111,
      0b00000011,0b11111110,
      0b00000001,0b11111100,
      0b00000000,0b11111000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
    },
    // startled 1 (8)
    {
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00001111,0b10000000,
      0b00011111,0b11000000,
      0b00111111,0b11100000,
      0b01111111,0b11110000,
      0b01111111,0b11110000,
      0b01111111,0b11110000,
      0b01111111,0b11110000,
      0b01110011,0b11110000,
      0b01110011,0b11110000,
      0b01110011,0b11110000,
    },
    // startled 2 (9)
    {
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00001111,0b10000000,
      0b00011111,0b11000000,
      0b00111111,0b11100000,
      0b01111111,0b11110000,
      0b01111111,0b11110000,
      0b01111111,0b11110000,
      0b01111111,0b11110000,
      0b01110011,0b11110000,
      0b01110011,0b11110000,
      0b01111111,0b11110000,
    },
  // Annoyed 1 (10)
    {
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000001,0b10000000,
      0b00000111,0b11000000,
      0b00011111,0b11100000,
      0b00111111,0b11110000,
      0b01111111,0b11110000,
      0b01111111,0b11110000,
      0b01110011,0b11110000,
      0b01100001,0b11110000,
      0b01100001,0b11110000,
      0b01100001,0b11110000,
    },
// Annoyed 2 (11)
    {      
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b01000000,
      0b00000001,0b11100000,
      0b00000111,0b11110000,
      0b00011111,0b11110000,
      0b01111111,0b11110000,
      0b01110011,0b11110000,
      0b01100001,0b11110000,
      0b01100001,0b11110000,
      0b01100001,0b11110000,
    },
// Blush (12)
    {
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00001111,0b10000000,
      0b00011111,0b11000000,
      0b00111111,0b11100000,
      0b01111111,0b11110000,
      0b01111111,0b11110000,
      0b01110011,0b11110000,
      0b01110011,0b11110000,
      0b01111111,0b11110000,
      0b00000000,0b00000000,
      0b00000000,0b00000000,
      0b00000000,0b00000000
    },
    // fault (13)
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

unsigned char blush[][32] = {
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
};
