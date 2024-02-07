#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>

using namespace std;

struct nutrient_Info {
    string food;
    string measure;
    double grams;
    double calories;
    double protein;
    double fat;
    double satFat;
    double fiber;
    double carbs;
    string category;

    nutrient_Info* next;
};

void merge(nutrient_Info*& head, nutrient_Info* left, nutrient_Info* right) {
    nutrient_Info temp;
    nutrient_Info* current = &temp;

    while (left != nullptr && right != nullptr) {
        if (left->grams <= right->grams) {
            current->next = left;
            left = left->next;
        }
        else {
            current->next = right;
            right = right->next;
        }
        current = current->next;
    }

    if (left != nullptr) {
        current->next = left;
    }

    if (right != nullptr) {
        current->next = right;
    }

    head = temp.next;
}

void mergeSort(nutrient_Info*& head) {
    if (head == nullptr || head->next == nullptr) {
        return;
    }

    // Split the list into two halves
    nutrient_Info* leftPtr = head;
    nutrient_Info* rightptr = head->next;

    while (rightptr != nullptr && rightptr->next != nullptr) {
        leftPtr = leftPtr->next;
        rightptr = rightptr->next->next;
    }

    nutrient_Info* right = leftPtr->next;
    leftPtr->next = nullptr;

    // Recursively sort each half
    mergeSort(head);
    mergeSort(right);

    // Merge the sorted halves
    merge(head, head, right);
}



void heapify(nutrient_Info* arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left]->grams > arr[largest]->grams)
        largest = left;

    if (right < n && arr[right]->grams > arr[largest]->grams)
        largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

// Function to perform heap sort on the linked list
void heapSort(nutrient_Info*& head) {
    if (head == nullptr || head->next == nullptr)
        return;

    // Count the number of nodes
    int count = 0;
    nutrient_Info* temp = head;
    while (temp != nullptr) {
        count++;
        temp = temp->next;
    }
    // Create a temporary array to store pointers to linked list nodes
    nutrient_Info** arr = new nutrient_Info * [count];
    temp = head;

    for (int i = 0; i < count; i++) {
        arr[i] = temp;
        temp = temp->next;
    }
    // build a max heap
    for (int i = count / 2 - 1; i >= 0; i--)
        heapify(arr, count, i);

    // heap sort
    for (int i = count - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
    // reconstruct linked list
    head = arr[0];
    temp = head;
    for (int i = 1; i < count; i++) {
        temp->next = arr[i];
        temp = temp->next;
    }
    temp->next = nullptr;

    // Cleanup: Release temp arr
    delete[] arr;
}


nutrient_Info* getTail(nutrient_Info* head) {
    while (head != nullptr && head->next != nullptr) {
        head = head->next;
    }
    return head;
}

nutrient_Info* partition(nutrient_Info* head, nutrient_Info* end, nutrient_Info*& newHead, nutrient_Info*& newEnd) {
    nutrient_Info* pivot = end;
    nutrient_Info* prev = nullptr, * cur = head, * tail = pivot;

    while (cur != pivot) {
        if (cur->grams < pivot->grams) {
            if (newHead == nullptr)
                newHead = cur;
            prev = cur;
            cur = cur->next;
        }
        else {
            if (prev)
                prev->next = cur->next;
            nutrient_Info* tmp = cur->next;
            cur->next = nullptr;
            tail->next = cur;
            tail = cur;
            cur = tmp;
        }
    }

    if (newHead == nullptr)
        newHead = pivot;

    newEnd = tail;

    return pivot;
}

// Recursive function to perform quicksort on the linked list
nutrient_Info* quicksort(nutrient_Info* head, nutrient_Info* end) {
    if (!head || head == end)
        return head;

    nutrient_Info* newHead = nullptr, * newEnd = nullptr;

    nutrient_Info* pivot = partition(head, end, newHead, newEnd);

    if (newHead != pivot) {
        nutrient_Info* tmp = newHead;
        while (tmp->next != pivot)
            tmp = tmp->next;
        tmp->next = nullptr;

        newHead = quicksort(newHead, tmp);

        tmp = getTail(newHead);
        tmp->next = pivot;
    }

    pivot->next = quicksort(pivot->next, newEnd);

    return newHead;
}

// Function to start the quicksort on the linked list
void quicksort(nutrient_Info*& head) {
    head = quicksort(head, getTail(head));
}

double stringToDouble(const string& str) {
    if (str.empty()) {
        return 0.0;
    }
    return stod(str);
}

nutrient_Info* readCSVFileRow(const string& line) {
    nutrient_Info* row = new nutrient_Info;
    istringstream ss(line);
    string cellValue;

    // Read each column from the CSV row
    getline(ss, row->food, ',');
    getline(ss, row->measure, ',');
    getline(ss, cellValue, ',');
    row->grams = stringToDouble(cellValue);
    getline(ss, cellValue, ',');
    row->calories = stringToDouble(cellValue);
    getline(ss, cellValue, ',');
    row->protein = stringToDouble(cellValue);
    getline(ss, cellValue, ',');
    row->fat = stringToDouble(cellValue);
    getline(ss, cellValue, ',');
    row->satFat = stringToDouble(cellValue);
    getline(ss, cellValue, ',');
    row->fiber = stringToDouble(cellValue);
    getline(ss, cellValue, ',');
    row->carbs = stringToDouble(cellValue);
    getline(ss, row->category);

    row->next = nullptr;  // Initialize the next pointer to nullptr

    return row;
}

void loadCsvDataLinkedList(ifstream& file, nutrient_Info*& head) {
    string line;
    // Ignore the header line
    getline(file, line);

    nutrient_Info* current = nullptr;
    while (getline(file, line)) {
        nutrient_Info* newNode = readCSVFileRow(line);

        if (head == nullptr) {
            // First node in the linked list
            head = newNode;
            current = head;
        }
        else {
            // Link the new node to the end of the list
            current->next = newNode;
            current = newNode;
        }
    }
}

void printData(const nutrient_Info* row, int index) {
    cout << left << setw(5) << index + 1
        << setw(43) << row->food
        << setw(15) << row->measure
        << setw(7) << row->grams
        << setw(10) << row->calories
        << setw(9) << row->protein
        << setw(5) << row->fat
        << setw(9) << row->satFat
        << setw(7) << row->fiber
        << setw(7) << row->carbs
        << setw(34) << row->category << endl;
}

void printLinkedList(nutrient_Info* head) {
    cout << "\n";
    for (int i = 0; i < 151; ++i) {
        std::cout << "=";
    }
    
    cout << "\n"; cout << left << setw(5) << ""
        << setw(43) << "food"
         << setw(15) << "measure"
        << setw(7) << "grams"
        << setw(10) << "calories"
        << setw(9) << "protein"
        << setw(5) << "fat"
        << setw(9) << "satFat"
        << setw(7) << "fiber"
        << setw(7) << "carbs"
        << setw(34) << "category" << endl;

    int index = 0;
    nutrient_Info* current = head;
    while (current != nullptr) {
        printData(current, index);
        current = current->next;
        index++;
    }
    cout << "\n";
    for (int i = 0; i < 151; ++i) {
        std::cout << "=";
    }
}

void deleteLinkedList(nutrient_Info*& head) {
    while (head != nullptr) {
        nutrient_Info* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    ifstream file("Nutrients_Info.csv");
    nutrient_Info* data = nullptr;

    if (file.is_open()) {
        //Read CSV data into the linked list
        loadCsvDataLinkedList(file, data);
        printLinkedList(data);

        cout << "\n\n The above is the nutrient info of a list of food";
        string choice;
        double timeTaken;
        while (true) {
            cout << "\nPlease select a sorting method to sort the food by Grams\n1)Quick Sort\n2)Merge Sort\n3)Heap Sort\nEnter Your Choice:";
            cin >> choice;
            if (choice == "1") {
                auto start = chrono::high_resolution_clock::now();
                quicksort(data);
                auto end = chrono::high_resolution_clock::now();
                timeTaken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
                break;
            }
            else if (choice == "2") {
                auto start = chrono::high_resolution_clock::now();
                mergeSort(data);
                auto end = chrono::high_resolution_clock::now();
                timeTaken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
                break;
            }
            else if (choice == "3") {
                auto start = chrono::high_resolution_clock::now();
                heapSort(data);
                auto end = chrono::high_resolution_clock::now();
                timeTaken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
                break;
            }
            else {
                cout << "Please enter a valid option";
                continue;
            }

        }
        

        printLinkedList(data);
        cout << "\n\n\nThe above is the sorted data using the selected sorting method\n";
        timeTaken *= 1e-6;
        std::cout << timeTaken << fixed << setprecision(9) << "ms\n";
        
        // Cleanup: Delete the linked list
        deleteLinkedList(data);
    }

    return 0;
}
