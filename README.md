This is a temporary implementation of an STL-like library.
The main goal is to learn new features, which are quite extensive in the library.

What is worth learning from std::vector:
- Using allocators vs operator new / operator delete vs std::construct_at / std::destroy_at
- Self-assignment handling
- Exception safety guarantees
- trivially_copyable optimizations