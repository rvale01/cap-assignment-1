#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "main.h"

int main(){
    FILE *chain_file_ptr; // the pointer to the file where the chain is stored

    int user_default_choice = user_choice();
    int print_wallet = 2;
    int timestamp_choice = 2;
    if(user_default_choice == 2){
        // 1 to print the wallet balance for each transaction
        // 2 to print just the total balance
        print_wallet = wallet_print_choice();

        // 1 to add timestamps
        // 2 not to add timestamps
        timestamp_choice = ask_for_timestamps();

    }

    // Set the file path for the chain file and open it
    char file_path[] = "../chains/test.chain";
    chain_file_ptr = fopen(file_path, "r");

    // Check if the file was opened successfully
    if (chain_file_ptr == NULL){
        printf("The file could not be opened!\n"); // if not the program stops
        exit(1);
    } else {
        // chain which stores the header and the transactions
        chain_details chain;

        // the balance which will be updated after
        double wallet_balance = 0.0;

        // read the public key from the dile
        FILE *wallet_key_file;
        RSA *wallet_key = read_public_key();

        // If a header can be read from the file, it means the file has still some data after the first loop
        while(fread(&(chain.header), sizeof(chain_block_header_t), 1, chain_file_ptr) == 1){

            print_timestamp(timestamp_choice);
            // inside the while loop
                // the header of the block is printed
                print_header(&(chain.header));

                uint64_t transactions_no = chain.header.transactions_no;
                // memory is allocated to the list of the transactions based on the number of transactions 
                chain.transaction = malloc(sizeof(chain_block_transaction_t) * transactions_no);
                
                //loop trough all transaction
                for(uint64_t x = 0; x< transactions_no; x++){
                    print_timestamp(timestamp_choice);
                    // get the transaction and save it inside the the chain.transaction
                    fread(&(chain.transaction[x]), sizeof(chain_block_transaction_t), 1, chain_file_ptr);
                    
                    // Check if the transaction has not been compromised and it's valid
                    int is_correct = verify_transaction_hash(&(chain.transaction[x]), wallet_key);
                    if(is_correct == 1){
                        calculate_wallet_balance(wallet_key, chain.transaction[x], &wallet_balance);
                        printf("The transaction is valid!");
                    }else{
                        printf("The transaction is not valid!");
                    }
                    
                    // print the transaction's details
                    if(print_wallet == 1){
                        printf("%.2f", wallet_balance);
                    }

                    print_transaction(&(chain.transaction[x]));
                    print_timestamp(timestamp_choice);
                }
                // free the memory of the chain
                free(chain.transaction);

                printf("%.2f", wallet_balance);
        } 

        RSA_free(wallet_key);
    }


    printf("\n");
    print_timestamp(timestamp_choice);
    fclose(chain_file_ptr);
    return 0;
}