#include <iostream>
#include "project3.hpp"
#include "History_Transaction_definitions.hpp"

#ifndef MARMOSET_TESTING
unsigned int Transaction::assigned_trans_id = 0;
int main(){
	History hist{};
	hist.read_history();

	std::cout<< "[Staring history]:" << std::endl;
	hist.print();
	hist.sort_by_date();

	std::cout << "[Sorted            ]:" << std::endl;
	hist.print();

	hist.update_acb_cgl();
	hist.print();

	std::cout << "[CGL for 2018      ]:" << hist.compute_cgl(2018) << std::endl;
	std::cout << "[CGL for 2019      ]:" << hist.compute_cgl(2019) << std::endl;

	return 0;
}
#endif
