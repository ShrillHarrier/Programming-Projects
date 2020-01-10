#ifndef _HISTORY_TRANSACTION_HPP_
#define _HISTORY_TRANSACTION_HPP_

#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>

#include "project3.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK
//
Transaction::Transaction(std::string ticker_symbol, unsigned int day_date, unsigned int month_date, unsigned year_date, bool buy_sell_trans, unsigned int number_shares, double trans_amount ){
	symbol = ticker_symbol;
	day = day_date;
	month = month_date;
	year = year_date;

	if(buy_sell_trans == true){
		trans_type = "Buy";
	}
	else{
		trans_type = "Sell";
	}

	shares = number_shares;
    amount = trans_amount;
    trans_id = assigned_trans_id;
    assigned_trans_id++;

    acb = 0;
    acb_per_share = 0;
    share_balance = 0;
    cgl = 0;
}



// Destructor
// TASK
//
Transaction::~Transaction() {
	//Fix This
	/*delete p_next;
	p_next = nullptr;*/
}


// TASK
// Overloaded < operator.
//
bool Transaction::operator<(Transaction const &other){
	//Fix Later
	if(((this->year) * 360 + (this->month) * 30 + (this->day)) < (other.year * 360 + other.month * 30 + other.day)){
		return true;
	}
	else{
		return false;
	}
}

// GIVEN
// Member functions to get values. 
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values. 
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " " 
    << std::setw(4) << get_symbol() << " " 
    << std::setw(4) << get_day() << " "  
    << std::setw(4) << get_month() << " " 
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) { 
    std::cout << "  Buy  "; 
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " " 
    << std::setw(10) << get_amount() << " " 
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl() 
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TASK
//
History::History(){
	p_head = nullptr;
}


// Destructor
// TASK
//
History::~History(){
	Transaction *current = get_p_head();
	Transaction *next = nullptr; //= current->get_next();

	//Fix Later

	while(current != nullptr){
		next = current->get_next();
		delete current;

		current = next;
	}
}


// TASK
// read_transaction(...): Read the transaction history from file. 
//
void History::read_history(){
	ece150::open_file();

	while(ece150::next_trans_entry()){
		Transaction *trans = new Transaction(ece150::get_trans_symbol(), ece150::get_trans_day(), ece150::get_trans_month(), ece150::get_trans_year(), ece150::get_trans_type(), ece150::get_trans_shares(), ece150::get_trans_amount());
		trans->set_next(nullptr);
		insert(trans);

	}

	ece150::close_file();
}


// insert(...): Insert transaction into linked list.
//
void History::insert(Transaction *p_new_trans){
		Transaction *temp = p_head;

	    if (p_head == nullptr){
	        p_head = p_new_trans;
	        p_new_trans->set_next(nullptr);
	    }

	    else{
	        while (temp->get_next() != nullptr){
	            temp = temp->get_next();
	        }
	        temp->set_next(p_new_trans);
	        p_new_trans->set_next(nullptr);
	    }
}


// TASK
// sort_by_date(): Sort the linked list by trade date.
//
void History::update_acb_cgl(){
		Transaction *current = p_head;
		double total = 0;
		unsigned int balance = 0;
		double currentratio = 0;

		while(current != nullptr){
			bool buysell = current->get_trans_type();
			if(buysell == true){
				total += current->get_amount();
				current->set_acb(total);
				current->set_share_balance(balance + current->get_shares());
				balance = current->get_share_balance();
				current->set_acb_per_share(current->get_acb() / current->get_share_balance());
				currentratio = current->get_acb_per_share();
				current->set_cgl(0);
			}
			else{
				current->set_acb(total - (current->get_shares() * currentratio));
				total = current->get_acb();
				current->set_share_balance(balance-current->get_shares());
				balance = current->get_share_balance();
				current->set_acb_per_share(current->get_acb() / current->get_share_balance());
				current->set_cgl(current->get_amount()-(current->get_shares()*currentratio));
			}

			current = current->get_next();
		}
}



// TASK
// sort_by_date(): Sort the linked list by trade date.
//
void History::sort_by_date(){
		Transaction *p_b4 = nullptr;
	 	Transaction *old_head = p_head;
	    Transaction *trav = old_head;
	    Transaction *p_min = trav;
	    p_head = nullptr; //Sorted Linked List

	    while (old_head != nullptr){
	        //Find Min
	        while (trav->get_next() != nullptr){

	            if (*trav->get_next() < *p_min){
	                p_min = trav->get_next();
	                p_b4 = trav;
	            }
	            trav = trav->get_next();
	        }

	        if (p_b4 == nullptr){
	            old_head = p_min->get_next();
	        }
	        else{
	            p_b4->set_next(p_min->get_next());
	        }
	        trav = old_head;
	        insert(p_min);
	        p_min = trav;
	        p_b4 = nullptr;
	    }
}



// TASK
// update_acb_cgl(): Updates the ACB and CGL values.
//


// TASK
// compute_cgl(): )Compute the ACB, and CGL.
//
double History::compute_cgl(unsigned int year){
	Transaction *current = p_head;
				double total = 0;

				while(current != nullptr){
					bool buysell = current->get_trans_type();
					if(buysell == false && (current->get_year() == year)){
						total += current->get_cgl();
					}
					current = current->get_next();
				}

				return total;
}


// TASK
// print() Print the transaction history.
//
void History::print(){
		std::cout<<"=========== BEGIN TRANSACTION HISTORY ==========="<<std::endl;

		Transaction *current = p_head;

		while(current != nullptr){
			//std::cout<<current->get_trans_id()<<"	"<<current->get_symbol()<<"	"<<current->get_day()<<"	"<<current->get_month()<<"	"<<current->get_year()<<"	"<<current->get_trans_type()<<"	"<<current->get_shares()<<"	"<<current->get_amount()<<"	"<<current->get_acb()<<"	"<<current->get_share_balance()<<"	"<<current->get_acb_per_share()<<"	"<<current->get_cgl()<<std::endl;
			current->print();
			current = current->get_next();
		}

		std::cout<<"============ END TRANSACTION HISTORY ============"<<std::endl;
}




// GIVEN
// get_p_head(): Full access to the linked list.
// 
Transaction *History::get_p_head() { return p_head; }

#endif
