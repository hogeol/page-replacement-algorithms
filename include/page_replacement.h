#ifndef __SIM__
#define __SIM__

#include <queue>
#include <string>

struct RateCount
{
	int raccess_count = 0;
	int rread_count = 0;
	int rwrite_count = 0;
	int rhit_number = 0;
	int rfault_number = 0;
};

struct Virtualpage
{
	bool vread = 0;
	bool vwrite = 0;
	int vaccess_page_number;
};

class PageReplacement
{
	private:
	std::string malgorithm;
	int mframe_number;
	RateCount mcounter;
	std::queue<Virtualpage> mpage_container;
	
	public:
	PageReplacement(std::string algorithm, std::string frame_number): malgorithm(algorithm), mframe_number(stoi(frame_number)) {} //Constructor
	void ArgInput();	//Recieve algorithms and frame number
	void Display(); //Display on screen after calculate all rates
	void FifoAlgorithm(); //Calculate hit and fault based on FIFO (the input argument is ordered access address)
	void NruAlgorithm(); //Calculate hit and fault based on NRU (the input argument is ordered access address)
	/*Getter and Setter*/
	std::string GetAlgorithm();
	int GetFrameNumber();
	void SetFrameNumber(int frame_number);
	void SetAlgorithm(std::string algorithm);
	void SetPageInput(const Virtualpage &page_input);
	void CounterSetting(const Virtualpage &access_page);
	void DisplayPages();
};


#endif __SIM__
