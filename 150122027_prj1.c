 #include <stdio.h>
#include <stdlib.h>
#include <time.h> 

typedef struct Number {     //number data type was created to store digits with typedef
    char digit;
    struct Number* next;
} number;

//prototype of the functions

number* reverseNum(number* head);  
void addToResult(number** result, char newDigit);
void printList(struct Number* node);
number* addLists(number* list1, number* list2);
number* multiply(number* multiplicand, number* multiplier);

//multiplication function
number* multiply(number* multiplicand, number* multiplier){
	
	number* current1 = multiplicand;
    number* current2 = multiplier;
	number* result=NULL;
	number* totalResult=NULL;
	int carry = 0;             //carry will be calculated
	int shift = 0;               //shift will be calculated
	int decimal1 = 0; // Ýlk sayý için ondalýk basamak sayýsý
    int decimal2 = 0;

	//loop that will multiply the digits one by one
	while(current2!=NULL){     
		result=NULL;
		if(current2->digit=='.'){
			current2=current2->next;
			continue;
		}else{
			current1 = multiplicand;
			while(current1!=NULL){
				if(current1->digit=='.'){
					current1 = current1->next;
					continue;
				}else{
					//multiplication
                    int tempResult = (current2->digit-'0') * (current1->digit-'0') + carry;
                    carry = tempResult / 10;
                    char digit = (tempResult % 10+'0');  
                    
                    addToResult(&result, digit);   //add result to linked list
        
                    current1 = current1->next;    //move to next node
                }
                
            }
            //if there is carry left, it will be added to the last node
            if (carry > 0) {
                addToResult(&result, (char)(carry+'0'));
                carry = 0;    //carry will be reset
            }
            current2 = current2->next;   //move to next node in multiplier
        }
        
        shift++;     //shift is increased and shifting starts from the second result
        int i;
        for(i=1;i<shift;i++){
        	number* newNode = (number*)malloc(sizeof(number));
            newNode->digit = '0';
            newNode->next = NULL;
            
            number* temp = result;
            
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newNode;
		}
		
		printList(result);  //prints intermediate results to the console
		
		result = reverseNum(result);  //reverse the results for addition
		

            //if totalResult is NULL, the first intermediate result is initialized
            if (totalResult == NULL) {
                totalResult = result;
            } else {
                //addition process is done by adding the intermediate results to totalResult
                totalResult = addLists(totalResult, result);
            }  
    }
    
    //calculating where to put the decimal point
    current1 = multiplicand; 
    current2 = multiplier;
    
    while (current1 != NULL) {
       if (current1->digit == '.') {
        break;      //stops the loop when the point is found
       }
       decimal1++;
       current1 = current1->next; 
    }
    while (current2 != NULL) {      //same operations for the second number
       if (current2->digit == '.') {
        break; 
       }
       decimal2++;
       current2 = current2->next; 
    }
    int totalDecimal=decimal1+decimal2; //finds how many steps in total need to be skipped
    
	int k = 1;
	number* current = totalResult;

	while(current != NULL){            /*skip as many nodes as total decimal and put the dot
		                                since the number is now written in reverse,
		                                it is counted from the beginning of the node */
			if(k == totalDecimal){ 
			number* decimalPoint = (number*)malloc(sizeof(number));
			decimalPoint->digit = '.';
			decimalPoint->next = current->next;
			current->next = decimalPoint;
			break;
		}
	 	current = current->next;
		k++;
	 }
    totalResult = reverseNum(totalResult);  //the totalResult is also reversed
  return totalResult;  
}

// the result of each multiplication is added to the list.
void addToResult(number** result, char newDigit) {
    number* newNode = (number*)malloc(sizeof(number));
    newNode->digit = newDigit;
    newNode->next = *result;
    *result = newNode;
}
// function that adds intermediate results
number* addLists(number* res1, number* res2) {
    number* result = NULL;
    number* ptr0 = NULL;
    int carry = 0;

// adds nodes unless one of the numbers comes to the end or carry is 0
    while (res1 != NULL || res2 != NULL||carry>0) {
        int sum = carry;
        if (res1 != NULL) {
            sum += (int)(res1->digit - '0');  // adds numbers to sum
            res1 = res1->next;
        }
        if (res2 != NULL) {
            sum += (int)(res2->digit - '0');
            res2 = res2->next;
        }

        carry = sum / 10;   //calculates carry
        sum = sum % 10;    //calculates the number to be written in the units place
        
        number* newNode = (number*)malloc(sizeof(number));
        newNode->digit = sum + '0';
        newNode->next = NULL;

    if (result == NULL) {      //if the list is empty, create a new list
            result = newNode;
            ptr0 = result;  
        } else {             //ensures that each new node is added to the correct place.
            ptr0->next = newNode; 
            ptr0 = newNode;  
        }
    }
    return result;
}

// reverse linked list to make addition of numbers easier
number* reverseNum(number* head) {
    number* previous = NULL;
    number* current = head;
    number* next = NULL;

    while (current != NULL) {
        next = current->next;       // save next node
        current->next = previous;  // point to current node in reverse order
        previous = current;       // Update previous
        current = next;          // move to next node
    }
    return previous;       
}

// prints the number by writing the digits in the node in order
void printList(struct Number* node) {
    while (node != NULL) {
        printf("%c", node->digit);  
        node = node->next;  
    }
    printf("\n"); 
}

int main() {
	
	clock_t start = clock();  //the timing function is started
	
	//input file is opened for reading
    FILE* file = fopen("150122027_prj1_input.txt", "r");
    if (file == NULL) {
        printf("File could not be opened\n");
        return 1;
    }

    // linked list heads
    number* multiplicand = NULL;
    number* multiplier = NULL;
    number* current1;
    number* current2;
    char ch;

    // reads the first line from the file
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') break;      //stops at the end of the line
        
        //creates a new node and adds the numbers to the head of the list
        number* newNode;
        
        newNode = malloc(sizeof(number));
        newNode->digit = ch;  
        newNode->next = multiplicand; 
        multiplicand = newNode; 
    }

    // reads the second line from the file
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') break;  

        //creates a new node and adds the numbers to the head of the list
        number* newNode;

        newNode = malloc(sizeof(number));
        newNode->digit = ch;  
        newNode->next = multiplier; 
        multiplier = newNode; 
    }

    // prints numbers in a linked list
    printf("Multiplicand: ");
    printList(multiplicand);

    printf("Multiplier: ");
    printList(multiplier); 
    
    // closes the input file
    fclose(file);
    
    // the multiplication function is called
    number* result = multiply(multiplicand, multiplier);
    
    clock_t end = clock();   //the timing function is stopped

    // the program's running time is calculated
    double totalTime = (double)(end - start) / CLOCKS_PER_SEC;
    
    // the numbers are reversed again to be written correctly.
    multiplicand = reverseNum(multiplicand);
    multiplier = reverseNum(multiplier);
    
    FILE* newFile =fopen("150122027_prj1_output.txt", "w");
    
    // print multiplicand
    fprintf(newFile, "Multiplicand: ");
    current1 = multiplicand;
    while (current1 != NULL) {
        fprintf(newFile, "%c", current1->digit);
        current1 = current1->next;
    }
    fprintf(newFile, "\n");

    // print multiplier
    fprintf(newFile, "Multiplier: ");
    current2 = multiplier;
    while (current2 != NULL) {
        fprintf(newFile, "%c", current2->digit);
        current2 = current2->next;
    }
    fprintf(newFile, "\n");
    
    // print multiplication result
    fprintf(newFile,"Result: ");
    while (result != NULL) {
        fprintf(newFile, "%c", result->digit);
        result = result->next;
    }
    fprintf(newFile, "\n");
    
    // print execution time of program
    fprintf(newFile, "Execution time of program: %f second\n", totalTime);

    return 0;
}
