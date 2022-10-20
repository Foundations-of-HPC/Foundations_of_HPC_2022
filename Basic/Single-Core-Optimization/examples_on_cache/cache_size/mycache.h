#if !defined(L1WORDS)
#define L1SIZE   32768             // guess for the L1 capacity (32KB)
#define L1WORDS (L1SIZE / WORD)    // guess for the word capacity of L1
#endif
#if !defined(L2WORDS)
#define L2SIZE 262144              // guess for the L2 capacity (256KB)
#define L2WORDS (L2SIZE/WORD)      // guess for the L2 word capacity
#endif
#if !defined(L3WORDS)
#define L3SIZE  8388608            // guess for the L3 capacity ( 8192K )
#define L3WORDS (L3SIZE/WORD)      // guess for the L3 word capacity
#endif
