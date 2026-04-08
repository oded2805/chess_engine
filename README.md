# chess_engine
Features-
Board representation and legal move generation
| Search | Alpha-Beta Pruning with Quiescence & Iterative Deepening |
| Memory | Two-slot Transposition Table (32MB default) |
| Heuristics | MVV-LVA Move Ordering & Tapered Piece-Square Tables |

The engine uses a fixed-size transposition table for caching previously evaluated positions.

The table size is controlled by a compile-time constant (TABLE_SIZE)
TABLE_SIZE must be a power of 2 (required for fast indexing using bit masking)
The current value is relatively large to improve hit rate, but can be reduced if needed (e.g., for memory constraints)

Using make:
make

and to clean 
make clean

Run
./chess

This project is still evolving and may change as new ideas are explored.
