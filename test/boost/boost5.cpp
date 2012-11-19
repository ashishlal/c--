#include <boost/regex.hpp> 
#include <boost/lexical_cast.hpp>
#include <iostream> 

int main() 
{ 
  std::string s = "mixer-a2"; 
  boost::regex expr("[0-9]+$"); 
  boost::smatch what; 
  if (boost::regex_search(s, what, expr)) 
  { 
    std::cout << what[0] << std::endl; 
    unsigned index = boost::lexical_cast<unsigned>(what);
    std::cout << index << std::endl; 
  } 
} 
