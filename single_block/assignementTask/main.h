#include <string.h>
#include <strings.h>
#include <inttypes.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

// this is the struct used for the block header 
typedef struct chain_header
{
    uint64_t prev_block;
    uint64_t difficulty;
    uint64_t nonce;
    time_t timestamp;
    uint64_t transactions_no;
} chain_block_header_t;

// the struct used for the transaction block
typedef struct chain_transaction
{
    time_t timestamp;
    double amount;
    uint64_t hash;
    uint8_t signature[1024];
    uint8_t wallet_recipient_key[1024];
    uint8_t wallet_sender_key[1024];
} chain_block_transaction_t;

// struct for each chain. Each chain has one header and several transactions
typedef struct chain {
    chain_block_header_t header;
    chain_block_transaction_t *transaction;
} chain_details;

// function used to print the header of the chain
void print_header (const chain_block_header_t *header){
    printf("\n\n\n\n--------- HEADER STARTING ------------\n");
    printf("Value of the prev header: %lu\n", header->prev_block);
    printf("Value of the difficulty: %X\n", (unsigned int)header->difficulty);
    printf("Value of the nonce: %lu\n", header->prev_block);
    printf("Value of the timestamp: %u\n", (unsigned int)header->timestamp);
    printf("Number of transactions: %u\n", (unsigned int)header->transactions_no);
    printf("--------- HEADER COMPLETED ------------\n\n\n\n\n");
}

// function used to print each value of the transaction
void print_transaction(const chain_block_transaction_t *transaction){
    printf("\n\n\n\n--------- TRANSACTION STARTING ------------\n");
    printf("Value of the timestamp: %u\n", (unsigned int)transaction->timestamp);
    printf("Value of the amount: %.2lf\n", transaction->amount);
    printf("Value of the hash: %u\n", (unsigned int)transaction->hash);
    printf("Value of the signature: ");

    for (int x = 0; x < 1024; x++) {
        printf("%X", transaction->signature[x]);
    }

    printf("\n\n");
    printf("Value of the wallet recipient key:");
    
    for (int x = 0; x < 1024; x++){
        printf("%X", (unsigned int)transaction->wallet_recipient_key[x]);
    }

    printf("\n\n");
    printf("Value of the wallet sender key:");
    
    for (int x = 0; x < 1024; x++){
        printf("%X", (unsigned int)transaction->wallet_sender_key[x]);
    }

    printf("\n\n");
    printf("--------- TRANSACTION COMPLETED ------------\n\n\n\n\n");
}

void exampleFunction(RSA *wallet_key) {
    // Your implementation here
    printf("ok");
}

// function used to verify the transaction
int verify_transaction_hash(const chain_block_transaction_t *transaction, RSA *wallet_key) {
    if (wallet_key == NULL) {
        printf("Error: Invalid RSA key.\n");
        exit(1);
    }
    uint8_t decrypted_signature[1024];
    int rsa_size = RSA_size(wallet_key);

    // Decrypt the signature using the sender's public key
    RSA_public_decrypt(rsa_size, (const unsigned char *)(transaction->signature), (unsigned char *)(decrypted_signature), wallet_key, RSA_PKCS1_PADDING);

    // taking only the first 64 bits of the decrypted signature as the hash
    uint64_t hash_64 = 0;
    for (uint8_t i = 0; i < 8; i++) {
        hash_64 = (hash_64 << 8) | decrypted_signature[i];
    }
    return hash_64 == transaction->hash;
}

RSA * read_public_key(){
    // Path to the PEM file containing the public key.
    char wallet_key_path[] = "../keys/0/public_key.pem";
    
    // Opening the PEM file
    FILE *wallet_key_file = fopen(wallet_key_path, "rb");
    RSA *wallet_key = RSA_new();
    
    // If there is a problem with the file, the programs stops
    if (wallet_key_file == NULL) {
        printf("The wallet key file could not be opened!\n");
        exit(1);
    }

    // Read the RSA public key from the PEM file
    PEM_read_RSA_PUBKEY(wallet_key_file, &wallet_key, NULL, NULL);

    if (wallet_key == NULL) {
        printf("Error reading the public key.\n");
        exit(1);
    }

    // Close the PEM file and return the RSA key pointer.
    fclose(wallet_key_file);
    return wallet_key;
}

double calculate_wallet_balance(RSA *wallet_key, chain_block_transaction_t transaction ) {
    double balance = 0.0;

    // Check if the wallet is the sender or recipient in the transaction
    // If the wallet is the sender, subtract the transaction amount from the balance
    // If the wallet is the recipient, add the transaction amount to the balance
    if (memcmp(&transaction.wallet_sender_key, wallet_key, 1024) == 0) {
        balance -= transaction.amount;
    } else if (memcmp(&transaction.wallet_recipient_key, wallet_key, 1024) == 0) {
        balance += transaction.amount;
    }

    // Return the balance
    return balance;
}

int wallet_print_choice()
{
    int user_choice;
    // the loops ends if the user enters 1 or 2, otherwise it will keep going
    while (1)
    {
        // Prompt the user to choose an option
        printf("Enter 1 to print the wallet balance for each transaction and the total.\n");
        printf("Enter 2 to print just the total balance.\n");
        scanf("%d", &user_choice);

        if (user_choice == 1 || user_choice == 2)
        {
            break;
        }
        else
        {   
            // If the user entered an invalid choice, prompt them again and the loop will restart
            printf("Invalid input! Please enter either 1 or 2.\n");
        }
    }
    return user_choice;
}

// function which asks the user if they want to display everything in a default way
int user_choice() {
    int choice;
    printf("Enter 1 to display just the transactions and headers details\n");
    printf("Enter 2 for additional choices:\n");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Displaying default output.\n");
    } else if (choice == 2) {
        printf("Displaying additional choices.\n");
        // Additional code for handling extra choices
    } else {
        printf("Invalid choice!\n");
        user_choice(); // Recursive call to prompt the user again
    }

    return choice;
}

// Function used to ask if the user wants to see the timestamps around
// (this can be used for testing purposes)
int ask_for_timestamps(){
    int choice;
    printf("Do you want to add timestamps to your program?\n");
    printf("Enter 1 for yes and 2 for no: ");
    scanf("%d", &choice);

    if (choice == 1){
        printf("Timestamps enabled!\n");
        // Your code for adding timestamps here
    } else {
        printf("Default display will be used.\n");
        // Your code for default display here
    }

    return choice;
}


// Function which prints the timestamp just if the user selected 1
void print_timestamp(int timestamp_choice){
    if(timestamp_choice == 1){
        time_t t = time(NULL);
        char buffer[80];
        strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&t));
        printf("Timestamp for testing: %s\n\n", buffer);
    }
}