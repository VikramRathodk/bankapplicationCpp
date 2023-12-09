#include <iostream>
#include <thread>
#include <mutex>

/*
    BankAccount class represents a simple bank account with
    deposit, withdraw, and getBalance methods
 
 */
class BankAccount {
private:
    //Balance of the bank account
    double balance; 
    // Mutex for synchronization
    std::mutex mtx; 

public:
    // Constructor initializes balance to 0
    BankAccount() : balance(0) {}

    /*Method to deposit money into the account
    
    */
    void deposit(double amount, int threadID) {
        //To ensure the thread safety
        std::lock_guard<std::mutex> lock(mtx); 
        balance += amount; 
        std::cout << "Thread " << threadID << " deposited: $" << amount << std::endl; 
    }

    // Method to withdraw money from the account
    void withdraw(double amount, int threadID) {
        std::lock_guard<std::mutex> lock(mtx); 
        if (balance >= amount) {
            balance -= amount;  
            std::cout << "Thread " << threadID << " withdrew: $" << amount << std::endl; 
        } else {
            std::cout << "Thread " << threadID << " cannot withdraw $" << amount << " (insufficient Amount)" << std::endl;
        }
    }

    // Method to get the current balance of the account
    double getBalance() const {
        return balance; 
    }
};

// Function to simulate transactions (deposits and withdrawals) on the BankAccount
void performTransactions(BankAccount& account, int threadID) {
    /*Simulating random deposits and withdrawals
        Logic:
        for each Iteration Generating the random amount
        and if the current index is even then user or  
        the is going to be deposit the amount and  for
        odd case user or thread will withdraw the amount from the Bank   
    */
    for (int i = 0; i < 5; ++i) {
        double amount = rand() % 100 + 1; 
        if (i % 2 == 0) {
            account.deposit(amount, threadID);
        } else {
            account.withdraw(amount, threadID);
        }
    }
}

int main() {
    const int numThreads = 4;
    BankAccount account; 

    std::thread threads[numThreads]; 

    // Creating  threads and calling the  performTransactions function 
    for (int i = 0; i < numThreads; ++i) {
        threads[i] = std::thread(performTransactions, std::ref(account), i);
    }

    // Wait for all threads to finish execution
    for (int i = 0; i < numThreads; ++i) {
        threads[i].join();
    }

    //final balance of the account
    std::cout << "Final Balance: $" << account.getBalance() << std::endl;

    return 0; 
}
