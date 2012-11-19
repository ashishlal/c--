#include <boost/regex.hpp> 
#include <locale> 
#include <iostream> 

int main() 
{ 
  std::string s = "mixer12-a13"; 
  boost::regex expr("[0-9]+$"); 
  boost::smatch what; 
  if (boost::regex_search(s, what, expr)) 
  { 
    std::cout << what[0] << std::endl; 
    std::cout << what[1] << " " << what[2] << std::endl; 
  } 
} 
