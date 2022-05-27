#include <iostream>
#include <fstream>
#include <ctime>
#include "../include/page_replacement.h"

#define FILENAME "access.list"

void ClearPage(Virtualpage &page_container)
{
  page_container.vread = 0;
  page_container.vwrite = 0;
  page_container.vaccess_page_number = 0;
}
void ClearCounts(RateCount &ratecount_container)
{
  ratecount_container.raccess_count = 0;
  ratecount_container.rread_count = 0;
  ratecount_container.rwrite_count = 0;
}

int main(int argc, char** argv)
{
  if(argc != 3){
    std::cerr << "Error!! check your arguments\nPlease input 2 arguments\nFirst argument should be algorithms(FIFO or NRU)\nSecond should be frame numbers\n";
    return -1;
  }
  //file (access.list) open
  std::ifstream f_in(FILENAME);
  if(f_in.fail()){
    std::cerr << "failed to open file\n";
    exit(100);
  }
  PageReplacement start_replace(argv[1], argv[2]);
  start_replace.ArgInput(); //select algorithm
  if(start_replace.GetAlgorithm() == "ERROR"){
    std::cerr << "Selected algorithm is not supported\nplease select between nru or fifo\n";
    return -1;
  }
  
  //read access.list
  std::string page_number_tmp;
  Virtualpage page_tmp;
  while(f_in >> page_number_tmp){
    if(page_number_tmp != "read" && page_number_tmp != "write"){
      page_tmp.vaccess_page_number = stoi(page_number_tmp);
    }
    if(page_number_tmp == "read"){
      page_tmp.vread = 1;
      start_replace.SetPageInput(page_tmp);
      ClearPage(page_tmp);
      continue;
    }
    else if(page_number_tmp == "write"){
      page_tmp.vwrite = 1;
      start_replace.SetPageInput(page_tmp);
      ClearPage(page_tmp);
      continue;
    }
  } 
  if(start_replace.GetAlgorithm() == "FIFO"){
    std::cout<<"FIFO\n";
    start_replace.FifoAlgorithm();
  }
  if(start_replace.GetAlgorithm() == "NRU"){
    std::cout<<"NRU\n";
    start_replace.NruAlgorithm();
  }
  //start_replace.DisplayPages(); 
  /*Display output*/
  start_replace.Display();
  /*file close*/
  f_in.close();
  return 0;
}
