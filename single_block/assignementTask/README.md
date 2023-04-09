<h1>ASSIGNMENT 1</h1>

This folder contains all the tasks (1, 2 and 3) required in assignment 1 of this module.

# Task 1
In the first task, it was required to print the transaction's details. This is done in main.c and in main.h.
The main function in main.c loops trough all the chains blocks (made up by header and transactions).
For each chain block it prints the header and then there is another loop which goes trough all the transactions. In this loop all the transactions details are printed too.

The functions used to accomplish this task are: print_transaction and print_header

# Task 2
This task required implementing a wallet feature. This task was accomplished by adding a new function to main.h and using it in main.c. 
The new function is calculate_wallet_balance. This takes as parameters the wallet_balance, the transaction and the wallet key (public key read from './keys/0). This function reads the transaction. If the wallet key is equal to the wallet_sender_key, it subtract the transaction amount from the balance. If the wallet key is equal to the wallet_recipient_key, it adds the transaction amount to the balance

# Task 3
This task was about making sure just the valid trasactions are included in the total wallet balance.
To do so I implemented a new function called verify_transaction_hash which, as suggested by the worksheet, it decrypts the signature (which it's taken from the transaction) using the sender’s
public key. Once that's done, it will take just the first 64 bits of the decrypted.
After that is finished, it will check the hash of the transaction and the one just generated. If they are equal, it means the transaction is valid, otherwise it is not.

In main.c, once this function (verify_transaction_hash) is called, the result is saved and the calculate_wallet_balance function is called only if the transaction is valid. If the transaction is not valid, then the transaction's amount is not included in the wallet balance.

# Extras
In this project, a couple of extra things have been added. The user can choose if they want to see the total balance at the end or for each transaction. The user can also decide to show the timestamps when the program is running. Or the user can decided to just see the defaults settings (jsut the details of the transactions and the header)