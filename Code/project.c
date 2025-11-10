#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h> 
#endif
#include <time.h>


#define MAX_NAME 30
#define MAX_EMAIL 40
#define MAX_PHONE 25
#define MAX_PASS 32
#define MAX_USERS 100
#define XOR_KEY 0x5A     

struct User {
    char name[MAX_NAME];
    char email[MAX_EMAIL];
    char phone[MAX_PHONE];
    char password[MAX_PASS];
    int balance;
    int isBlocked;
};

struct Slip{
	char yesOrNo;
	char type[21];
} wantedSlip;
	
struct User users[MAX_USERS];
int noOfUsers = 0;

// Function prototypes
int createYourAccount();
int loginYourAccount();
void stripNewline(char *str);
void xorCipher(char *str);
char *reversePassword(char password[MAX_PASS]);
void printOptions(struct User *u);
void balanceInquiry(struct User *u);
int fastCash(struct User *u);
int loadUsers(struct User users[]);
void saveUsers(struct User users[], int count);
int cashWithdrawal(struct User *u);
void deposit(struct User *u);
void askingForSlip(char type[20]);
void transactionSlip(int amount,int balance);


int main() {
//    Enable ANSI escape sequences in Windows console
     HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
     DWORD dwMode = 0;
     GetConsoleMode(hOut, &dwMode);
     dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
     SetConsoleMode(hOut, dwMode);

    int option;
    noOfUsers = loadUsers(users);   

   printf("\n\t\t\t\033[34m SAA BANK \033[0m\n");
    printf("\t\t   \033[34mThe Name Of Trust\033[0m\n\n");
    printf("\033[32mChoose one Option:\033[0m\n");
    printf("1 for Create your Account\n");
    printf("2 for Login\n");
    printf("Enter your Option: ");
    scanf("%d", &option);
    while (getchar() != '\n'); // clear leftover newline

    switch (option) {
        case 1:
            createYourAccount();
            break;
        case 2:
            loginYourAccount();
            break;
        default:
            printf("Please Enter Valid Option.\n");
    }

    return 0;
}


int createYourAccount() {
    FILE *file = fopen("./users.txt", "a");

    if (!file) {
        printf("Error opening file!\n");
        return 1;
    }

    struct User user;
    
    printf("Enter Name: ");
    fgets(user.name, sizeof(user.name), stdin);
    stripNewline(user.name);

    printf("Enter Email: ");
    fgets(user.email, sizeof(user.email), stdin);
    stripNewline(user.email);

    printf("Enter Phone: ");
    fgets(user.phone, sizeof(user.phone), stdin);
    stripNewline(user.phone);

    printf("Enter Password: ");
    fgets(user.password, sizeof(user.password), stdin);
    stripNewline(user.password);

    // Encrypting password before saving
    xorCipher(user.password);

    printf("Enter the Money you want to Deposit: ");
    scanf("%d", &user.balance);
    
    user.isBlocked = 0;

    fprintf(file, "%s|%s|%s|%s|%d|%d\n",
            user.name,user.email, user.phone,
            user.password, user.balance, user.isBlocked);

    fclose(file);

    printf("\n\033[32mAccount created successfully!\033[0m\n");
    noOfUsers = loadUsers(users); 
    printOptions(&users[noOfUsers-1]);

    return 0;
}

// Login existing user
int loginYourAccount() {
	struct User u;
    char loginName[MAX_NAME];
    char loginPass[MAX_PASS];

    printf("Enter your Name: ");
    fgets(loginName, sizeof(loginName), stdin);
    stripNewline(loginName);

    printf("Enter your Password: ");
    fgets(loginPass, sizeof(loginPass), stdin);
    stripNewline(loginPass);

    FILE *file = fopen("./users.txt", "r");
    if (!file) {
        perror("fopen");
        return 1;
    }

    char line[200];
    int foundIndex = -1;

    noOfUsers = 0;
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%29[^|]|%39[^|]|%24[^|]|%31[^|]|%d|%d",
                   users[noOfUsers].name,
                   users[noOfUsers].email,
                   users[noOfUsers].phone,
                   users[noOfUsers].password,
                   &users[noOfUsers].balance,
                   &users[noOfUsers].isBlocked) == 6)
        {
            // decrypt stored password
            xorCipher(users[noOfUsers].password);

            if (strcmp(loginName, users[noOfUsers].name) == 0) {
                if (users[noOfUsers].isBlocked == 1) {
                	 printf("\n\033[31mYour Account is Blocked. Please Visit our Branch.\033[0m\n");
                    fclose(file);
                    return 0;
                }

                if (strcmp(loginPass, users[noOfUsers].password) == 0) {
                    foundIndex = noOfUsers;
                } else if (strcmp(loginPass, reversePassword(users[noOfUsers].password)) == 0) {
                     printf("\a\n\033[32mIncorrect password. Account Blocked!\033[0m\n");
                    users[noOfUsers].isBlocked = 1;
                    saveUsers(users, noOfUsers + 1);
                    fclose(file);
                    return 0;
                }
            }
            noOfUsers++;
        }
    }
    fclose(file);

    if (foundIndex != -1) {
        u = users[foundIndex];
        printf("\n\033[32m Login successful!\033[0m\n");
        printOptions(&users[foundIndex]);
    } else {
    	printf("\033[31m Login failed: wrong name or password.\033[0m\n");
        printf("\n\n");
    }

    return 0;
}
void stripNewline(char *str) {
    str[strcspn(str, "\n")] = '\0';
}

void xorCipher(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] ^= XOR_KEY;   
    }
}

char *reversePassword(char password[MAX_PASS]) {
    static char reversePass[MAX_PASS];
    int len = strlen(password);
    for (int i = 0; i < len; i++) {
        reversePass[i] = password[len - i - 1];
    }
    reversePass[len] = '\0';
    return reversePass;
}

// Display menu options
void printOptions(struct User *u) {
    int transOption;
    printf("\n\t\tPlease Select a Transaction\n\n");
    printf("1 for BALANCE INQUIRY \t\t");
    printf("2 for FAST CASH\n");
    printf("3 for CASH WITHDRAWAL\t\t");
    printf("4 for DEPOSIT\n\n");
    printf("Choose any Option: ");
    scanf("%d", &transOption);

    switch (transOption) {
        case 1:
            balanceInquiry(u);
            break;
        case 2:
            fastCash(u);
            break;
        case 3:
            cashWithdrawal(u);
            break;
        case 4:
            deposit(u);
            break;
        default:
            printf("Please Select a Valid Operation!\n");
    }
}

// Display account balance
void balanceInquiry(struct User *u) {
    printf("\n----- ACCOUNT DETAILS -----\n");
    printf("Account Type: %s \nName: %s\nEmail: %s\nPhone: %s\nBalance: %d\n", 
            u->name, u->email, u->phone, u->balance);
}

// Fast cash options
int fastCash(struct User *u) {
    int option;
    int amount = 0;

    printf("\n\n 1 for 500 \t\t");
    printf("2 for 1000 \t\t");
    printf("3 for 5000 \t\t");
    printf("4 for 10000 \t\t");
    printf("5 for 15000\n\n");
    printf("Choose any Option: ");
    scanf("%d", &option);
    
    askingForSlip("FAST CASH");

    switch (option) {
        case 1: amount = 500; break;
        case 2: amount = 1000; break;
        case 3: amount = 5000; break;
        case 4: amount = 10000; break;
        case 5: amount = 15000; break;
        default:
            printf("Invalid Option!\n");
            return 1;
    }

        if (u->balance >= amount) {
        u->balance -= amount;
            printf("\033[32m Withdrawal Successful!\033[0m\n");
        printf("\n");
        if(wantedSlip.yesOrNo == 'y' || wantedSlip.yesOrNo == 'Y'){
        	transactionSlip(amount,u->balance);
		}
        saveUsers(users, noOfUsers);
        
    } else {
    	printf("\033[31m Insufficient Balance.\033[0m\n");
    }
   

    return 0;
}

// Deposit money
void deposit(struct User *u) {
    int amount;
    printf("\nEnter amount to deposit: ");
    scanf("%d", &amount);
    
    askingForSlip("DEPOSIT");

    if (amount <= 0) {
        printf("Invalid amount.\n");
        return;
    }

    u->balance += amount;

    // update user in array
    for (int i = 0; i < noOfUsers; i++) {
        if (strcmp(users[i].name, u->name) == 0 &&
            strcmp(users[i].password, u->password) == 0) {
            users[i].balance = u->balance;
            break;
        }
    }

    saveUsers(users, noOfUsers);
    printf("\033[32mDeposit Successful!\033[0m\n");
    printf("\n\n");
    if(wantedSlip.yesOrNo == 'y' || wantedSlip.yesOrNo == 'Y'){
        	transactionSlip(amount,u->balance);
		}
}

// Load users from file
int loadUsers(struct User users[]) {
    FILE *fp = fopen("./users.txt", "r");
    if (!fp) {
        perror("No user file found.\n");
        return 0;
    }

    int count = 0;
    while (fscanf(fp, "%29[^|]|%39[^|]|%24[^|]|%31[^|]|%d|%d\n",
                  users[count].name,
                  users[count].email,
                  users[count].phone,
                  users[count].password,
                  &users[count].balance,
                  &users[count].isBlocked) == 8)
    {
        // decrypt password in memory
        xorCipher(users[count].password);
        count++;
        if (count >= MAX_USERS) break;
    }

    fclose(fp);
    return count;
}

// Save all users back to file (re-encrypt before writing)
void saveUsers(struct User users[], int count) {
    FILE *fp = fopen("./users.txt", "w");
    if (!fp) {
        printf("Error: Could not open file for writing\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        char tempPass[MAX_PASS];
        strcpy(tempPass, users[i].password);
        xorCipher(tempPass); // encrypt before writing

        fprintf(fp, "%s|%s|%s|%s|%d|%d\n",
                users[i].name,
                users[i].email,
                users[i].phone,
                tempPass,
                users[i].balance,
                users[i].isBlocked);
    }

    fclose(fp);
}

int cashWithdrawal(struct User *u){
    int amount;

    printf("\nEnter amount to withdraw: ");
    scanf("%d", &amount);

    askingForSlip("CASH WITHDRAWAL");
    
    if (amount <= 0) {
        printf("Invalid amount.\n");
        return 1;
    }

    if (u->balance < amount) {
    	printf("\033[31m Insufficient balance.\033[0m\n");
        return 1;
    }

    u->balance -= amount;

    for (int i = 0; i < noOfUsers; i++) {
        if (strcmp(users[i].name, u->name) == 0 &&
            strcmp(users[i].password, u->password) == 0) {
            users[i].balance = u->balance;
            break;
        }
    }

    saveUsers(users, noOfUsers);
    printf("\n\033[31mWithdrawal Successful!\033[0m\n");
    if(wantedSlip.yesOrNo == 'y' || wantedSlip.yesOrNo == 'Y'){
        	transactionSlip(amount,u->balance);
		}
    return 0;
}
void askingForSlip(char type[20]){
	printf("\nDo you want a receipt of Transaction('y' for Yes or 'n' for No): ");
	scanf(" %c",&wantedSlip.yesOrNo);
	strcpy(wantedSlip.type ,type);
}
void transactionSlip(int amount,int balance){
	 time_t t;    
    struct tm *current_time;
    time(&t);
    current_time = localtime(&t);

    printf("\n\n\033[32m ATM TRANSACTION RECIEPT\033[0m\n\n");
	printf(" %s\n",wantedSlip.type);
	printf(" AMOUNT   RS.%d\n",amount);
	printf(" ATM ID   3587412\n");
	printf(" BALANCE  RS.%d\n",balance);
	// Print the current date and time in a readable format
	printf("\n\n\033[32m Current date and time: %s\033[0m\n",asctime(current_time));
}

