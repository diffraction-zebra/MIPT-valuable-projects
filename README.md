# MIPT-valuable-projects

Homework C++ course in MIPT.
    
shared_ptr

    + support
      + weak_ptr
      + allocator, deleter
      + allocate_shared, make_shared
      + different structs for keep elements and pointers (single operator new call in each case)
    
stack_allocator

    The main idea: right side of array is for memory, left for list of pointers on empty memory. 
    It include some logic with with the choice of the best piece of memory and tracking the elemets of list.
    + support alignment
    + if memory allocation is failed allocator throw exception
    + supporting using at some cases those allocate memory for different types at the same time
    + beautiful to the eye
    
variant

    Some strange enheritance allows overloading of constructors as same as classic overloading. 
    If classic overloading not support any type there is a constructor for not-overloading construction 
    (also it check that only one type support this - otherwise CE).
    + supporting classic <variant> functions.
