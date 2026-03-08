#include <stdio.h> 
#include <string.h> // for strings functions 
#include <ctype.h> // for isdigit() and isalpha()
#include <stdlib.h>   // For atoi()

#define MAX_DONORS 100
#define MAX_RECIPIENTS 100

// Validation function prototypes

int isValidNumber(char str[]);
int isValidName(char str[]);
int isValidGender(char str[]);
int isValidBloodGroup(char str[]);
int isValidContact(char str[]);
int isValidDate(char str[]);
void toUpperCase(char str[]);

// Donor and Recipient Structures

struct donor{  // laiba
    int id;
    char name[100];
    int age;
    char bloodGroup[5];
    char gender[10];
    char contact[15];
    char Donationdate[15];
    int bloodStock; // in ml
};
struct recipient { //laiba
    int id;
    char name[100];
    int age;
    char gender[10];
    char bloodGroup[5];
    char contact[15];
};


// Function Prototypes

void addDonor(struct donor donors[],int *count); // 1  laiba
void updateDonor(struct donor donors[], int count); //2 //rumaisa +
void searchDonor(struct donor donors[], int count);  //3 rumaisa +
void viewstock(struct donor donors[], int count);  // 4 //rumaisa  +
void matchdonor(struct donor donors[], int count);  // 5 //rumaisa +
void viewalldonors(struct donor *donors, int count);  //6 laiba 
void addRecipient(struct recipient *recipients, int *rcount); // 7 //laiba 
void manageStock(struct donor *donors,int count); // 8 // hammad
void deleteDonor(struct donor *donors,int *count);  // 9 // hammad
void printMenu();  // laiba

// file handling

void loadDonors(struct donor donors[], int *count);  // rumaisa
void saveDonors(struct donor donors[], int count);  // rumaisa
void loadRecipients(struct recipient recipients[], int *rcount); // rumaisa
void saveRecipients(struct recipient recipients[], int rcount); // rumaisa

// Main Function

int main() //laiba
{
    struct donor donors[MAX_DONORS];                 // array to store donor data
    struct recipient recipients[MAX_RECIPIENTS];    // array to store recipient data
    int count = 0, rcount = 0;                     // counters for donors and recipients
    int choice;                                     // for switch case
   

    // Load saved data from files into memory

    loadDonors(donors, &count);
    loadRecipients(recipients, &rcount);
    do{
        printMenu();   // display menu to user
        printf("\nKindly enter the option no. you want to access: \n");
        scanf("%d",&choice);

        switch(choice){
            case 1:
                addDonor(donors,&count);
                break;
            case 2:
                updateDonor(donors,count);
                break;
            case 3:
                searchDonor(donors,count);
                break;
            case 4:
                viewstock(donors,count);
                break;
            case 5:
                matchdonor(donors,count);
                break;
            case 6:
                viewalldonors(donors,count);
                break;
            case 7:
                addRecipient(recipients,&rcount);
                break;
            case 8:
                manageStock(donors,count);
                break;
            case 9: 
                deleteDonor(donors,&count);
                break;
            case 10:
                printf("\nExiting.....\n");
                break;
            default:
                printf("\nInvalid choice\n");
        }

    }while(choice!=10);        // loop until user chooses to exit

   return 0;
}

// Print Menu Function

void printMenu(){
    printf("\n-------    Blood Bank Management System    -------\n");
    printf("1. Add new donor\n");
    printf("2. Update donor details\n");
    printf("3. Search donor\n");
    printf("4. View Blood Stock\n");
    printf("5. Find Donor by Blood Group\n");
    printf("6. View All Donors\n");
    printf("7. Add Recipient\n");
    printf("8. Manage Blood Stock\n");
    printf("9. Delete donor\n");
    printf("10. Exit\n"); 
}


// Load Donors from File

void loadDonors(struct donor donors[], int *count) {
    FILE *fp = fopen("donors.txt", "r"); // Open file for reading
    if (!fp) {
        printf("No donor file found. Starting fresh.\n");
        return;
    }

    // Read each donor's data into the array
    while (fscanf(fp, "%d|%[^|]|%d|%[^|]|%[^|]|%[^|]|%[^|]|%d\n",
                  &donors[*count].id,
                  donors[*count].name,
                  &donors[*count].age,
                  donors[*count].bloodGroup,
                  donors[*count].gender,
                  donors[*count].contact,
                  donors[*count].Donationdate,
                  &donors[*count].bloodStock) == 8) {
        (*count)++; // increment donor count
    }

    fclose(fp); // Close the file
}

void saveDonors(struct donor donors[], int count) {
    FILE *fp = fopen("donors.txt", "a"); // Append mode

    if (!fp) {
        printf("Error opening donor file.\n");
        return;
    }

    // Write only the last donor added (to avoid rewriting full file)
    int i = count - 1;

    fprintf(fp, "%d|%s|%d|%s|%s|%s|%d|%s\n",
            donors[i].id,
            donors[i].name,
            donors[i].age,
            donors[i].bloodGroup,
            donors[i].gender,
            donors[i].contact,
            donors[i].bloodStock,
            donors[i].Donationdate);

    fclose(fp);
}


// Load recipients from file
void loadRecipients(struct recipient recipients[], int *rcount) {
    FILE *fp = fopen("recipients.txt", "r"); // Open file for reading
    if (!fp) {
        printf("No recipient file found. Starting fresh.\n");
        return;  // Exit if file doesn't exist
    }

    // Read each line and store into the recipients array
    // Format: ID | Name | Age | Gender | BloodGroup | Contact
    while (fscanf(fp, "%d|%[^|]|%d|%[^|]|%[^|]|%[^\n]\n",
              &recipients[*rcount].id,
              recipients[*rcount].name,
              &recipients[*rcount].age,
              recipients[*rcount].gender,      // Correct order
              recipients[*rcount].bloodGroup,
              recipients[*rcount].contact) == 6)
    {
        (*rcount)++; // Increase count for each valid record
    }

    fclose(fp); // Close file after reading
}

void saveRecipients(struct recipient recipients[], int rcount) {
    FILE *fp = fopen("recipients.txt", "a"); // Append mode

    if (!fp) {
        printf("Error opening recipient file.\n");
        return;
    }

    int i = rcount - 1;

    fprintf(fp, "%d|%s|%d|%s|%s|%s\n",
            recipients[i].id,
            recipients[i].name,
            recipients[i].age,
            recipients[i].gender,
            recipients[i].bloodGroup,
            recipients[i].contact);

    fclose(fp);
}



void addDonor(struct donor donors[], int *count) {
    // Check if maximum donors reached
    if (*count >= MAX_DONORS) {
        printf("\nThe system has reached its maximum limit. No more donors can be added.\n");
        return;
    }

    char temp[100];
    int newid;

    
    // Donor ID: Validate number, uniqueness, and positive value
   
    while (1) {
        printf("\nEnter donor ID: ");
        scanf("%s", temp);
        if (!isValidNumber(temp)) {
            printf("Invalid ID! Only digits allowed.\n");
            while(getchar() != '\n'); // clear input buffer
            continue;
        }

        newid = atoi(temp);

        // Check if ID already exists
        int idExists = 0;
        for (int i = 0; i < *count; i++) {
            if (donors[i].id == newid) {
                idExists = 1;
                break;
            }
        }
        if (idExists) {
            printf("Error: Donor ID %d already exists!\n", newid);
            continue;
        }

        if (newid <= 0) {
            printf("ID must be a positive number.\n");
            continue;
        }

        break; // valid ID entered
    }
    donors[*count].id = newid;

   
    // Donor Name: Validate letters and spaces only
    
    while (1) {
        printf("Enter donor's name: ");
        scanf(" %[^\n]", temp);
        if (!isValidName(temp)) {
            printf("Invalid name! Use letters and spaces only.\n");
            continue;
        }
        break;
    }
    strcpy(donors[*count].name, temp);

    
    // Donor Age: Validate number and realistic age
   
    while (1) {
        printf("Enter donor's age: ");
        scanf("%s", temp);
        if (!isValidNumber(temp)) {
            printf("Invalid age! Enter digits only.\n");
            while(getchar() != '\n');
            continue;
        }
        int age = atoi(temp);
        if (age <= 0 || age > 120) {
            printf("Invalid age! Enter a realistic age.\n");
            continue;
        }
        donors[*count].age = age;
        break;
    }

    
    // Blood Group: Validate standard groups
   while (1) {
    printf("Enter donor's blood group (A+, A-, B+, B-, O+, O-, AB+, AB-): ");
    scanf("%s", temp);
    toUpperCase(temp);  // <-- ADD THIS
    if (!isValidBloodGroup(temp)) {
        printf("Invalid blood group!\n");
        continue;
    }
    break;
}
strcpy(donors[*count].bloodGroup, temp);

 
    
    // Gender: Validate Male/Female
   
    while (1) {
        printf("Enter donor's gender (Male/Female): ");
        scanf("%s", temp);
        if (!isValidGender(temp)) {
            printf("Invalid gender! Enter 'Male' or 'Female'.\n");
            continue;
        }
        break;
    }
    strcpy(donors[*count].gender, temp);

    
    // Contact Number: Validate digits and length
   
    while (1) {
        printf("Enter donor's contact number (7-15 digits): ");
        scanf("%s", temp);
        if (!isValidContact(temp)) {
            printf("Invalid contact number!\n");
            continue;
        }
        break;
    }
    strcpy(donors[*count].contact, temp);

    
    // Blood Stock: Validate positive number
   
    while (1) {
        printf("Enter blood stock donated in ml (450 ml = 1 unit): ");
        scanf("%s", temp);
        if (!isValidNumber(temp)) {
            printf("Invalid input! Enter digits only.\n");
            continue;
        }
        int stock = atoi(temp);
        if (stock <= 0) {
            printf("Blood stock must be greater than 0.\n");
            continue;
        }
        donors[*count].bloodStock = stock;
        break;
    }

    
    // Donation Date: Validate DDMMYYYY format
    
    while (1) {
        printf("Enter donation date (DDMMYYYY): ");
        scanf("%s", temp);
        if (!isValidDate(temp)) {
            printf("Invalid date! Enter in DDMMYYYY format.\n");
            continue;
        }
        strcpy(donors[*count].Donationdate, temp); // copy string correctly
        break;
    }

   
    // Save donor and confirm
   
    (*count)++;
    saveDonors(donors, *count);
    printf("\nNew donor has been added successfully to the database!\n");
}


// update donor 

void updateDonor(struct donor donors[], int count) {
    if (count == 0) {
        printf("No donors in the system yet.\n");
        return;
    }

    char temp[100];
    int id, donorfound = 0, b_or_p;

    
    // Get donor ID to update
    // Validate input is digits only
    
    do {
        printf("\nEnter donor ID to fetch existing donor details: ");
        scanf("%s", temp);
        if (!isValidNumber(temp)) {
            printf("Invalid ID! Only digits allowed.\n");
            continue;
        }
        id = atoi(temp);
        break;
    } while (1);

    
    // Search for donor in array
    
    for (int i = 0; i < count; i++) {
        if (donors[i].id == id) {
            donorfound = 1;
            printf("\nDonor found with ID: %d!\nKindly enter new details:\n", id);

           
            // Choose update type: stock or personal info
            
            do {
                printf("1. Update blood stock\n2. Edit personal details\nEnter your choice: ");
                scanf("%s", temp);
            } while (!isValidNumber(temp) && printf("Invalid choice! Enter 1 or 2.\n"));
            b_or_p = atoi(temp);

            if (b_or_p == 1) {
               
                // Update blood stock only
                
                do {
                    printf("Enter new blood stock in ml (450 ml = 1 unit): ");
                    scanf("%s", temp);
                    if (!isValidNumber(temp)) printf("Invalid input! Enter digits only.\n");
                } while (!isValidNumber(temp));
                donors[i].bloodStock = atoi(temp);
            }
            else if (b_or_p == 2) {
                
                // Update personal details with validation
                

                // Name
                do {
                    printf("Enter new name: ");
                    scanf(" %[^\n]", temp);
                    if (!isValidName(temp)) printf("Invalid name! Use letters and spaces only.\n");
                } while (!isValidName(temp));
                strcpy(donors[i].name, temp);

                // Age
                do {
                    printf("Enter new age: ");
                    scanf("%s", temp);
                    if (!isValidNumber(temp)) printf("Invalid age! Enter digits only.\n");
                } while (!isValidNumber(temp));
                donors[i].age = atoi(temp);

                // Blood Group

                do {
    printf("Enter new blood group: ");
    scanf("%s", temp);
    toUpperCase(temp);  // <-- ADD THIS
    if (!isValidBloodGroup(temp)) printf("Invalid blood group! Enter valid blood group.\n");
} while (!isValidBloodGroup(temp));
strcpy(donors[i].bloodGroup, temp);

                

                // Gender
                do {
                    printf("Enter new gender: ");
                    scanf("%s", temp);
                    if (!isValidGender(temp)) printf("Invalid gender! Enter 'Male' or 'Female'.\n");
                } while (!isValidGender(temp));
                strcpy(donors[i].gender, temp);

                // Contact
                do {
                    printf("Enter new contact number: ");
                    scanf("%s", temp);
                    if (!isValidContact(temp)) printf("Invalid contact! Enter digits only, 7-15 chars.\n");
                } while (!isValidContact(temp));
                strcpy(donors[i].contact, temp);

                // Donation Date
                do {
                    printf("Enter new donation date (DDMMYYYY): ");
                    scanf("%s", temp);
                    if (!isValidDate(temp)) printf("Invalid date! Enter in DDMMYYYY format.\n");
                } while (!isValidDate(temp));
                strcpy(donors[i].Donationdate, temp);
            }
            else {
                printf("Invalid option! Choose 1 or 2.\n");
                return;
            }

           
            // Save updated donor data
            
            saveDonors(donors, count);
            printf("\nDonor information has been updated successfully!\n\n");
            break;
        }
    }

    if (!donorfound) {
        printf("\nDonor with ID %d not found in the database.\nConsider adding it as a new donor.\n\n", id);
    }
}


// search donor with id 

void searchDonor(struct donor *donors, int count){
    if (count == 0) {
        printf("No donors in the system yet. Kindly add a donor first.\n");
        return;
    }

    char temp[100];
    int id, donorfound = 0;

    
    // Get donor ID with validation (digits only)
   
    do {
        printf("Kindly enter the donor ID for searching: ");
        scanf("%s", temp);
        if (!isValidNumber(temp)) {
            printf("Invalid ID! Only digits allowed.\n");
            continue;
        }
        id = atoi(temp);
        break;
    } while (1);

   
    // Search for donor in array
    
    for(int i = 0; i < count; i++){
        if(donors[i].id == id){
            donorfound = 1;

            
            // Print donor details
            
            printf("\nDonor has been found with ID: %d in the system!\n", id);
            printf("\nDETAILS OF THE DONOR OF ID: %d\n", id);
            printf("ID: %d\n", donors[i].id);
            printf("Name: %s\n", donors[i].name);
            printf("Age: %d\n", donors[i].age);
            printf("Blood Group: %s\n", donors[i].bloodGroup);
            printf("Gender: %s\n", donors[i].gender);
            printf("Contact: %s\n", donors[i].contact);
            printf("Donation Date: %s\n", donors[i].Donationdate);
            printf("\n--------------------------------------\n");
            break;
        }
    }

   
    // If donor not found
   
    if(!donorfound){
        printf("\nDonor with ID %d is not present in the database.\nConsider adding it as a new donor.\n\n", id);
    }
}

void viewstock(struct donor donors[], int count) {
    if (count == 0) {
        printf("There are zero donors in the system yet.\n");
        printf("Kindly enter new donors to be able to view stock.\n");
        return;
    }

    char groups[8][5] = {"A+", "A-", "B+", "B-", "O+", "O-", "AB+", "AB-"};
    int total[8] = {0};

   
    // Calculate total stock per blood group

    for (int i = 0; i < count; i++) {
        for (int j = 0; j < 8; j++) {
            if (strcmp(donors[i].bloodGroup, groups[j]) == 0) {
                total[j] += donors[i].bloodStock;
                break;
            }
        }
    }

   
    // Display total stock in units and remaining ml
  
    printf("\n--------------------------------------------\n");
    printf("   Blood Group   |   Units   |  Remaining ml\n");
    printf("--------------------------------------------\n");

    for (int j = 0; j < 8; j++) {
        int units = total[j] / 450;       // full units
        int remaining = total[j] % 450;   // leftover ml
        printf("       %-6s     |    %-3d    |      %-3d\n", groups[j], units, remaining);
    }

    printf("--------------------------------------------\n");
}


void matchdonor(struct donor donors[], int count) {
    if (count == 0) {
        printf("\nNo donors are available in the system yet.\n");
        printf("Kindly add donors first.\n");
        return;
    }

    char searchbloodgroup[5];

   
    // Get blood group input with validation
    
    do {
    printf("\nEnter required blood group to find matching donors: ");
    scanf("%s", searchbloodgroup);
    toUpperCase(searchbloodgroup);  // <-- ADD THIS
    if (!isValidBloodGroup(searchbloodgroup)) {
        printf("Invalid blood group! Enter valid blood group (A+, A-, B+, B-, O+, O-, AB+, AB-).\n");
    }
} while (!isValidBloodGroup(searchbloodgroup));

    int bloodfound = 0;

    
    // Search for matching donors and display details
   
    printf("\n--- Matching Donors of Blood Group: %s ---\n", searchbloodgroup);
    for (int i = 0; i < count; i++) {
        if (strcmp(donors[i].bloodGroup, searchbloodgroup) == 0) {
            bloodfound = 1;
            int units = donors[i].bloodStock / 450;
            int remaining = donors[i].bloodStock % 450;
            printf("\nID: %d\n", donors[i].id);
            printf("Name: %s\n", donors[i].name);
            printf("Age: %d\n", donors[i].age);
            printf("Gender: %s\n", donors[i].gender);
            printf("Contact: %s\n", donors[i].contact);
            printf("Donation Date: %s\n", donors[i].Donationdate);
            printf("Available Blood Stock: %d ml (%d units + %d ml remaining)\n", donors[i].bloodStock, units, remaining);
            printf("--------------------------------------\n");
        }
    }

    
    // Display summary if no donors found
    
    if (bloodfound)
        printf("\nEnd of matching donor list.\n");
    else
        printf("\nNo donors found with blood group %s.\n", searchbloodgroup);
}

void viewalldonors(struct donor *donors, int count) {
    if (count == 0) {
        printf("No donors available.\n");
        return;
    }

    // Display table header
    printf("\n-------------------------------------------------------------------------------------------------\n");
    printf("| %-3s | %-20s | %-3s | %-5s | %-6s | %-12s | %-10s |\n",
           "ID", "Name", "Age", "Blood", "Gender", "Contact", "Stock");
    printf("-------------------------------------------------------------------------------------------------\n");

    // Print each donor's details with stock converted into units + ml
    for (int i = 0; i < count; i++) {
        int units = donors[i].bloodStock / 450;     // full units
        int remaining = donors[i].bloodStock % 450; // leftover ml
        printf("| %-3d | %-20s | %-3d | %-5s | %-6s | %-12s | %2d units + %3d ml |\n",
               donors[i].id, donors[i].name, donors[i].age, donors[i].bloodGroup,
               donors[i].gender, donors[i].contact, units, remaining);
    }

    printf("-------------------------------------------------------------------------------------------------\n");
}

void addRecipient(struct recipient *recipients, int *rcount) {
    if (*rcount >= MAX_RECIPIENTS) {
        printf("Reached maximum recipient limit.\n");
        return;
    }

    char temp[100];
    int newID;

    
    // Validate and assign unique recipient ID
    
    do {
        printf("Enter recipient ID: ");
        scanf("%s", temp);

        if (!isValidNumber(temp)) {
            printf("Invalid ID! Only digits allowed.\n");
            continue;
        }

        newID = atoi(temp);

        // Check for duplicate ID
        int exists = 0;
        for (int i = 0; i < *rcount; i++)
            if (recipients[i].id == newID)
                exists = 1;

        if (exists) {
            printf("Error: Recipient ID %d already exists!\n", newID);
            newID = -1; // force retry
        }

    } while (newID <= 0);

    recipients[*rcount].id = newID;

    
    // Name
   
    do {
        printf("Enter recipient name: ");
        scanf(" %[^\n]", temp);
        if (!isValidName(temp))
            printf("Invalid name! Use letters and spaces only.\n");
    } while (!isValidName(temp));
    strcpy(recipients[*rcount].name, temp);

    
    // Age
    
    do {
        printf("Enter recipient age: ");
        scanf("%s", temp);
        if (!isValidNumber(temp))
            printf("Invalid age! Enter digits only.\n");
    } while (!isValidNumber(temp));
    recipients[*rcount].age = atoi(temp);

    // Blood Group
    
    do {
    printf("Enter recipient blood group: ");
    scanf("%s", temp);
    toUpperCase(temp);  // <-- ADD THIS
    if (!isValidBloodGroup(temp))
        printf("Invalid blood group! Enter A+, A-, B+, B-, O+, O-, AB+, AB-.\n");
} while (!isValidBloodGroup(temp));
strcpy(recipients[*rcount].bloodGroup, temp);

    
    
    // Gender
    
    do {
        printf("Enter recipient gender: ");
        scanf("%s", temp);
        if (!isValidGender(temp))
            printf("Invalid gender! Enter 'Male' or 'Female'.\n");
    } while (!isValidGender(temp));
    strcpy(recipients[*rcount].gender, temp);

    
    // Contact Number
    
    do {
        printf("Enter recipient contact number: ");
        scanf("%s", temp);
        if (!isValidContact(temp))
            printf("Invalid contact! Use 7–15 digits.\n");
    } while (!isValidContact(temp));
    strcpy(recipients[*rcount].contact, temp);

    // Save recipient
    (*rcount)++;
    saveRecipients(recipients, *rcount);
    printf("Recipient added successfully!\n");
}


void manageStock(struct donor *donors, int count) {
    if (count == 0) {
        printf("No donors available to manage stock.\n");
        return;
    }

    char temp[20];
    int id, ml, choice, found = 0;

    // ----- Get a valid Donor ID -----
    do {
        printf("Enter Donor ID to manage stock: ");
        scanf("%s", temp);
        if (!isValidNumber(temp)) {
            printf("Invalid input! Please enter digits only.\n");
            continue;
        }
        id = atoi(temp);
        if (id <= 0)
            printf("ID must be a positive number.\n");
    } while (id <= 0);

    // ----- Search for donor -----
    for (int i = 0; i < count; i++) {
        if (donors[i].id == id) {
            found = 1;

            // Display donor info
            printf("\nDonor found!\n");
            printf("ID: %d\nName: %s\nAge: %d\nBlood Group: %s\nGender: %s\nContact: %s\nAvailable Stock: %d ml\n",
                   donors[i].id, donors[i].name, donors[i].age, donors[i].bloodGroup,
                   donors[i].gender, donors[i].contact, donors[i].bloodStock);

            // ----- Choose action: increase or decrease -----
            do {
                printf("\nChoose action:\n1. Increase stock\n2. Decrease stock\nEnter choice: ");
                scanf("%s", temp);

                if (!isValidNumber(temp)) {
                    printf("Invalid input! Enter 1 or 2.\n");
                    choice = 0;
                } else {
                    choice = atoi(temp);
                    if (choice != 1 && choice != 2)
                        printf("Invalid choice! Enter 1 or 2.\n");
                }
            } while (choice != 1 && choice != 2);

            // ----- Enter amount in ml -----
            do {
                printf("Enter amount in ml (450 ml = 1 Unit): ");
                scanf("%s", temp);

                if (!isValidNumber(temp)) {
                    printf("Invalid input! Enter digits only.\n");
                    ml = -1;
                } else {
                    ml = atoi(temp);
                    if (ml <= 0)
                        printf("Amount must be a positive number.\n");
                }
            } while (ml <= 0);

            // ----- Confirm action -----
            printf("\nYou are about to %s %d ml of blood for donor %s (ID: %d).\n",
                   (choice == 1) ? "increase" : "decrease", ml, donors[i].name, donors[i].id);
            printf("Confirm? (Y/N): ");
            
            char confirm;
            scanf(" %c", &confirm);
            if (confirm != 'Y' && confirm != 'y') {
                printf("Operation cancelled.\n");
                return;
            }

            // ----- Apply stock change -----
            if (choice == 1) {
                donors[i].bloodStock += ml;
                printf("Stock increased! New stock: %d ml\n", donors[i].bloodStock);
            } else {
                if (donors[i].bloodStock >= ml) {
                    donors[i].bloodStock -= ml;
                    printf("Stock decreased! New stock: %d ml\n", donors[i].bloodStock);
                } else {
                    printf("Not enough stock to decrease! Available: %d ml\n", donors[i].bloodStock);
                }
            }

            saveDonors(donors, count);
            break;
        }
    }

    // If donor was not found
    if (!found) {
        printf("Donor with ID %d not found!\n", id);
    }
}


void deleteDonor(struct donor *donors, int *count) {
    if (*count == 0) {
        printf("No donors available to delete.\n");
        return;
    }

    char temp[20];
    int id, found = 0;

    // ----- Get a valid Donor ID -----
    do {
        printf("Enter Donor ID to delete: ");
        scanf("%s", temp);

        if (!isValidNumber(temp)) {
            printf("Invalid input! Enter digits only.\n");
            continue;
        }

        id = atoi(temp);
        if (id <= 0)
            printf("ID must be a positive number.\n");

    } while (id <= 0);

    // ----- Search for donor -----
    for (int i = 0; i < *count; i++) {
        if (donors[i].id == id) {
            found = 1;

            // Show details before deletion
            printf("\nDonor found!\n");
            printf("ID: %d\nName: %s\nAge: %d\nBlood Group: %s\nGender: %s\nContact: %s\nAvailable Stock: %d ml\n",
                   donors[i].id, donors[i].name, donors[i].age, donors[i].bloodGroup,
                   donors[i].gender, donors[i].contact, donors[i].bloodStock);

            // ----- Confirm deletion -----
            printf("Are you sure you want to delete this donor? (Y/N): ");
            char confirm;
            scanf(" %c", &confirm);

            if (confirm != 'Y' && confirm != 'y') {
                printf("Deletion cancelled.\n");
                return;
            }

            // Store name for message
            char deletedName[100];
            strcpy(deletedName, donors[i].name);

            // ----- Shift array elements left -----
            for (int j = i; j < *count - 1; j++) {
                donors[j] = donors[j + 1];
            }

            (*count)--;

            saveDonors(donors, *count);

            printf("Donor '%s' (ID: %d) deleted successfully!\n", deletedName, id);
            return;
        }
    }

    // No matching donor
    if (!found) {
        printf("Donor with ID %d not found. Enter a valid ID.\n", id);
    }
}


// Check if string contains only digits
int isValidNumber(char str[]) {
    for (int i = 0; str[i]; i++)
        if (!isdigit(str[i])) return 0;
    return 1;
}

// Check if name contains only letters and spaces
int isValidName(char str[]) {
    for (int i = 0; str[i]; i++)
        if (!isalpha(str[i]) && str[i] != ' ') return 0;
    return 1;
}

// Check if gender is exactly "Male" or "Female" (case-insensitive)
int isValidGender(char str[]) {
    return (strcasecmp(str, "Male") == 0 || strcasecmp(str, "Female") == 0);
}

// Check if blood group matches valid list
int isValidBloodGroup(char str[]) {
    char *valid[] = {"A+", "A-", "B+", "B-", "O+", "O-", "AB+", "AB-"};
    for (int i = 0; i < 8; i++)
        if (strcasecmp(str, valid[i]) == 0) return 1;
    return 0;
}

// Check contact number: digits only + length between 7 and 15
int isValidContact(char str[]) {
    int len = strlen(str);
    if (len < 7 || len > 15) return 0;

    for (int i = 0; i < len; i++)
        if (!isdigit(str[i])) return 0;

    return 1;
}

// Check date format: must be exactly 8 digits (DDMMYYYY)
int isValidDate(char str[]) {
    if (strlen(str) != 8) return 0;  

    for (int i = 0; i < 8; i++)
        if (!isdigit(str[i])) return 0;

    return 1;
}


void toUpperCase(char str[]) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]);
    }
}
