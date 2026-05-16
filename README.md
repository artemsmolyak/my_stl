This is a temporary implementation of an STL-like library.
The main goals are 
- to learn new features, which are quite extensive in the library
- do experiments with different allocators and containers


What is worth learning from std::vector:
Allocation:
- Using allocators vs operator new / operator delete vs std::construct_at / std::destroy_at vs std::allocator_traits<A>
- instead of your allocaters you can try to use std::unique_ptr
- empty base optimisation (EBO) can be done in 2 ways - private inheritance or [[no_unique_address]].

Push back:
- Self-assignment handling (problem when push_back(vector[0]) during reallocation)
- Exception safety guarantees
- trivially copyable optimizations


What is worth learning from smart pointers:




// Good references:
// - Patrice Roy: "C++ Memory Management"
// - Clang/libstdc++ STL implementation
// - Ilya Mesharin (Mesherin Ilya): https://www.youtube.com/@mesyarik