#include "../include/page_replacement.h"
#include <iostream>
#include <vector>
#include <ctime>

void PageReplacement::ArgInput()
{
  if(malgorithm == "NRU" || malgorithm == "nru"){
    malgorithm = "NRU";
  }
  else if(malgorithm == "FIFO" || malgorithm == "fifo"){
    malgorithm = "FIFO";
  }
  else{
    malgorithm = "ERROR";
  }
}

void PageReplacement::Display()
{
  double page_fault_rate = ((double)mcounter.rfault_number / (double)mcounter.raccess_count)*100;
  std::cout << "Physical frame number : " << mframe_number << std::endl;
  std::cout << "Total number of access: " << mcounter.raccess_count << std::endl;
  std::cout << "Total number of read  : " << mcounter.rread_count << std::endl;
  std::cout << "Total number of write : " << mcounter.rwrite_count << std::endl;
  std::cout << "Number of page hits   : " << mcounter.rhit_number << std::endl;
  std::cout << "Number of page faults : " << mcounter.rfault_number << std::endl;
  std::cout << "Page fault rate       : " << mcounter.rfault_number << "/" << mcounter.raccess_count << " = "<< page_fault_rate << "%" << std::endl;
}

void PageReplacement::FifoAlgorithm()
{
  clock_t start, finish;
  start = clock();
  std::vector<int> physic_memory;
  std::vector<int>::iterator physic_iter; //iterator of memory
  bool is_hit=0;
  /*first input page with page fault count*/
  physic_memory.push_back(mpage_container.front().vaccess_page_number);
  CounterSetting(mpage_container.front());
  mpage_container.pop();
  mcounter.rfault_number++;
  /*calculate hit or fault*/
  while(mpage_container.size()){
    /*page hit*/
    CounterSetting(mpage_container.front());
    for(physic_iter = physic_memory.begin(); physic_iter != physic_memory.end(); physic_iter++){
      if(mpage_container.front().vaccess_page_number == *physic_iter){
        mcounter.rhit_number++;
        mpage_container.pop();
        //std::cout<< "hit" << std::endl;
        is_hit = true;
        break;
      }
    }
    /*page fault*/
    if(is_hit == false){
      mcounter.rfault_number++;
      physic_iter = physic_memory.begin();
      if(physic_memory.size() >= mframe_number)
        physic_memory.erase(physic_iter);
      physic_memory.push_back(mpage_container.front().vaccess_page_number);
      mpage_container.pop();
      //std::cout<< "fault" << std::endl;
    }
    is_hit = 0;
  }
  finish = clock();
  double dura = (double)(finish - start) / CLOCKS_PER_SEC;
  std::cout << "Processing time       : " << dura << "s" << std::endl;
}

void PageReplacement::NruAlgorithm()
{
  clock_t start, finish;
  start = clock();
  std::vector<Virtualpage> physic_memory;
  std::vector<Virtualpage>::iterator physic_iter; //iterator of memory
  bool is_hit=false;
  bool rwcnt=0;
  bool rcnt=0;
  bool wcnt=0;
  physic_memory.push_back(mpage_container.front());
  CounterSetting(mpage_container.front());
  mcounter.rfault_number++;
  mpage_container.pop();
  std::vector<Virtualpage>::iterator delete_iter=physic_memory.begin();
  while(mpage_container.size()){
    CounterSetting(mpage_container.front());
    for(physic_iter = physic_memory.begin(); physic_iter != physic_memory.end(); physic_iter++){
    /*page hit*/
      if(mpage_container.front().vaccess_page_number == (*physic_iter).vaccess_page_number){
        mcounter.rhit_number++;
        if(mpage_container.front().vread == 1){
          (*physic_iter).vread = 1;
        }
        if(mpage_container.front().vwrite == 1){
          (*physic_iter).vwrite = 1;
        }
        Virtualpage tmp_page=(*physic_iter);
        delete_iter = physic_iter;
        physic_memory.erase(delete_iter);
        physic_memory.push_back(tmp_page);
        mpage_container.pop();
        is_hit = true;
        break;
      }
    }
    /*page fault*/
    if(is_hit == false){
      mcounter.rfault_number++;
      /*if memory size iv over frame number*/
      if(physic_memory.size() >= mframe_number){
        for(physic_iter = physic_memory.begin(); physic_iter != physic_memory.end(); physic_iter++){
          /*first check not read and write access*/
          if((*physic_iter).vread == false && (*physic_iter).vwrite == false){
            delete_iter = physic_iter;
            break;
          }
          /*second check just read access*/
          if(rcnt==1)
            continue;
          if((*physic_iter).vread == true && (*physic_iter).vwrite == false){
            rcnt=1;
            delete_iter = physic_iter;
          }
          /*third check just write access*/
          if(wcnt==1 || rcnt==1)
            continue;
          if((*physic_iter).vread == false && (*physic_iter).vwrite == true){
            wcnt=1;
            delete_iter = physic_iter;
          }
          /*last check read and write access*/
          if(rwcnt==1 || rcnt==1 || wcnt==1)
            continue;
          if((*physic_iter).vread == true && (*physic_iter).vwrite == true){
            delete_iter = physic_iter;
            rwcnt=1;
          }
        }
      }
      /*if size of page is over than frame number, erase page based on NRU algorithm*/
      if(physic_memory.size() >= mframe_number)
        physic_memory.erase(delete_iter);
      physic_memory.push_back(mpage_container.front());
      mpage_container.pop();
    }
    rcnt=0;
    wcnt=0;
    rwcnt=0;
    is_hit = false;
  }
  finish = clock();
  double dura = (double)(finish - start) / CLOCKS_PER_SEC;
  std::cout << "Processing time       : " << dura << "s" << std::endl;
}

std::string PageReplacement::GetAlgorithm()
{
  return malgorithm;
}

int PageReplacement::GetFrameNumber()
{
  return mframe_number;
}

void PageReplacement::SetAlgorithm(std::string algorithm)
{
  malgorithm = algorithm;
}

void PageReplacement::SetFrameNumber(int frame_number)
{
  mframe_number = frame_number;
}

void PageReplacement::SetPageInput(const Virtualpage &page_input)
{
  mpage_container.push(page_input);
}
void PageReplacement::CounterSetting(const Virtualpage &access_page)
{
  mcounter.raccess_count++;
  if(access_page.vread == true)
    mcounter.rread_count++;
  if(access_page.vwrite == true)
    mcounter.rwrite_count++;
}

void PageReplacement::DisplayPages()
{
  while(mpage_container.size()){
    std::cout<<"---\nPage number: "<<mpage_container.front().vaccess_page_number<<", Read: "<<mpage_container.front().vread<<", Write: "<<mpage_container.front().vwrite<<std::endl; 
    mpage_container.pop();
  }
}
