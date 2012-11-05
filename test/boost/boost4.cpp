#include <boost/interprocess/managed_shared_memory.hpp> 
#include <boost/interprocess/allocators/allocator.hpp> 
#include <boost/interprocess/containers/string.hpp> 
#include <iostream> 

int main() 
{ 
  boost::interprocess::shared_memory_object::remove("Highscore"); 
  boost::interprocess::managed_shared_memory managed_shm(boost::interprocess::open_or_create, "Highscore", 1024); 
  typedef boost::interprocess::allocator<char, boost::interprocess::managed_shared_memory::segment_manager> CharAllocator; 
  typedef boost::interprocess::basic_string<char, std::char_traits<char>, CharAllocator> string; 
  string *s = managed_shm.find_or_construct<string>("String")("Hello!", managed_shm.get_segment_manager()); 
  s->insert(5, ", world"); 
  std::cout << *s << std::endl; 
} 
