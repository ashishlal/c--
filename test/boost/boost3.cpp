#include <boost/interprocess/managed_shared_memory.hpp> 
#include <boost/interprocess/allocators/allocator.hpp> 
#include <boost/interprocess/containers/string.hpp> 
#include <cstring>
#include <cstdlib>
#include <string>
#include <iostream>

int main(int argc, char *argv[])
{
  using namespace boost::interprocess;
  typedef boost::interprocess::allocator<char, boost::interprocess::managed_shared_memory::segment_manager> CharAllocator; 
  typedef boost::interprocess::basic_string<char, std::char_traits<char>, CharAllocator> string; 
  if(argc == 1){  //Parent process

      boost::interprocess::shared_memory_object::remove("MySharedMemory");

      //Create a shared memory object.
      managed_shared_memory shm (create_only, "MySharedMemory", 1024);

      string *s = shm.find_or_construct<string>("String")("Hello!", shm.get_segment_manager()); 
      std::cout << *s << std::endl;

      //Launch child process
      std::string s1(argv[0]); s1 += " child ";
      if(0 != std::system(s1.c_str()))
         return 1;
  }
  else{
      //Open already created shared memory object.
      managed_shared_memory shm (open_only, "MySharedMemory");
      std::pair<string *,std::size_t> ret = shm.find<string>("String");
      std::cout << *(ret.first) << std::endl;
  }
  return 0;
}
