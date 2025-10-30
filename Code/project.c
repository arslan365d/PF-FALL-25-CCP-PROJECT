#include <stdio.h>
#include <string.h>

#define MAX_NAME 30
#define MAX_EMAIL 25
#define MAX_PHONE 25
#define MAX_PASS 32

struct User {
    char name[MAX_NAME];
    char phone[MAX_PHONE];
    char password[MAX_PASS];
    char email[MAX_EMAIL];
    int balance;
};

void stripNewline(char *str);
int createYourAccount();
int loginYourAccount(struct User *u);

int main() {
    int option;
    struct User found;

    printf("\n\t\t\tSAA BANK\n");
    printf("\t\t   The Name Of Trust\n");
    printf("Choose one Option:\n");
    printf("1 for Create your Account\n");
    printf("2 for Login\n");
    printf("Enter your Option: ");
    scanf("%d", &option);
    while (getchar() != '\n'); 

    switch (option) {
        case 1:
            createYourAccount();
            break;
        case 2:
            loginYourAccount(&found);
            break;
        default:
            printf("Please Enter Valid Option.\n");
    }

    return 0;
}

void stripNewline(char *str) {
    str[strcspn(str, "\n")] = '\0';
}

int createYourAccount() {
    FILE* file = fopen("./users.txt", "a");
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
    
    printf("Enter the Money you want to Deposit: ");
    scanf("%d",&user.balance);
    stripNewline(user.password);
    
    fprintf(file, "%s|%s|%s|%s|%d\n",
            user.name, user.email, user.phone, user.password,user.balance);

    fclose(file);
    printf("\nAccount created successfully!\n");
    return 0;
}

int loginYourAccount(struct User *u) {
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
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%29[^|]|%24[^|]|%24[^|]|%31[^|]|%d",
           u->name, u->email, u->phone, u->password, &u->balance) == 5) {

            if (strcmp(loginName, u->name) == 0 && strcmp(loginPass, u->password) == 0) {
                found = 1;
                break;
            }
        }
    }

    fclose(file);

    if (found) {
        printf("\nLogin successful!\n");
        printf("Name: %s\nEmail: %s\nPhone: %s\nBalance: %d\n", u->name, u->email, u->phone,u->balance);
    } else {
        printf("\nLogin failed: wrong name or password.\n");
    }

    return 0;
}

