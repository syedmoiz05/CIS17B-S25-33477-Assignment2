#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <stdexcept>


using namespace std;


// prompts the user to enter valid integer input
int intval (const string &prompt) {
    while(true) {
    cout << prompt; 
    string line; 
    getline(cin, line);
    try {
        return stoi(line);
    } catch (const invalid_argument &e) {
        cout << "invalid input, please try again: \n";
    }

}
}


// prompts the user to enter valud string input cannot be empty
string stringval(const string &prompt, bool empty = false) {
    while(true) {
        cout << prompt;
        string line;
        getline(cin, line);

        if(empty || !line.empty())
        return line;
        cout << "Invalid input, cannot be empty please try again: \n";
    }

}

// Book class
// title, author, ISBN, avalability
class Book {
    private: 
    string Title, Author;
    int ISBN;
    bool Avalability;

    public:
    Book (const string &aTitle, const string &aAuthor, int aISBN) { // constructor to inilize the book
        Title = aTitle;
        Author = aAuthor;
        ISBN = aISBN; 
        Avalability = true;
    }

    void CheckOut() {  // check out the book returns false if checked out
        Avalability = false;
    }

    void CheckIn() {  // check in the book retunns true once it's checked in
        Avalability = true;
    }

    bool IsAvalable() const { // checkls for avalability 
        return Avalability;
    }

    int returnISBN() const {  // checks the ISBN number
        return ISBN;
    }

    string ReturnTitle() const { 
        return Title;
    }

    string ReturnAuthor() const {  
        return Author;
    }

    void CreateTitle(const string &newTitle) { // you can modify the title of the private member
        Title = newTitle;

    }

    void CreateAuthor(const string &newAuthor) { // you can modify the author of the priovate member
        Author = newAuthor;

    }

};


// user class
// name, userid, borrowed books
class User {
    protected: 
    string name;
    int UserID;
    vector<int> BorrowedBooks; // vector to store ISBN of borrowed books

    public: 

    User (string aName, int aUserID) {
        name = aName;
        UserID = aUserID;
    }

    virtual ~User() = default; // made destructor virtual for better cleanup in derived classes 

    string Name() const {
        return name;
    }

    void setName(const string &aName) {
        name = aName;
    }

    int ID() const {
        return UserID;
    }

    virtual string Role() const = 0; // makes user an abstarct class, ie.  has to be implmented in sub classes

    void BorrowBook(int ISBN) {     // Borrows the book putting it into the borrowed books vector
        BorrowedBooks.push_back(ISBN);
    }

    void ReturnBook(int ISBN) {   // returns the books by removing it from th borrowed books vector
        BorrowedBooks.erase(remove(BorrowedBooks.begin(), BorrowedBooks.end(), ISBN), 
        BorrowedBooks.end());
    }

    bool AlreadyBorrowed(int ISBN) const { // checks to see if the book is already borrowed inside the vector
        return (find(BorrowedBooks.begin(), BorrowedBooks.end(), ISBN) != BorrowedBooks.end());
    }

  
};

class Student : public User {
    public: 

    Student(string aName, int aUserID) : User(aName, aUserID) {}

    string Role() const override { // reuses code in user but needs implmentation for role this time
        return "Student";  // retunring a string showing the type of user
    }


};

class Faculty : public User {
    public: 
    Faculty (string aName, int aUserID) : User(aName, aUserID) {}

    string Role() const override {   // same as student but for faculty
        return "Faculty";  
    }


};

class Library {
    private: 
    Library() : UserIDcounter(1) {}; // private constructor, to ensure only one inistance of the Library class
    static Library* instance; 
    int UserIDcounter; // counter to auto geenrate ID's

    public:
    map<int, User*> users; // a Map to store userID to pointer of User
    vector<Book> books;

    static Library* getInstance() { 
        if(!instance)
        instance = new Library();
        return instance;
    }

    ~Library() { // destructor to clean up dynamically allocated users

        for (auto &user : users) {
            delete user.second;
        }

    }

    
    Library(const Library&) = delete; // delete copy constructor
    Library& operator=(const Library&) = delete; // delete copy assignment operator


   void AddBook () {
    string name = stringval("Enter the name of the book: \n", false);
    string Author = stringval("Enter the name of the author: \n", false);
    int ISBN = intval("Enter the ISBN number: \n");
    
    // checks for duplicates
    for(auto &book : books) {
        if (book.returnISBN() == ISBN) {
            cout << "ERROR: Book already exists with this ISBN: \n";
            return;
        }
    }
    // checks for duplicates above if not then add it to the vector
        books.push_back(Book(name, Author, ISBN));
        cout << "Book has succesfully been added: \n";
    

   }

   void EditBook() {
         int ISBN = intval("Enter the ISBN number of the book you want to edit: \n");
         auto book = find_if(books.begin(), books.end(), [ISBN](const Book &b) {
              return b.returnISBN() == ISBN;
         });
    
         if (book == books.end()) {
              cout << "ERROR: Book not found with this ISBN: \n";
              return;
         }
    
         // get the new title and the new author
         string newTitle = stringval("Enter the new title of the book( blank if no edit): \n", true);
         string newAuthor = stringval("Enter the new author of the book( blank if no edit): \n", true);

         if (!newTitle.empty())
            book->CreateTitle(newTitle);
            if (!newAuthor.empty())
            book->CreateAuthor(newAuthor);
    
    
         cout << "Book has been successfully edited: \n";



   }

   void RemoveBook() {

    int ISBN = intval("Enter the ISBN number of the book you wish to remove: \n");
    // use remove if to remove the book from the vector
    auto book = remove_if(books.begin(), books.end(), [ISBN](const Book &b) {
        return b.returnISBN() == ISBN;
    });

    // check to see if the book was found
    if (book != books.end()) {
        books.erase(book, books.end());
        cout << "Book has successfully been removed: \n";
    } else {
        cout << "ERROR: Book not found with this ISBN: \n";
    }

    }


   void AddUser() {
    string name = stringval("Enter the username of the user: \n", false); 
    int type;
    while (true) {
     type = intval("Enter the type of the user(1 for student, 2 for faculty): \n");
     if (type == 1 || type == 2) {
         break;
     }
     cout << "Invalid input, please try again: \n";
    }
    // counter to create different ID's
    int id = UserIDcounter++;

    // checks for tpye of user and adds it to the map
    if (type == 1) {
        users[id] = new Student(name, id);
    } else if (type == 2) {
        users[id] = new Faculty(name, id);
        cout << "User has been successfully added: \n";
    } 

   }

   void RemoveUser() {
    // checks for the ID within map and deletes it if found, returns error if not
    int id = intval("Enter the ID of the user you wish to remove: \n");
    if (users.find(id) == users.end()) {
        cout << "ERROR: User not found with this ID: \n";
        return;
    }
    // removes user
    delete users[id];
    users.erase(id);
    cout << "User has been successfully removed: \n";

   }


   void EditUser() {
    // checks for valid ID and edits the user name
    int id = intval("Enter the ID of the user you wish to edit: \n");
    if (users.find(id) == users.end()) {
        cout << "ERROR User not found with this ID: \n";
    }
    // gets the new name and edits it
    User *user = users[id];
    string aName = stringval("Enter the new name of the user(blank if no edit): \n", true);
    if (!aName.empty())
    user->setName(aName);
    cout << "User has been successfully edited: \n";


   }

   void CheckOutBook() {
    int userid = intval("Enetr the ID of the user: \n");
    if (users.find(userid) == users.end()) {
        cout << "ERROR: User not found with this ID: \n";
        return;
    }
    // checks for the ISBN and then checks book out if avalable
    int isbn = intval("Eneter the ISBN of the book: \n");
    for (auto &book : books) {
        if (book.returnISBN() == isbn) {
           if(!book.IsAvalable()) {
               cout << "ERROR: Book is not avalable: \n";
               return;
           }
           book.CheckOut();
           users[userid]->BorrowBook(isbn);
            cout << "Book has been succesfully checked out: \n";
            return;
        }
    }


   }

   void CheckInBook() {
    int userid = intval("Enetr the ID of the user: \n");
    if (users.find(userid) == users.end()) {
        cout << "ERROR: User not found with this ID: \n";
        return;
    }
    // checks to see if user has the book
    int isbn = intval("Eneter the ISBN of the book: \n");
    if(!users[userid]->AlreadyBorrowed(isbn)) {
        cout << "ERROR: User has not borrowed this book. \n";
        return;
    }
      // checks in the book
    for (auto &book : books) {
        if (book.returnISBN() == isbn) {
            book.CheckIn();
            users[userid]->ReturnBook(isbn);
            cout << "Book has been succesfully checked in: \n";
            return;
        }
    }


   }


   void AllBooks() {
    // chekcs to see if vector is empty
    if (books.empty()) {
        cout << "No books in the Library database: \n";
        return;
    }
    // prints out the books
    for (const auto& book : books) {
        cout << "Title: " << book.ReturnTitle() << " || Author: " << book.ReturnAuthor() << " || ISBN: " << book.returnISBN() 
        << " || Avalability: " << (book.IsAvalable() ? "Avalable" : "Unavalable") << "\n";
    }


   }

   void AllUsers() {
    // checks to see if map is empty
    if (users.empty()) {
        cout << "No users in the Library database: \n";
        return;
    }
    // prints out the users
    for (const auto &user : users) {
        cout << "UserID: " << user.first << " || User Name: " << user.second->Name() << " || Role: " << user.second->Role() << '\n';
    }


   }


};


Library* Library::instance = nullptr; // initializing the static member

// seperate menu functions 

void BookMenu(Library* Library) {
    while (true) {
        cout << "\n Manage Books: \n";
        cout << "1. Add a book: \n";
        cout << "2. Edit a book: \n";
        cout << "3. Remove a book: \n";
        cout << "4. Return to main menu: \n";
        int choice = intval("Enter your choice: \n");

        switch (choice) {
            case 1: Library->AddBook();
            break;
            case 2: Library->EditBook();
            break;
            case 3: Library->RemoveBook();
            break;
            case 4: return;
            default: cout << "Invalid choice, please try again: \n";

    }
}
}

void UserMenu(Library* Library) {
    while (true) {
        cout << "\n Manage Users: \n";
        cout << "1. Add a User: \n";
        cout << "2. Edit a User: \n";
        cout << "3. Remove a User: \n";
        cout << "4. Return to main menu: \n";
        int choice = intval("Enter your choice: \n");

        switch (choice) {
            case 1: Library->AddUser();
            break;
            case 2: Library->EditUser();
            break;
            case 3: Library->RemoveUser();
            break;
            case 4: return;
            default: cout << "Invalid choice, please try again: \n";

    }
}
}

void TransactionMenu(Library* Library) {
    while (true) {
        cout << "\n Manage Transactions: \n";
        cout << "1. Check out book: \n";
        cout << "2. Check in book: \n";
        cout << "3. Show all books: \n";
        cout << "4. Show all users: \n";
        cout << "5. Return to main menu: \n";
        int choice = intval("Enter your choice: \n");

        switch (choice) {
            case 1: Library->CheckOutBook();
            break;
            case 2: Library->CheckInBook();
            break;
            case 3: Library->AllBooks();
            break;
            case 4: Library->AllUsers();
            break;
            case 5: return;
            default: cout << "Invalid choice, please try again: \n";

    }
}
}




int main() {
    // library instane
    Library *Library = Library::getInstance();

    while(true) {
        cout << "\n Main Menu: \n";
        cout << "1. Manage Books: \n";
        cout << "2. Manage Users: \n";
        cout << "3. Transactions: \n";
        cout << "4. Exit: \n";
        int choice = intval("Enter your choice: \n");

        switch (choice) {
            case 1: BookMenu(Library);
            break;
            case 2: UserMenu(Library);
            break;
            case 3: TransactionMenu(Library);
            break;
            case 4: return 0;
            defualt: cout << "Invalid choice, please try again: \n";

        }
    }


return 0;
}